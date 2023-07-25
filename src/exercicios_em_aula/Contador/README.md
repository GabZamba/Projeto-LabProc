## Enunciado do Exercício

Usando a sub-rotina de escrever dígito hexadecimal, crie um programa que conte de “0” a “F” no display, incrementando a cada um segundo.

Modifique o programa para controlar o sentido e/ou a velocidade da contagem de acordo com a posição das chaves.

### Observações Importantes

1. Caso se deseje simular utilizando o QEMU, o endereço de memória das seções `.text` e `.data` do [kernel.ld](kernel.ld) deve ser alterado para algum valor acima de 0x40000000 (recomenda-se 0x41000000). É importante também alterar o vetor de interrupções para realizar saltos de longas distâncias, tal como é realizado no exercicio [Do It Yourself](../Do%20It%20Yourself/do_it_yourself_qemu.s)
