
#include "src/ray/rpc/grpc_server.h"
#include <grpcpp/impl/service_type.h>

namespace ray {
namespace rpc {

GrpcServer::GrpcServer(std::string name, const uint32_t port, int num_threads)
    : name_(std::move(name)), port_(port), is_closed_(true), num_threads_(num_threads) {
  cqs_.reserve(num_threads_);
}

void GrpcServer::Run() {
  std::string server_address("0.0.0.0:" + std::to_string(port_));

  grpc::ServerBuilder builder;
  // TODO(hchen): Add options for authentication.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials(), &port_);
  // Register all the services to this server.
  if (services_.empty()) {
    RAY_LOG(WARNING) << "No service is found when start grpc server " << name_;
  }
  for (auto &entry : services_) {
    builder.RegisterService(&entry.get());
  }
  // Get hold of the completion queue used for the asynchronous communication
  // with the gRPC runtime.
  for (int i = 0; i < num_threads_; i++) {
    cqs_.push_back(std::move(builder.AddCompletionQueue()));
  }
  // Build and start server.
  server_ = builder.BuildAndStart();
  RAY_LOG(INFO) << name_ << " server started, listening on port " << port_ << ".";

  // Create calls for all the server call factories.
  for (auto &entry : server_call_factories_and_concurrencies_) {
    for (int i = 0; i < entry.second; i++) {
      // Create and request calls from the factory.
      entry.first->CreateCall();
    }
  }
  // Start threads that polls incoming requests.
  for (int i = 0; i < num_threads_; i++) {
    polling_threads_.emplace_back(&GrpcServer::PollEventsFromCompletionQueue, this, i);
  }
  // Set the server as running.
  is_closed_ = false;
}

void GrpcServer::RegisterService(GrpcService &service) {
  services_.emplace_back(service.GetGrpcService());

  for (int i = 0; i < num_threads_; i++) {
    service.InitServerCallFactories(cqs_[i], &server_call_factories_and_concurrencies_);
  }
}

void GrpcServer::PollEventsFromCompletionQueue(int index) {
  void *tag;
  bool ok;

  // Keep reading events from the `CompletionQueue` until it's shutdown.
  while (cqs_[index]->Next(&tag, &ok)) {
    RAY_LOG(WARNING) << std::this_thread::get_id();
    auto *server_call = static_cast<ServerCall *>(tag);
    bool delete_call = false;

    if (ok) {
      switch (server_call->GetState()) {
      case ServerCallState::PENDING:
        // We've received a new incoming request. Now this call object is used to
        // track this request.
        server_call->SetState(ServerCallState::PROCESSING);
        server_call->HandleRequest();
        break;
      case ServerCallState::SENDING_REPLY:
        // GRPC has sent reply successfully, invoking the callback.
        server_call->OnReplySent();
        // The rpc call has finished and can be deleted now.
        delete_call = true;
        break;
      default:
        RAY_LOG(FATAL) << "Shouldn't reach here.";
        break;
      }
    } else {
      // `ok == false` will occur in two situations:
      // First, the server has been shut down, the server call's status is PENDING
      // Second, server has sent reply to client and failed, the server call's status is
      // SENDING_REPLY
      if (server_call->GetState() == ServerCallState::SENDING_REPLY) {
        server_call->OnReplyFailed();
      }
      delete_call = true;
    }
    if (delete_call) {
      delete server_call;
    }
  }
}

}  // namespace rpc
}  // namespace ray
