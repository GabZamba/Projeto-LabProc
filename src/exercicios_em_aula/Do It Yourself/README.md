## Enunciado do Exercício

Crie sua própria instrução
Escolha uma instrução inválida (por exemplo, um coprocessador ausente) e implemente uma instrução não existente no ARM (“máquina virtual”) à sua escolha, no tratamento da exceção de instrução inválida. (por exemplo usando algum exercício anterior: a divisão, potenciação, fatorial, etc.).

#### Importante:

1. Verifique se a instrução considerada inválida é ou não a sua nova instrução, antes de executá-la. Defina algum opcode para ela;
2. Não esqueça de **restaurar** os valores de registradores que foram alterados pela sua rotina e que não estejam relacionados com a instrução. Você pode alterar os flags, conforme a necessidade da instrução (overflow, valor zero, negativo, etc.).

### Observações Importantes

1. Caso se deseje simular utilizando o QEMU, o endereço de memória da seção `.data` do [kernel.ld](kernel.ld) deve ser alterado para algum valor acima de 0x40000000 (recomenda-se 0x41000000).
2. Foram disponibilizadas duas versões do código, [do_it_yourself.s](do_it_yourself.s) e [do_it_yourself_qemu.s](do_it_yourself_qemu.s). A primeira é para execução na placa Evaluator7T, e a segunda apresenta as modificações necessárias para ser executada no QEMU (instruções extras para saltos de longas distâncias).
3. A instrução criada funciona para todos os registradores, menos R13, R14 e R15, por apresentarem valores diferentes devido ao modo UNDEF
