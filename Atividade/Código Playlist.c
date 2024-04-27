#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALBUNS 10
#define MAX_ARTISTAS 500

struct Artista {
    char nome[100];
    char genero[100];
    char local[100];
    char albuns[MAX_ALBUNS][100];
    int num_albuns;
};

void ler_artistas_arquivo(struct Artista artistas[], int *num_artistas, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Arquivo invalido\n");
        exit(1);
    }

    *num_artistas = 0;
    int num_linha = 0;
    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\n")] = 0;
        if (*num_artistas < MAX_ARTISTAS) {
            switch (num_linha % 4) {
                case 0:
                    strcpy(artistas[*num_artistas].nome, linha);
                    break;
                case 1:
                    strcpy(artistas[*num_artistas].genero, linha);
                    break;
                case 2:
                    strcpy(artistas[*num_artistas].local, linha);
                    break;
                case 3:
                    strcpy(artistas[*num_artistas].albuns[artistas[*num_artistas].num_albuns++], linha);
                    break;
            }
            num_linha++;
            if (num_linha % 4 == 0) {
                (*num_artistas)++;
            }
        }
    }
    fclose(arquivo);
}

void escrever_artistas_arquivo(struct Artista artistas[], int num_artistas, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Arquivo invalido\n");
        exit(1);
    }

    for (int i = 0; i < num_artistas; i++) {
        fprintf(arquivo, "%s\n", artistas[i].nome);
        fprintf(arquivo, "%s\n", artistas[i].genero);
        fprintf(arquivo, "%s\n", artistas[i].local);
        for (int j = 0; j < artistas[i].num_albuns; j++) {
            fprintf(arquivo, "%s\n", artistas[i].albuns[j]);
        }
        fprintf(arquivo, "==========\n");
    }
    fclose(arquivo);
}

void inserir_artista(struct Artista artistas[], int *num_artistas) {
    if (*num_artistas >= MAX_ARTISTAS) {
        printf("Numero maximo de artistas atingido.\n");
        return;
    }

    struct Artista novo_artista;
    printf("Nome do artista: ");
    scanf("%s", novo_artista.nome);
    printf("Genero musical: ");
    scanf("%s", novo_artista.genero);
    printf("Local de criacao/nascimento: ");
    scanf("%s", novo_artista.local);
    printf("Albuns - digite 'fim' para encerrar:\n");
    novo_artista.num_albuns = 0;
    while (1) {
        printf("Album: ");
        scanf("%s", novo_artista.albuns[novo_artista.num_albuns]);
        if (strcmp(novo_artista.albuns[novo_artista.num_albuns], "fim") == 0) {
            break;
        }
        novo_artista.num_albuns++;
    }
    artistas[*num_artistas] = novo_artista;
    (*num_artistas)++;
    escrever_artistas_arquivo(artistas, *num_artistas, "artistas.txt");
    printf("Artista adicionado\n");
}

void remover_artista(struct Artista artistas[], int *num_artistas) {
    char nome_artista[100];
    printf("Nome a ser removido: ");
    scanf("%s", nome_artista);

    int indice = -1;
    for (int i = 0; i < *num_artistas; i++) {
        if (strcmp(artistas[i].nome, nome_artista) == 0) {
            indice = i;
            break;
        }
    }

    if (indice != -1) {
        for (int i = indice; i < *num_artistas - 1; i++) {
            artistas[i] = artistas[i + 1];
        }
        (*num_artistas)--;
        escrever_artistas_arquivo(artistas, *num_artistas, "artistas.txt");
        printf("Artista removido com sucesso.\n");
    } else {
        printf("Artista não encontrado.\n");
    }
}

