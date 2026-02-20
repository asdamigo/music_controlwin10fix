#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if _WIN32 || _WIN64		//Mudança que torna possivel o programa ser compilado em sistemas windows 10,
#include <windows.h>		//pois o usleep() não funciona por si so em tal sistema operacional.
#define usleep(us) Sleep((us) / 1000)
#else
#include <unistd.h>
#endif
#include <time.h>

typedef struct Musica{
    char nome[50];
    char autor[50];
    struct Musica *prox;
}Musica;

FILE *pont_arq;
int abrir_playlist();
int adicionar_musica(Musica **inicio, Musica **fim);
int listar_musicas(Musica *inicio);
int alterar_ordem_musicas(Musica **inicio);
int ler_playlist(Musica **inicio, Musica **fim);
int excluir_musica(Musica **inicio, Musica **fim);
int musica_existe(Musica *inicio, const char *nome, const char *autor);
int salvar_playlist(Musica *inicio);


int main(){
    Musica *inicio = NULL;
    Musica *fim = NULL;
    int opcao = 0;
    int aleatorio = 0;
    srand(time(NULL));

    abrir_playlist();
    ler_playlist(&inicio, &fim);

    printf("Escolha uma das opções do menu:\n");
    
    while (opcao != 5)
    {
    printf("Menu:\n");
    printf("1 - Adicionar musica a playlist\n");
    printf("2 - Listar musicas da playlist\n");
    printf("3 - Alterar ordem de musicas da playlist\n");
    printf("4 - Excluir musica da playlist\n");
    printf("5 - Sair do programa\n");
    scanf("%d", &opcao);
    getchar();
    switch(opcao){
        case 1:
            printf("Opção escolhida: Adicionar musica a playlist\n");
            adicionar_musica(&inicio, &fim);
            break;
        case 2:
            printf("Opção escolhida: Listar musicas da playlist\n");
            listar_musicas(inicio);
            break;
        case 3:
            printf("Opção escolhida: Alterar ordem de musicas da playlist\n");
            alterar_ordem_musicas(&inicio);
            break;
        case 4:
            printf("Opção escolhida: Excluir musica da playlist\n");
            excluir_musica(&inicio, &fim);
            break;
        case 5:
            printf("\nSair do Programa\n");
            printf("\nSalvando playlist\n");
            salvar_playlist(inicio);
            printf("\n[");
            for(int i=0; i<30; i++){
                aleatorio= 50000  + rand()% (150000 - 50000 + 1); 
                printf("=");
                usleep(aleatorio); 
            }
            printf("]\n");
            usleep(1000000);
            printf("Playlist salva com sucesso! Até a próxima!\n");
            usleep(1000000);
            break;
        default:
            printf("Opção inválida! Digite uma opção válida.\n");
            break;
    }  
    }
    

    if(pont_arq != NULL) fclose(pont_arq);
    
    return 0;

}

int salvar_playlist(Musica *inicio){
    pont_arq = fopen("playlist.txt", "w");
    if(pont_arq == NULL){
        printf("ERRO AO ABRIR PLAYLIST!");
        return 1;
    }

    Musica *atual = inicio;
    while(atual != NULL){
        fprintf(pont_arq, "%s|%s\n", atual->nome, atual->autor);
        atual = atual->prox;
    }

    fclose(pont_arq);
    return 0;
}

