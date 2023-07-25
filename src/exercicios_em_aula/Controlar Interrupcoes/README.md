## Enunciado do Exercício

Escreva funções em assembler ou combinando C e assembler para habilitar e desabilitar as interrupções IRQ e FIQ, alterando os bits correspondentes no registrador cpsr. Essas funções devem retornar o estado anterior do bit.

```C
int enable_irq(void);
int disable_irq(void);
int enable_fiq(void);
int disable_fiq(void);
```

### Observações Importantes

1. Atente-se à versão do `arm-none-eabi` no Linker Options do Makefile. Caso ela não seja a mesma do computador, ocorrerá erro ao compilar
2. Atente-se aos breakpoints e ao jump automaticamente inserido ao executar o comando `make gdb-auto`, pois caso você apague ou renomeie estas funções, este comando pode resultar em erro
3. Caso se deseje simular utilizando o QEMU, o endereço de memória das seções `.text` e `.data` do [kernel.ld](./kernel.ld) deve ser alterado para algum valor acima de 0x40000000 (recomenda-se 0x41000000)
