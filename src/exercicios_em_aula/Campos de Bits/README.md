## Enunciado do Exercício

Vários registradores do S3C4510B possuem campos de bit, por exemplo `IOPCON`, `TMOD`, `ULCON`, `UCON` e `USTAT`.
Como dar nomes aos seus bits e campos de bits para facilitar a leitura do código em C?

### Observações Importantes

1. Atente-se aos breakpoints e ao jump automaticamente inserido ao executar o comando `make gdb-auto`, pois caso você apague ou renomeie estas funções, este comando pode resultar em erro
2. Caso se deseje simular utilizando o QEMU, o endereço de memória das seções `.text` e `.data` do [kernel.ld](kernel.ld) deve ser alterado para algum valor acima de 0x40000000 (recomenda-se 0x41000000). Além disso, é importante lembrar de alterar, da mesma forma, o valor do endereço do ponteiro (um valor sugerido está comentado no início do [código](campos_de_bit.c))
