# UaiBank 💳

Sistema de gestão de usuários bancários desenvolvido em C, como projeto da disciplina **SSC0502 - Laboratório de Introdução à Ciência da Computação I** (ICMC-USP).

## Funcionalidades

- Inserção de usuário individual ou em lote
- Busca de usuário por ID
- Transferência de saldo entre contas (com validação de saldo negativo)
- Remoção de usuário por ID
- Persistência dos dados em arquivo `example.txt` ao encerrar

## Como compilar e rodar

```bash
gcc UaiBank.c -o uaibank
./uaibank
```

## Interface de comandos

O sistema lê comandos da entrada padrão em loop até EOF (`Ctrl+D` no terminal).

| Comando | Descrição | Formato |
|---|---|---|
| `1` | Inserir um usuário | `1` → `<nome>, <idade>, <saldo>` |
| `2` | Inserir vários usuários | `2` → `<n>` → `<nome>, <idade>, <saldo>` (repetir n vezes) |
| `3` | Buscar usuário por ID | `3` → `<id>` |
| `4` | Transferência entre contas | `4` → `<id_origem> <id_destino> <quantia>` |
| `5` | Remover usuário por ID | `5` → `<id>` |

## Exemplo

**Entrada:**
```
1
Joel Gamer, 13, 1000000.00
2
2
Silvia Santos, 30, 12.50
Manuella da Silva, 21, 123.89
3
1
5
1
3
1
```

**Saída:**
```
Usuário inserido com id 1.
Usuários inseridos com id 2 e 3.
Usuário 1 tem saldo de R$1000000.00.
Usuário 1 removido com sucesso.
Erro: Usuário 1 não encontrado.
```

**`example.txt` gerado:**
```
Silvia Santos, 30, 12.50
Manuella da Silva, 21, 123.89
```

## Detalhes de implementação

- Alocação dinâmica com `malloc`/`realloc`/`free` — o vetor de usuários cresce e encolhe conforme necessário
- IDs sequenciais e únicos gerados automaticamente pelo sistema
- Remoção por deslocamento dos elementos no vetor (sem buracos)
- Nenhum usuário pode ficar com saldo negativo após transferência
- Nome suporta até 100 caracteres
