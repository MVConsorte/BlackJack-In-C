#include "BlackJack21.h"
#include <stdio.h>
#include <locale.h>

int main(){
    setlocale(LC_ALL, "portuguese"); //perceba que, por estar em porugues, o ponto (num real) ? virgula agora para float
    Player jogador = {"Smith", 100}, dealer = {"John",2000};
    //round(&dealer, &jogador);
    blackJack_game(&dealer, &jogador);

    getchar(); //aguardar para terminar = pause
    return 0;
}