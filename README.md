# Projeto da disciplina de sistemas operacionais


__Disciplina:__ Sistemas Operacionais \
__Período:__ 2023/2 \
__Professor:__ Alba Cristina 

Este projeto tem como objetivo a implementação de um escalonador de processos, utilizando o modelo lottery scheduling.

## Envolvidos

| Nome | Matrícula |
| ---- | --------- |
| Pablo Araujo | 19/0049901 |
| Samuel||
| Eduardo||

## Projeto

A especificação do projeto pode ser encontrada em [projeto.pdf](projeto.pdf).

## Ideia inicial

### Lógica geral
- Criação das entidades: processo, escalonador e logger.
 - Processo: contém as informações do processo, como id, tempo de execução, prioridade, etc.
 - Escalonador: contém a lógica do escalonador, como a criação de processos, execução, etc.
 - Logger: contém a lógica de log, como a criação do makespan, tempo de execução etc.

### Chamadas UNIX

- E/S de arquivos
  - `open()`
  - `read()`
  - `write()`
  - `close()`

- Gerenciamento de processos
  - `fork()`
  - `exec()`
  - `wait()`
  - `exit()`

- Temporização
  - `alarm()`
  - `setitimer()`

### Estrutura geral
```
│
├── src/
│   ├── main.c         # Arquivo principal do programa
│   ├── process.c      # Implementação - classe Process
│   ├── process.h      # Cabeçalho - classe Process
│   ├── scheduler.c    # Implementação - classe Scheduler
│   ├── scheduler.h    # Cabeçalho - classe Scheduler
│   ├── logger.c       # Implementação - classe Logger
│   ├── logger.h       # Cabeçalho - classe Logger
│
├── include/
│   ├── common.h       # Cabeçalho com definições globais
│
├── Makefile            # Arquivo de construção do projeto
├── README.md           # Arquivo de descrição do projeto
``````

## Compilação & Execução
```bash
$ make
```

```bash
$ ./prog 
```