int excluir_musica(Musica **inicio, Musica **fim){
    if(*inicio == NULL){
        printf("A playlist está vazia!\n");
        return 0;
    }

    char nome[50], autor[50];
    printf("Digite o nome da música a ser excluída: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = '\0'; 

    printf("Digite o autor da música a ser excluída: ");
    fgets(autor, 50, stdin);
    autor[strcspn(autor, "\n")] = '\0'; 

    if(!musica_existe(*inicio, nome, autor)){
        printf("A música '%s - %s' não existe na playlist!\n", nome, autor);
        return 1;
    }

    Musica *atual = *inicio;
    Musica *anterior = NULL;

    while(atual != NULL){
        if(strcmp(atual->nome, nome) == 0 && strcmp(atual->autor, autor) == 0){
            if(anterior == NULL){
                *inicio = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            if(atual == *fim){
                *fim = anterior;
            }
            free(atual);
            printf("Música '%s - %s' excluída da playlist!\n", nome, autor);
            return 0;
        }
        anterior = atual;
        atual = atual->prox;
    }

    return 1; 
}

int alterar_ordem_musicas(Musica **inicio){
    if(*inicio == NULL){
        printf("A playlist está vazia!\n");
        return 0;
    }

    int pos1, pos2;
    printf("Digite a posição da primeira música a ser trocada: ");
    scanf("%d", &pos1);
    getchar();
    printf("Digite a posição da segunda música a ser trocada: ");
    scanf("%d", &pos2);
    getchar();

    if(pos1 <= 0 || pos2 <= 0){
        printf("Posições inválidas! As posições devem ser maiores que zero.\n");
        return 1;
    }

    Musica *musica1 = NULL, *musica2 = NULL;
    Musica *atual = *inicio;
    int contador = 1;

    while(atual != NULL){
        if(contador == pos1){
            musica1 = atual;
        }
        if(contador == pos2){
            musica2 = atual;
        }
        atual = atual->prox;
        contador++;
    }

    if(musica1 == NULL || musica2 == NULL){
        printf("Posições inválidas! Não existem músicas nas posições informadas.\n");
        return 1;
    }

    char temp_nome[50], temp_autor[50];
    
    strcpy(temp_nome, musica1->nome);
    strcpy(temp_autor, musica1->autor);
    
    strcpy(musica1->nome, musica2->nome);
    strcpy(musica1->autor, musica2->autor);
    
    strcpy(musica2->nome, temp_nome);
    strcpy(musica2->autor, temp_autor);

    printf("Músicas trocadas com sucesso!\n");

    return 0;
}

int listar_musicas(Musica *inicio){
    if(inicio == NULL){
        printf("A playlist está vazia!\n");
        return 0;
    }

    Musica *atual = inicio;
    int contador = 1;

    printf("Músicas na playlist:\n");
    while(atual != NULL){
        printf("%d - %s - %s\n", contador, atual->nome, atual->autor);
        atual = atual->prox;
        contador++;
    }

    return 0;
}

int musica_existe(Musica *inicio, const char *nome, const char *autor){
    Musica *atual = inicio;
    while(atual != NULL){
        if(strcmp(atual->nome, nome) == 0 && strcmp(atual->autor, autor) == 0){
            return 1; 
        }
        atual = atual->prox;
    }
    return 0; 
}

int adicionar_musica(Musica **inicio, Musica **fim){
    Musica *nova_musica = (Musica*)malloc(sizeof(Musica));
    if(nova_musica == NULL){
        printf("ERRO AO ALOCAR MEMORIA!");
        return 1;
    }

    printf("Digite o nome da musica: ");
    fgets(nova_musica->nome, 50, stdin);
    nova_musica->nome[strcspn(nova_musica->nome, "\n")] = '\0'; 
    
    printf("Digite o autor da musica: ");
    fgets(nova_musica->autor, 50, stdin);
    nova_musica->autor[strcspn(nova_musica->autor, "\n")] = '\0'; 

    if(musica_existe(*inicio, nova_musica->nome, nova_musica->autor)){
    printf("Erro: Essa música já existe na playlist!\n");
    free(nova_musica);
    return 1;
}

    nova_musica->prox = NULL;

    if(*inicio == NULL){
        *inicio = nova_musica;
        *fim = nova_musica;
    } else {
        (*fim)->prox = nova_musica;
        *fim = nova_musica;
    }

    printf("Musica adicionada a playlist: %s - %s\n", nova_musica->nome, nova_musica->autor);

    return 0;
}

int abrir_playlist(){
    pont_arq = fopen("playlist.txt", "a");
    pont_arq == NULL ? printf("ERRO AO ABRIR OU CRIAR PLAYLIST!\n") : printf("Playlist aberta com sucesso!\n");
    return 0;
}
int ler_playlist(Musica **inicio, Musica **fim){
    pont_arq = fopen("playlist.txt", "r");
    if(pont_arq == NULL){
        printf("ERRO AO ABRIR PLAYLIST!");
        return 1;
    }

    char linha[100];
    while(fgets(linha, sizeof(linha), pont_arq) != NULL){
        Musica *nova_musica = (Musica*)malloc(sizeof(Musica));
        if(nova_musica == NULL){
            printf("ERRO AO ALOCAR MEMORIA!");
            return 1;
        }

        char *token = strtok(linha, "|");
        if(token != NULL){
            strncpy(nova_musica->nome, token, 50);
            nova_musica->nome[strcspn(nova_musica->nome, "\n")] = '\0'; 
        }

        token = strtok(NULL, "\n");
        if(token != NULL){
            strncpy(nova_musica->autor, token, 50);
            nova_musica->autor[strcspn(nova_musica->autor, "\n")] = '\0'; 
        }

        nova_musica->prox = NULL;

        if(*inicio == NULL){
            *inicio = nova_musica;
            *fim = nova_musica;
        } else {
            (*fim)->prox = nova_musica;
            *fim = nova_musica;
        }
    }

    fclose(pont_arq);
    return 0;
}
