
#
# Lista dos arquivos fonte
#  Inclua seus arquivos aqui
#
FONTES = interrupt.s context_change.s aux.s gpio_utils.c buffer.c sched.c main.c 

BREAKPOINTS= panic main main2 context_change stop close_thread

INIT_CODE= reset

#
# Arquivos de saída
#  Altere se quiser
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
# Opções do compilador C
#
OPTS = -g

#
# Opções do linker
#
LDOPTS = -L/usr/lib/gcc/arm-none-eabi/10.3.1/ -L/usr/lib/arm-none-eabi/lib
LDOPTS += -lc -lgcc

#
# Gerar executável
#
${EXEC}: ${OBJETOS}
	${LD} -T ${LDSCRIPT} -M=${MAP} ${OBJETOS} ${LDOPTS} -o $@

#
# Compilar arquivos em C
#
.c.o:
	${GCC} ${OPTS} -c -o $@ $<

#
# Montar arquivos em assembler
#
.s.o:
	${AS} -g -o $@ $<

#
# Limpar tudo
#
clean:
	rm -f *.o ${EXEC} ${MAP}

#
# Iniciar qemu
#
qemu: ${EXEC}
	@if lsof -Pi :1234 >/dev/null ; then\
		echo "qemu já está executando"; \
	else qemu-system-arm -s -M virt & \
	fi

#
# Iniciar openocd
#
ocd: 
	@if pgrep openocd >/dev/null ; then\
		echo "openocd já está executando"; \
	else openocd -f evaluator7t.cfg & \
	fi

#
# Iniciar gdb com ocd
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
# Iniciar gdb com ocd inserindo os breakpoints automaticamente
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
		-ex "j ${INIT_CODE}"

#
# Iniciar gdb com qemu
#
gdb-qemu: ${EXEC}
	gdb-multiarch -ex "set architecture arm" \
		-ex "target extended-remote :1234" \
		-ex "load"  ${EXEC} \

#
# Iniciar gdb com qemu inserindo os breakpoints automaticamente
#
gdb-qemu-auto: ${EXEC}
	gdb-multiarch -ex "set architecture arm" \
		-ex "target extended-remote :1234" \
		-ex "load"  ${EXEC} \
		-ex "layout src" \
		-ex "layout regs" \
		$(foreach breakpoint, $(BREAKPOINTS), -ex "b $(breakpoint)") \
		-ex "j ${INIT_CODE}"


