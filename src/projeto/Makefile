
#
# Source files list
#  Include your files here
#
FONTES = interrupt.s stubs.c mutex.s context_change.s aux.s gpio_utils.c buffer.c sched.c main.c threads.c

BREAKPOINTS= reset panic stop

INIT_CODE= reset 

#
# Output files
#  Change if needed
#
EXEC = kernel.elf
MAP = kernel.map

PREFIXO = arm-none-eabi-
LDSCRIPT = kernel.ld
AS = ${PREFIXO}as
LD = ${PREFIXO}ld
GCC = ${PREFIXO}gcc
OBJ = $(FONTES:.s=.o)
OBJETOS = $(OBJ:.c=.o)

#
# C compiler options
#
OPTS = -g

#
# Linker options
#
LDOPTS = -L/usr/lib/gcc/arm-none-eabi/10.3.1/ -L/usr/lib/arm-none-eabi/lib
LDOPTS += -lc -lgcc

#
# generate executable file
#
${EXEC}: ${OBJETOS}
	${LD} -T ${LDSCRIPT} -M=${MAP} ${OBJETOS} ${LDOPTS} -o $@

#
# Compile C files
#
.c.o:
	${GCC} ${OPTS} -c -o $@ $<

#
# Mount assembly files
#
.s.o:
	${AS} -g -o $@ $<

#
# Clean all object files
#
clean:
	rm -f *.o ${EXEC} ${MAP}

#
# Initialize QEMU
#
qemu: ${EXEC}
	@if lsof -Pi :1234 >/dev/null ; then\
		echo "qemu já está executando"; \
	else qemu-system-arm -s -M virt & \
	fi

#
# Initialize openocd
#
ocd: 
	@if pgrep openocd >/dev/null ; then\
		echo "openocd já está executando"; \
	else openocd -f evaluator7t.cfg & \
	fi

#
# Initialize gdb with ocd
#
gdb: ${EXEC}
	gdb-multiarch -ex "set architecture arm" \
		-ex "target extended-remote :3333" \
		-ex "shell sleep 0.5" \
		-ex "monitor init" \
		-ex "shell sleep 0.5" \
		-ex "monitor halt" \
		-ex "shell sleep 0.5" \
		-ex "load" \
		${EXEC}

#
# Initialize gdb with ocd, inserting the breakpoints and start automatically
#
gdb-auto: ${EXEC}
	gdb-multiarch -ex "set architecture arm" \
		-ex "target extended-remote :3333" \
		-ex "shell sleep 0.5" \
		-ex "monitor init" \
		-ex "shell sleep 0.5" \
		-ex "monitor halt" \
		-ex "shell sleep 0.5" \
		-ex "load" \
		${EXEC} \
		-ex "layout src" \
		-ex "layout regs" \
		$(foreach breakpoint, $(BREAKPOINTS), -ex "b $(breakpoint)") \
		-ex "shell sleep 0.5" \
		-ex "set tmod[0] = 0" \
		-ex "set *intpnd = *intpnd" \
		-ex "set \$$cpsr = 0x13" \
		-ex "j ${INIT_CODE}"

#
# Initialize gdb with qemu
#
gdb-qemu: ${EXEC}
	gdb-multiarch -ex "set architecture arm" \
		-ex "target extended-remote :1234" \
		-ex "load"  ${EXEC} \

#
# Initialize gdb with qemu, inserting the breakpoints and start automatically
#
gdb-qemu-auto: ${EXEC}
	gdb-multiarch -ex "set architecture arm" \
		-ex "target extended-remote :1234" \
		-ex "load"  ${EXEC} \
		-ex "layout src" \
		-ex "layout regs" \
		$(foreach breakpoint, $(BREAKPOINTS), -ex "b $(breakpoint)") \
		-ex "j ${INIT_CODE}"


