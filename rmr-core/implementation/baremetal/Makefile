CC ?= arm-none-eabi-gcc

CFLAGS := -x c -std=c11 -ffreestanding -nostdlib -nostartfiles -nodefaultlibs \
	-fno-builtin -fno-stack-protector -fomit-frame-pointer -fno-strict-aliasing \
	-fsingle-precision-constant -fno-tree-vectorize -fno-unroll-loops \
	-march=armv7e-m -mtune=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os

LDFLAGS := -Wl,--gc-sections -Wl,-T,linker.ld

all:
	$(CC) $(CFLAGS) rmr_baremetal_entry.c $(LDFLAGS) -o rmr_baremetal.elf

clean:
	rm -f rmr_baremetal.elf
