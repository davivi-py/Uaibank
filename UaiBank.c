#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    int id;
    char nome[110];
    int idade;
    double saldo_atual;
} user;

int main()
{
    int total_users = 0;
    int next_id = 1;
    user *clientes = NULL;
    int opcao;

    while (scanf("%d", &opcao) != EOF)
    {
        switch (opcao)
        {
            case 1:
            {
                total_users++;
                clientes = realloc(clientes, total_users * sizeof(user));

                clientes[total_users - 1].id = next_id;
                next_id++;

                scanf(" %109[^,], %d, %lf",
                      clientes[total_users - 1].nome,
                      &clientes[total_users - 1].idade,
                      &clientes[total_users - 1].saldo_atual);

                printf("Usuário inserido com id %d.\n", clientes[total_users - 1].id);
                break;
            }

            case 2:
            {
                int n;
                scanf("%d", &n);
                int id_inicial = next_id;

                for (int i = 0; i < n; i++)
                {
                    total_users++;
                    clientes = realloc(clientes, total_users * sizeof(user));
                    clientes[total_users - 1].id = next_id;
                    next_id++;

                    scanf(" %109[^,], %d, %lf",
                          clientes[total_users - 1].nome,
                          &clientes[total_users - 1].idade,
                          &clientes[total_users - 1].saldo_atual);
                }

                int id_final = next_id - 1;

                if (n == 1)
                {
                    printf("Usuário inserido com id %d.\n", id_inicial);
                }
                else
                {
                    printf("Usuários inseridos com id ");
                    for (int k = id_inicial; k <= id_final; k++)
                    {
                        if (k == id_final) {
                            printf("%d.\n", k);
                        } else if (k == id_final - 1) {
                            printf("%d e ", k);
                        } else {
                            printf("%d, ", k);
                        }
                    }
                }
                break;
            }

            case 3:
            {
                int id_busca;
                scanf("%d", &id_busca);
                int encontrou = 0;

                for (int i = 0; i < total_users; i++)
                {
                    if (clientes[i].id == id_busca)
                    {
                        encontrou = 1;
                        printf("Usuário %d tem saldo de R$%.2lf.\n", clientes[i].id, clientes[i].saldo_atual);
                        break;
                    }
                }

                if (encontrou == 0)
                {
                    printf("Erro: Usuário %d não encontrado.\n", id_busca);
                }
                break;
            }

            case 4:
            {
                int id_origem, id_destino;
                double quantia;
                scanf("%d %d %lf", &id_origem, &id_destino, &quantia);

                int indice_origem = -1;
                int indice_destino = -1;

                for (int i = 0; i < total_users; i++)
                {
                    if (clientes[i].id == id_origem) {
                        indice_origem = i;
                    }
                    if (clientes[i].id == id_destino) {
                        indice_destino = i;
                    }
                }

                if (indice_origem != -1 && indice_destino != -1)
                {
                    if (clientes[indice_origem].saldo_atual >= quantia)
                    {
                        clientes[indice_origem].saldo_atual -= quantia;
                        clientes[indice_destino].saldo_atual += quantia;
                        printf("Transferência de R$%.2lf realizada com sucesso de %d para %d.\n",
                               quantia, id_origem, id_destino);
                    }
                    else
                    {
                        printf("Erro: Saldo insuficiente para realizar a transferência.\n");
                    }
                }
                else
                {
                    printf("Erro: Um dos usuários não foi encontrado.\n");
                }
                break;
            }

            case 5:
            {
                int id_remover;
                scanf("%d", &id_remover);
                int achou_para_remover = 0;

                for (int i = 0; i < total_users; i++)
                {
                    if (clientes[i].id == id_remover)
                    {
                        achou_para_remover = 1;

                        for (int j = i; j < total_users - 1; j++)
                        {
                            clientes[j] = clientes[j + 1];
                        }

                        total_users--;

                        if (total_users > 0) {
                            clientes = realloc(clientes, total_users * sizeof(user));
                        } else {
                            free(clientes);
                            clientes = NULL;
                        }

                        printf("Usuário %d removido com sucesso.\n", id_remover);
                        break;
                    }
                }

                if (achou_para_remover == 0)
                {
                    printf("Erro: Usuário %d não encontrado.\n", id_remover);
                }
                break;
            }

            default:
                break;
        }
    }

    FILE *arquivo = fopen("example.txt", "w");
    if (arquivo != NULL)
    {
        for (int i = 0; i < total_users; i++)
        {
            fprintf(arquivo, "%s, %d, %.2lf\n", clientes[i].nome, clientes[i].idade, clientes[i].saldo_atual);
        }
        fclose(arquivo);
    }

    if (clientes != NULL) {
        free(clientes);
    }

    return 0;
}
