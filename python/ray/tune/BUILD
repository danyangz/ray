py_test(
    name = "test_actor_reuse",
    size = "medium",
    srcs = ["tests/test_actor_reuse.py"],
    tags = ["jenkins_only"],
)

py_test(
    name = "test_automl_searcher",
    size = "small",
    srcs = ["tests/test_automl_searcher.py"],
)

py_test(
    name = "test_checkpoint_manager",
    size = "small",
    srcs = ["tests/test_checkpoint_manager.py"],
    deps = [":tune_lib"],
)

py_test(
    name = "test_cluster",
    size = "large",
    srcs = ["tests/test_cluster.py"],
    deps = [":tune_lib"],
    tags = ["jenkins_only", "exclusive"],
)

py_test(
    name = "test_commands",
    size = "medium",
    srcs = ["tests/test_commands.py"],
    deps = [":tune_lib"],
    tags = ["exclusive"],
)

py_test(
    name = "test_dependency",
    size = "small",
    srcs = ["tests/test_dependency.py"],
    deps = [":tune_lib"],
)

py_test(
    name = "test_experiment_analysis",
    size = "medium",
    srcs = ["tests/test_experiment_analysis.py"],
    deps = [":tune_lib"],
)

py_test(
    name = "test_experiment",
    size = "small",
    srcs = ["tests/test_experiment.py"],
    deps = [":tune_lib"],
)

py_test(
    name = "test_logger",
    size = "small",
    srcs = ["tests/test_logger.py"],
    deps = [":tune_lib"],
    tags = ["jenkins_only"],
)

py_test(
    name = "test_ray_trial_executor",
    size = "medium",
    srcs = ["tests/test_ray_trial_executor.py"],
    deps = [":tune_lib"],
)

py_test(
    name = "test_track",
    size = "small",
    srcs = ["tests/test_track.py"],
    deps = [":tune_lib"],
)

py_test(
    name = "test_trial_runner",
    size = "large",
    srcs = ["tests/test_trial_runner.py"],
    deps = [":tune_lib"],
    tags = ["exclusive"],
)

py_test(
    name = "test_trial_scheduler",
    size = "medium",
    srcs = ["tests/test_trial_scheduler.py"],
    deps = [":tune_lib"],
)

py_test(
    name = "test_tune_restore",
    size = "large",
    srcs = ["tests/test_tune_restore.py"],
    deps = [":tune_lib"],
    tags = ["jenkins_only", "exclusive"],
)

py_test(
    name = "test_tune_save_restore",
    size = "large",
    srcs = ["tests/test_tune_save_restore.py"],
    deps = [":tune_lib"],
    tags = ["exclusive"],
)

py_test(
    name = "test_tune_server",
    size = "medium",
    srcs = ["tests/test_tune_server.py"],
    deps = [":tune_lib"],
)

# This is a dummy test dependency that causes the above tests to be
# re-run if any of these files changes.
py_library(
    name="tune_lib",
    srcs = glob(["**/*.py"], exclude=["tests/*.py"]),
)
