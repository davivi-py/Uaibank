#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Necessário para malloc, realloc e free

// Definição da estrutura do usuário
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
    int next_id = 1;     // Contador global de ID — só cresce, nunca reutiliza
    user *clientes = NULL; // Ponteiro inicializado nulo, sem memória alocada ainda
    int opcao;

    // O laço while continua lendo os comandos de entrada até o arquivo/teclado acabar (EOF)
    while(scanf("%d", &opcao) != EOF)
    {
        switch (opcao)
        {
            case 1:
            {
                total_users++; // Incrementa o total antes de alocar a memória

                // Solicita ao sistema operacional a realocação de memória com o novo tamanho
                clientes = realloc(clientes, total_users * sizeof(user));

                // O ID vem do contador global, que nunca é reutilizado
                clientes[total_users-1].id = next_id;
                next_id++;

                // Lê a string do nome até encontrar a vírgula, e depois os números.
                // O espaço no início de " %[^,]" consome a quebra de linha residual do buffer.
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
                scanf("%d", &n); // Lê a quantidade de usuários a serem inseridos de uma vez

                int id_inicial = next_id; // Guarda qual será o ID do primeiro desse lote

                // Laço para ler e alocar os 'n' usuários
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

                // Lógica de formatação para imprimir a mensagem correta exigida pelo PDF
                if (n == 1) 
                {
                    printf("Usuário inserido com id %d.\n", id_inicial);
                } 
                else 
                {
                    printf("Usuários inseridos com id ");
                    // Imprime a lista de IDs inseridos, formatando o final com "e" e ponto.
                    for(int k = id_inicial; k <= total_users; k++) 
                    {
                        if (k == total_users) {
                            printf("%d.\n", k); // Último da lista
                        } else if (k == total_users - 1) {
                            printf("%d e ", k); // Penúltimo da lista
                        } else {
                            printf("%d, ", k); // Demais itens
                        }
                    }
                }
                break;
            }
            case 3:
            {
                int id_busca;
                scanf("%d", &id_busca);

                int encontrou = 0; // Flag para rastrear o resultado (0 = falso, 1 = verdadeiro)

                // Varre o vetor sequencialmente
                for(int i = 0; i < total_users; i++)
                {
                    if(clientes[i].id == id_busca)
                    {
                        encontrou = 1;
                        // %.2lf força a impressão de apenas duas casas decimais no saldo
                        printf("Usuário %d tem saldo de R$%.2lf.\n", clientes[i].id, clientes[i].saldo_atual);
                        break; // Interrompe a varredura ao encontrar o alvo
                    }
                }

                // Checagem externa para garantir que o vetor inteiro foi percorrido
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

                // Identifica os índices no vetor de origem e destino simultaneamente
                for(int i = 0; i < total_users; i++)
                {
                    if(clientes[i].id == id_origem) {
                        indice_origem = i;
                    }
                    if(clientes[i].id == id_destino) {
                        indice_destino = i;
                    }
                }

                // Condição 1: Ambos os índices precisam ser diferentes de -1 (existem)
                if(indice_origem != -1 && indice_destino != -1)
                {
                    // Condição 2: Regra de saldo não negativo
                    if(clientes[indice_origem].saldo_atual >= quantia)
                    {
                        // Efetua a transação matemática na memória
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
                        
                        // Sobrescreve o usuário removido movendo os elementos sucessores uma posição para trás
                        for (int j = i; j < total_users - 1; j++) 
                        {
                            clientes[j] = clientes[j + 1]; 
                        } // Chave do for corrigida aqui
                        
                        total_users--; // Decrementa a contagem
                        
                        // Reajusta o bloco de memória para refletir o novo tamanho
                        if (total_users > 0) {
                            clientes = realloc(clientes, total_users * sizeof(user));
                        } else {
                            // Se era o único usuário, libera a memória completamente
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

    // --- GERANDO O ARQUIVO DE SAÍDA ---
    // A função fopen com modo "w" (write) cria o arquivo (ou sobrescreve se já existir)
    FILE *arquivo = fopen("example.txt", "w");
    
    if (arquivo != NULL) 
    {
        for(int i = 0; i < total_users; i++)
        {
            // fprintf funciona exatamente como o printf, mas direciona a saída para o arquivo de texto
            fprintf(arquivo, "%s, %d, %.2lf\n", clientes[i].nome, clientes[i].idade, clientes[i].saldo_atual);
        }
        fclose(arquivo); // É obrigatório fechar o arquivo para salvar fisicamente no disco
    }

    // Liberação final da memória Heap alocada antes do programa encerrar
    if (clientes != NULL) {
        free(clientes);
    }

    return 0; // Código de saída padrão indicando execução bem-sucedida
}