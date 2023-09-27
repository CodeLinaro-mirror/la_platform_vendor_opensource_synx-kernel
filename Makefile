SRC := $(shell pwd)
KBUILD_OPTIONS := SYNX_ROOT=$(SRC)

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules $(KBUILD_OPTIONS)

modules_install:
	$(MAKE) M=$(SRC) -C $(KERNEL_SRC) modules_install
%:
	$(MAKE) -C $(KERNEL_SRC) M=$(M) $@ $(KBUILD_OPTIONS)
clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) clean

