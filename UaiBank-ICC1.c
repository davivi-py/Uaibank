#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

// Definição da struct do usuário
typedef struct 
{
    int id;
    char nome[110];
    int idade;
    double saldo_atual;
} user;

int main()
{
    int total_users = 0; // Rastreador da quantidade de usuários ativos
    int next_id = 1;     // Contador >> global << de ID 
    user *clientes = NULL; // Ponteiro inicializado nulo sem memória alocada ainda
    int opcao;

    // O laço while continua lendo os comandos de entrada até o teclado acabar de digitar
    while(scanf("%d", &opcao) != EOF)
    {
        switch (opcao)
        {
            case 1:
            {
                total_users++; // Incrementa o total de users antes de alocar a memória

                // realocação de memória com o novo tamanho
                clientes = realloc(clientes, total_users * sizeof(user));

                // O ID vem do contador global, que nunca é reutilizado
                clientes[total_users-1].id = next_id;
                next_id++;

                // lê a string do nome até encontrar a vírgula e depois os números
                scanf(" %[^,], %d, %lf",
                    clientes[total_users-1].nome,
                    &clientes[total_users-1].idade,
                    &clientes[total_users-1].saldo_atual);

                printf("Usuário inserido com id %d.\n", clientes[total_users-1].id);
                
                break;
            }
            case 2:
            {
                int n;
                scanf("%d", &n); //le a quantidade de usuários a serem inseridos de uma vez

                int id_inicial = next_id; // guarda qual será o ID do primeiro desse lote

                // laço para ler e alocar os 'n' usuários
                for(int i = 0; i < n; i++)
                {
                    total_users++;
                    
                    clientes = realloc(clientes, total_users * sizeof(user));
                    clientes[total_users-1].id = next_id;
                    next_id++;

                    scanf(" %[^,], %d, %lf",
                        clientes[total_users-1].nome,
                        &clientes[total_users-1].idade,
                        &clientes[total_users-1].saldo_atual);
                }

                // logica de formatação para imprimir a mensagem correta exigida pelo PDF
                if (n == 1) 
                {
                    printf("Usuário inserido com id %d.\n", id_inicial);
                } 
                else 
                {
                    printf("Usuários inseridos com id ");
                    // imprime a lista de IDs inseridos formatando o final com "e" e ponto.
                    for(int k = id_inicial; k <= total_users; k++) 
                    {
                        if (k == total_users) {
                            printf("%d.\n", k); // ultimo da lista
                        } else if (k == total_users - 1) {
                            printf("%d e ", k); // penultimo da lista
                        } else {
                            printf("%d, ", k); // demais itens
                        }
                    }
                }
                break;
            }
            case 3:
            {
                int id_busca;
                scanf("%d", &id_busca);

                int encontrou = 0; // variavel pra rastrear o resultado (0 = falso, 1 = verdadeiro)

                // varre o vetor sequencialmente
                for(int i = 0; i < total_users; i++)
                {
                    if(clientes[i].id == id_busca)
                    {
                        encontrou = 1;
                        // %.2lf força a impressão de apenas duas casas decimais no saldo
                        printf("Usuário %d tem saldo de R$%.2lf.\n", clientes[i].id, clientes[i].saldo_atual);
                        break; // interrompe a varredura ao encontrar o alvo
                    }
                }

                // checagem externa para garantir que o vetor inteiro foi percorrido
                if(encontrou == 0)
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

                // identifica os indices no vetor de origem e destino simultaneamente
                for(int i = 0; i < total_users; i++)
                {
                    if(clientes[i].id == id_origem) {
                        indice_origem = i;
                    }
                    if(clientes[i].id == id_destino) {
                        indice_destino = i;
                    }
                }

                //ambos os índices precisam ser diferentes de -1 (existem)
                if(indice_origem != -1 && indice_destino != -1)
                {
                    // verificar se saldo não é negativo
                    if(clientes[indice_origem].saldo_atual >= quantia)
                    {
                        // calcula o saldo atual
                        clientes[indice_origem].saldo_atual -= quantia;
                        clientes[indice_destino].saldo_atual += quantia;
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

                for(int i = 0; i < total_users; i++)
                {
                    if(clientes[i].id == id_remover)
                    {
                        achou_para_remover = 1;
                        
                        // sobrescreve o usuario removido movendo os elementos sucessores uma posição pra tras
                        for (int j = i; j < total_users - 1; j++) 
                        {
                            clientes[j] = clientes[j + 1]; 
                        }
                        total_users--; // decrementa a contagem
                        
                        // reajusta o bloco de memoria para refletir o novo tamanho do vetor
                        if (total_users > 0) {
                            clientes = realloc(clientes, total_users * sizeof(user));
                        } else {
                            // se era o unico usuario libera a memoria completamente
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

    
    // a função fopen com modo "w" (write) cria o arquivo
    FILE *arquivo = fopen("example.txt", "w");
    
    if (arquivo != NULL) 
    {
        for(int i = 0; i < total_users; i++)
        {
            // direcionando a saída para o arquivo de texto
            fprintf(arquivo, "%s, %d, %.2lf\n", clientes[i].nome, clientes[i].idade, clientes[i].saldo_atual);
        }
        fclose(arquivo); // é obrigatório fechar o arquivo pra salvar fisicamente no disco
    }

    // liberação final da memoria 
    if (clientes != NULL) {
        free(clientes);
    }

    return 0; 
}
