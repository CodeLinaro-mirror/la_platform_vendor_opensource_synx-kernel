load(":synx_module_build.bzl", "create_module_registry")

SYNX_KERNEL_ROOT = "synx-kernel"

synx_modules = create_module_registry([":synx_headers"])
register_synx_module = synx_modules.register

register_synx_module(
    name = "synx-driver",
    path = "msm",
    srcs = [
        "synx/synx_compat.c",
        "synx/synx_compat_debug.c",
        "synx/synx_ioctl.c",
        "synx/synx_stub.c",
        "synx/synx_test_ioctl.c",
    ],

    # Configs are handled by config_options = []
    config_deps = {
        "CONFIG_QTI_HW_FENCE": [
            "//vendor/qcom/opensource/mm-drivers/hw_fence:%b_msm_hw_fence",
            "//vendor/qcom/opensource/mm-drivers/hw_fence:hw_fence_headers",
        ],
        "CONFIG_SYNX_IMPL": [
            "//vendor/qcom/opensource/synx-core:%b_synx-impl_synx",
        ],
    },
)
