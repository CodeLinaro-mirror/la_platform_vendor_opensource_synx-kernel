load(":synx_modules.bzl", "synx_modules")
load(":synx_module_build.bzl", "define_consolidate_perf_modules")

def define_neo61():
    define_consolidate_perf_modules(
        target = "neo_la",
        registry = synx_modules,
        modules = [
            "synx-driver",
            "qcom_ipc_lite",
        ],
        config_options = [
            "TARGET_SYNX_ENABLE",
            "CONFIG_MSM_GLOBAL_SYNX",
        ],
    )
