#
# Kurzanleitung
# =============
#
# make		-- Baut den Kernel.
# make all
#
# make install	-- Baut den Kernel und transferiert ihn auf den Server.
# 		   Das Board holt sich diesen Kernel beim nächsten Reset.
#
# make clean	-- Löscht alle erzeugten Dateien.
#


#
# Quellen
#
LSCRIPT = kernel.lds
OBJ = os/start.o
OBJ += os/ivt.o
OBJ += os/system/interrupt_trampolines.o
OBJ += os/lib/strings.o
OBJ += os/lib/memory.o
OBJ += os/test/test_runner.o
OBJ += os/kernel.o
OBJ += os/driver/uart/uart.o
OBJ += os/driver/uart/uart_io.o
OBJ += os/driver/timer.o
OBJ += os/driver/uart/printer/sync_uart_printer.o
OBJ += os/driver/uart/printer/uart_printer.o
OBJ += os/lib/io.o
OBJ += os/lib/ds/ringbuffer.o
OBJ += os/lib/ds/static_linked_list.o
OBJ += os/lib/ds/static_heap.o
OBJ += os/lib/timer.o
OBJ += os/lib/printer/printer.o
OBJ += os/lib/printer/string_printer.o
OBJ += os/driver/io.o
OBJ += os/system/exceptions.o
OBJ += os/system/interrupts.o
OBJ += os/system/scheduler.o
OBJ += os/system/mmu.o
OBJ += os/system/thread.o
OBJ += os/system/supervisor.o
OBJ += os/system/uart_system_controller.o



### USER CODE ###

OBJ += user/user_thread.o
OBJ += user/register-checker/regcheck_asm.o
OBJ += user/register-checker/regcheck.o
OBJ += user/lib/memory.o
OBJ += user/lib/strings.o
OBJ += user/lib/io.o
OBJ += user/lib/system.o
OBJ += user/lib/printer/string_printer.o
OBJ += user/lib/printer/printer.o

#
# Konfiguration
#
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

CFLAGS = -Wall -Wextra -ffreestanding -mcpu=cortex-a7 -O2
CPPFLAGS = -Iinclude

DEP = $(OBJ:.o=.d)

#
# Regeln
#
.PHONY: all
all: kernel kernel.bin dump

-include $(DEP)

%.o: %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -o $@ -c $<

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -I common/ -MMD -MP -o $@ -c $<

kernel: $(LSCRIPT) $(OBJ)
	$(LD) -T$(LSCRIPT) -o $@ $(OBJ) $(LIBGCC)

kernel.bin: kernel
	$(OBJCOPY) -Obinary --set-section-flags .bss=contents,alloc,load,data $< $@

kernel.bin.gz: kernel.bin
	gzip -v9 $<

kernel.img: kernel.bin
	 mkimage -A arm -T standalone -C none -a 0x8000 -d $< $@
	
dump:
	$(OBJDUMP) -D kernel > kernel_dump.s

.PHONY: install
install: kernel.img
	cp -v kernel.img /srv/tftp
#	arm-install-image $<

.PHONY: clean
clean:
	rm -f kernel kernel.bin kernel.bin.gz kernel.img kernel_dump.s
	rm -f $(OBJ)
	rm -f $(DEP)


