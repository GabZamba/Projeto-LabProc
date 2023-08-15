# Escalonador MultiLevel Feedback Queue (MLFQ)

Este projeto foi desenvolvido para a disciplina de Laboratório de Microprocessadores da Escola Politécnica da Universidade de São Paulo, sendo implementado mesclando a linguagem de programação C e a linguagem de montagem do processador ARMv4.


## Resumo sobre o MLFQ

O algoritmo de escalonamento implementado para a MLFQ foi o de decréscimo de prioridade, e boost-up periódico dos processos, que consiste no seguinte conjunto de regras:

1. Se um processo A possuir prioridade maior do que um processo B, então A executa e B não;

2. Se dois ou mais processos apresentam a mesma prioridade, então eles executam em Round-Robin;

3. Todo novo processo criado apresentará a prioridade máxima;

4. Quando o tempo total de execução de um processo superar o de um quantum, a sua prioridade é diminuída.

    - Isto ocorre seja em uma única execução (interrompida por temporizador), ou ao longo de múltiplas execuções (retornos sucessivos através de yield);

5. Após um período de tempo T, todos os processos são movidos à máxima prioridade (boost-up).

Para mais informações, recomenda-se a leitura do capítulo correspondente do livro [Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)

## Simulação e Teste

O projeto foi desenvolvido utilizando-se o software de emulação QEMU, e a placa de desenvolvimento Evaluator 7T, que contém um processador ARMv4. É importante ter em mente que o ambiente QEMU não possibilita a simulação de um temporizador, então a execução nele deve ser feita com cautela, pois o comportamento diferirá em relação à placa Evaluator 7T.

Na pasta `src/` há um Makefile que contém alguns comandos simples, listados abaixo:

```make```

Realiza a compilação e linkagem dos arquivos .c e .s listados no campo SRC

```make clean```

Realiza a limpeza dos arquivos objeto (.o) criados durante a compilação e linkagem

```make ocd``` ou ```make qemu```

Realiza a inicialização, respectivamente, do openocd para a comunicação com a placa Evaluator 7T, e do qemu-system-arm para a emulação via QEMU

```make gdb``` ou ```make gdb-qemu```

Inicia o depurador GDB utilizando, respectivamente, o código programado na placa Evaluator 7T, ou o programado no QEMU

```make gdb-auto``` ou ```make gdb-qemu-auto```

Inicia o depurador GDB utilizando, respectivamente, o código programado na placa Evaluator 7T, ou o programado no QEMU, porém já inserindo os breakpoints definidos no campo BREAKPOINTS, e saltando para o local definido no campo INIT_CODE