void editar_artista(struct Artista artistas[], int num_artistas) {
    char nome_artista[100];
    printf("Nome a ser editado: ");
    scanf("%s", nome_artista);

    int indice = -1;
    for (int i = 0; i < num_artistas; i++) {
        if (strcmp(artistas[i].nome, nome_artista) == 0) {
            indice = i;
            break;
        }
    }

    if (indice != -1) {
        printf("Novo genero: ");
        scanf("%s", artistas[indice].genero);
        printf("Novo local de criacao/nascimento: ");
        scanf("%s", artistas[indice].local);
        printf("Novos albuns - digite 'fim' para encerrar:\n");
        artistas[indice].num_albuns = 0;
        while (1) {
            printf("Album: ");
            scanf("%s", artistas[indice].albuns[artistas[indice].num_albuns]);
            if (strcmp(artistas[indice].albuns[artistas[indice].num_albuns], "fim") == 0) {
                break;
            }
            artistas[indice].num_albuns++;
        }
        escrever_artistas_arquivo(artistas, num_artistas, "artistas.txt");
        printf("Artista editado com sucesso.\n");
    } else {
        printf("Artista nao encontrado\n");
    }
}

void buscar_artista_binaria(struct Artista artistas[], int num_artistas) {
    char nome_artista[100];
    printf("Nome do artista: ");
    scanf("%s", nome_artista);

    int inicio = 0, fim = num_artistas - 1, meio;
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        if (strcmp(artistas[meio].nome, nome_artista) == 0) {
            printf("Artistas encontrados:\n");
            printf("Nome: %s\n", artistas[meio].nome);
            printf("Genero: %s\n", artistas[meio].genero);
            printf("Local: %s\n", artistas[meio].local);
            printf("Albuns:\n");
            for (int i = 0; i < artistas[meio].num_albuns; i++) {
                printf("- %s\n", artistas[meio].albuns[i]);
            }
            return;
        } else if (strcmp(artistas[meio].nome, nome_artista) < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    printf("Artista nao encontrado\n");
}

void buscar_album_sequencial(struct Artista artistas[], int num_artistas) {
    char nome_album[100];
    printf("Nome do album: ");
    scanf("%s", nome_album);

    int encontrado = 0;
    printf("Artistas com album '%s':\n", nome_album);
    for (int i = 0; i < num_artistas; i++) {
        for (int j = 0; j < artistas[i].num_albuns; j++) {
            if (strcmp(artistas[i].albuns[j], nome_album) == 0) {
                printf("- %s\n", artistas[i].nome);
                encontrado = 1;
                break;
            }
        }
    }
    if (!encontrado) {
        printf("Nenhum artista encontrado com album '%s'.\n", nome_album);
    }
}

void listar_artistas(struct Artista artistas[], int num_artistas) {
    printf("Lista de artistas:\n");
    for (int i = 0; i < num_artistas; i++) {
        printf("%d. %s\n", i + 1, artistas[i].nome);
        printf("   Gênero: %s\n", artistas[i].genero);
        printf("   Local: %s\n", artistas[i].local);
        printf("   Álbuns:\n");
        for (int j = 0; j < artistas[i].num_albuns; j++) {
            printf("   - %s\n", artistas[i].albuns[j]);
        }
    }
}

int main() {
    struct Artista artistas[MAX_ARTISTAS];
    int num_artistas;
    const char *nome_arquivo = "artistas.txt";

    ler_artistas_arquivo(artistas, &num_artistas, nome_arquivo);

    int opcao;
    while (1) {
        printf("\nMenu:\n");
        printf("1-Inserir novo artista\n");
        printf("2-Remover artista\n");
        printf("3-Editar artista\n");
        printf("4-Buscar artista\n");
        printf("5-Buscar album\n");
        printf("6-Listar artistas\n");
        printf("7-Sair\n");
        printf("Escolha uma das opcoes acima: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserir_artista(artistas, &num_artistas);
                break;
            case 2:
                remover_artista(artistas, &num_artistas);
                break;
            case 3:
                editar_artista(artistas, num_artistas);
                break;
            case 4:
                buscar_artista_binaria(artistas, num_artistas);
                break;
            case 5:
                buscar_album_sequencial(artistas, num_artistas);
                break;
            case 6:
                listar_artistas(artistas, num_artistas);
                break;
            case 7:
                printf("Saindo\n");
                exit(0);
            default:
                printf("Opcao invalida, tente novamente.\n");
        }
    }

    return 0;
}
