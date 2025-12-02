load(":synx_modules.bzl", "synx_modules")
load(":synx_module_build.bzl", "define_consolidate_perf_modules")

def define_pineapple():
    define_consolidate_perf_modules(
        target = "pineapple",
        registry = synx_modules,
        modules = [
            "synx-driver",
        ],
        config_options = [
            "TARGET_SYNX_ENABLE",
            "CONFIG_SYNX_IMPL",
        ],
    )
