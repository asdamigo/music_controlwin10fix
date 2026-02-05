#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *pont_arq;

int abrir_playlist();
int ler_playlist();

main(){
    char msc[40], author[30];
    

}

int abrir_playlist(){
    pont_arq = fopen("playlist.txt", "a");
    pont_arq == NULL ? printf("ERRO AO ABRIR OU CRIAR PLAYLIST!") : printf("Playlist aberta com sucesso!");
    return 0;
}
int ler_playlist(){
    pont_arq = fopen("playlist.txt", "r");
    pont_arq == NULL ? printf("ERRO AO ABRIR PLAYLIST!") : printf("Playlist aberta e pronta para leitura!");
    return 0;
}