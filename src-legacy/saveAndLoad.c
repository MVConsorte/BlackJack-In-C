#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "BlackJack21.h"

#define MAX_SAVES 4

/*
    Struct GameData

    Função para salvar (save)

    função para sobrescrever (overwrite)

    Função para carregar (load)

    função deletar

*/

struct GameData {
    char time_data[30];
    char player_name[50];
    float player_coin;
    char dealer_name[50];
    float dealer_coin;
    // int player_level;
    // int player_score; (level + <score>)
};
typedef struct GameData GameData;

void nameArchive(char *name, int max_length){
    printf("Salvar em (somente o nome sem extensão):\n");
    int i = 0;
    while (i < max_length - 1){
        char c = getc(stdin);

        // Verifica se o caractere é inválido
        if (c == '.' || c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|'){
            //subsituir por "_"
            c = '_';
        }

        if (c == '\n'){
            break;
        }
        name[i++] = c; //anexar string e incrementar a posição
    }
    name[i] = '\0'; //subsitituir \n por caracter nulo (fim da string)
    
    //Adicionar .txt ao final
    // alternativa crua = name[--i] = "t"; name[--i] = "x"; name[--i] = "t"; name[--i] = ".";
    snprintf(name, max_length, "%s.txt", name);

    printf("\033[H\033[J"); // Limpa o terminal usando caracteres de escape ANSI
}

void show_saves(GameData saves[], int num_saves) {
    printf("Saves disponíveis:\n");

    if (num_saves == 0) {
        printf("Não há saves.\n\n");
    } else {
        for (int i = 0; i < num_saves; i++) {
            printf("%d. %s (Nível: %d, Pontuação: %d, Dinheiro: %.2f)\n",
                   i + 1, saves[i].player_name, saves[i].player_level, saves[i].player_score, saves[i].player_coin);
        }
    }
}


void save(FILE *archive, GameData save_data[], int *num_saves, Player *user, Player *dealer){
    char name_archive[50]; // declarando variável para nome do arquivo
    nameArchive(name_archive, sizeof(name_archive)); //usuário add nome do salvamento (.txt add automaticamente)
    
    //Abrir arquivo
    archive = fopen(name_archive, "a+");
    if (archive == NULL){
        printf("Erro ao abrir o arquivo.\n");
        fclose(archive);
        return;
    }

    // Obter tempo atual
    time_t current_time; // time_t -> tipo de dado que armazena data e hora 
    time(&current_time); //obtém a data e anexa à variável
    char *time_string = ctime(&current_time); //converte para uma leitura compreensível - em inglês
    strtok(time_string, "\n"); // Remover a quebra de linha do final

    //Verificar quantidade da saves disponíveis, caso exceda, ir para função  de sobreescrever dados
    if (*num_saves >= MAX_SAVES){
        // lógica de sobreescrever dados
        printf("Número máximo de saves atingido.\n");
        fclose(archive);
        return;
    }
    
    // Preencher a estrutura GameData 
    //////// usuário ////////
    strcpy(save_data[*num_saves].time_data, time_string); //anexar no tipo referente ao horário e dia do save
    strcpy(save_data[*num_saves].player_name, user->name); //anexando nome do jogador
    save_data[*num_saves].player_coin = user->coin; //anexando dinheiro do jogador
    /////// dealer /////////
    strcpy(save_data[*num_saves].dealer_name, dealer->name); //anexando nome da banca == dealer
    save_data[*num_saves].dealer_coin = dealer->coin;
    
    // Atualizar o número de saves
    (*num_saves)++;

   // fprintf()
    fprintf(archive, "[ %s ]( %s | %.2f ){ %s | %.2f }\n", save_data[*num_saves - 1].time_data,
            save_data[*num_saves - 1].player_name, save_data[*num_saves - 1].player_coin,
            save_data[*num_saves - 1].dealer_name, save_data[*num_saves - 1].dealer_coin);
    
    fclose(archive);

}



int main() {
    setlocale(LC_ALL, "portuguese");
    Player jogador = {"Smith", 20}, dealer = {"John", 2080};
    int num_saves = 0;
    GameData save_data[MAX_SAVES];
    FILE *archive; // Declaração do ponteiro de arquivo
    save(archive, save_data, &num_saves, &jogador, &dealer);

    return 0;
}