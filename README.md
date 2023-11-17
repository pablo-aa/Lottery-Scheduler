# Projeto da disciplina de sistemas operacionais


__Disciplina:__ Sistemas Operacionais \
__Período:__ 2023.2 \
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
Descrição das chamadas UNIX utilizadas no projeto.

| Chamada | Descrição |
| --- | --- |
| `fork()` | Cria um novo processo duplicando o processo atual |
| `getpid()` | Obtém o ID do processo atual |
| `msgsnd()` | Envia uma mensagem para uma fila de mensagens |
| `msgget()` | Obtém o identificador de uma fila de mensagens |
| `msgctl()` | Realiza várias operações em uma fila de mensagens |
| `sigaction()` | Permite ao processo definir e manipular sinais |
| `execl()` | Substitui a imagem do processo atual por uma nova imagem de processo |
| `alarm()` | Agenda um alarme para o processo atual |
| `kill()` | Envia um sinal para um processo |
| `exit()` | Termina o processo atual |
| `sleep()` | Suspende a execução do processo atual por um número especificado de segundos |
| `fopen()` | Abre um arquivo |
| `fgets()` | Lê uma linha de um arquivo |

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
│   ├── message.h      # Cabeçalho - classe Message
│
├── include/
│   ├── common.h       # Cabeçalho com definições globais
│
├── Makefile            # Arquivo de construção do projeto
├── processes.txt       # Arquivo de entrada com os processos
├── projeto.pdf         # Especificação do projeto
├── README.md           # Arquivo de descrição do projeto
└── .gitignore          # Arquivo de configuração do git
```
``````

## Compilação & Execução
```bash
$ make
```

```bash
$ ./prog -v
```

```
observação: Se quiser obter mais informações sobre o processo de execução, altere '#define VERBOSE 1' em /lib/common.h
```
