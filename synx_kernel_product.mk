TARGET_SYNX_ENABLE := false
ifeq ($(TARGET_KERNEL_DLKM_DISABLE),true)
	ifeq ($(TARGET_KERNEL_DLKM_SYNX_OVERRIDE),true)
		TARGET_SYNX_ENABLE := true
	endif
else
TARGET_SYNX_ENABLE := true
endif

TARGET_SYNX_ENABLE := false
ifeq ($(TARGET_SYNX_ENABLE), true)
PRODUCT_PACKAGES += ipclite.ko
PRODUCT_PACKAGES += synx-driver.ko
endif
