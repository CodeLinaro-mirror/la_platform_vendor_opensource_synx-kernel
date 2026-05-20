load(":synx_modules.bzl", "synx_modules")
load(":synx_module_build.bzl", "define_consolidate_perf_modules")

def define_lahaina():
    define_consolidate_perf_modules(
        target = "lahaina",
        registry = synx_modules,
        modules = [
            "synx-driver",
        ],
        config_options = [
            "TARGET_SYNX_ENABLE",
        ],
    )
