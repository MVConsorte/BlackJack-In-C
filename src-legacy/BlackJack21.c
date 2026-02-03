#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <unistd.h>
#include "BlackJack21.h"

/* 
    Considerações importante:
        - scanf("%TAM[^\n]s", lista[i].nome); assim, ele irá ler tudo da linha (incluindo espaços)
        - fflush(stdin);  deseja garantir que os dados no buffer sejam escritos no arquivo 
        imediatamente, em vez de aguardar o buffer encher ou o programa terminar
        - system("cls"); (limpar o prompt a execução)
        - Lembre que há struct aninhadas (struct dentro de struct)

*/

/* BlackJack conceptions:
    [x] Montar uma estrutura (struct) Carta
    [x] Montar estrutura Baralho (ou algo que o represente)
    [x] Montar uma estrutura jogador
    [x] Estrutura Dealer
    [x] estrutura Rodada
    [x] estrutura jogo
*/

/*
    Issues:
    [] Ação Split
    [] Adicionar Banco de Dados
        [] save game
        [] load game
        [] sobreescrever save game
        [] continue game
            - escolher um load game existente
        [] *opciona* autosave
            - implementa sobreescrever save game
            - local save
*/

/*
// ajuste de função//
void save_game(Player *user, Player *dealer, int rewrite);
void memory_card(Player *user, Player *dealer, int save);

///////////////// Funções para as opções do menu ///////////////////////
// função: save e load

void new_game(Player *banca, Player *jogador){
    // lógica para começar novo jogo
    //rounds(banca, jogador);
}

void continue_game(Player *banca, Player *jogador){
    // lógica para continuar jogo
    //rounds(banca, jogador);
}
*/

////////////////////////////////////////


void blackJack_game(Player *banca, Player *jogador){
    //Art ascii: [0: Tela inicial, 1: tela de opções principal, default: Personagem movimentando = saida]
    //definir opções (Sair, continuar save e começar)
    int choice_menu;


    do{
        clear_terminal();

        // Tela inicial && escolha
        art_ascii(0);
        printf("Escolha a opções: ");
        scanf("%d", &choice_menu);

        switch (choice_menu){
            case 1: //escolheu entrar
                choice_menu = 0; // tratando excessão -> resetar valor prévio
                while(getchar() != '\n'); // limpar buffer

                do {
                    clear_terminal();
                    art_ascii(1);

                    printf("Escolha a opções: ");
                    scanf("%d", &choice_menu);

                    switch (choice_menu){
                        case 1:
                            //novo jogo -> criar arquivo ou substituir -> jogar -> armazenar dados (substituir ou novo)
                            clear_terminal();
                            rounds(banca, jogador);
                            break;

                        case 2: //continuar -> buscar arquivo salvo -> recuperar dados -> jogar -> armazenar dados (substituir ou novo)
                            clear_terminal();
                            rounds(banca, jogador);
                            break;

                        case 3: //sair
                            clear_terminal();

                            art_ascii(-1);
                            printf("\nVocê saiu do casino.\n\n");
                            sleep(2);
                            break;

                        default:
                            //printf("\nO que exatamente?\n");
                            //sleep(3);
                            while (getchar() != '\n'); // Limpa o buffer de entrada
                            clear_line();
                            break;
                    }
                    //if(choice_menu == 1 || choice_menu == 2){break;}

                }while (choice_menu != 3);

                break;

            case 2: //escolheu sair ***OK
                clear_terminal();
                printf("\n\nVocê observou o casino... e decidiu ir embora.\n\n");
                art_ascii(-1);
                break;

            default: //**** ok
                // printf("\nO que exatamente?\n");
                //sleep(3);
                while (getchar() != '\n');// Limpa o buffer de entrada
                break;

        }
    }while (choice_menu != 2);

}



/////////////////////////// Funções em uso /////////////////////////////////////

void pausa(){
    printf("\nPressione Enter para continuar...");
    while(getchar() != '\n'); //esperar até o usuário pressionar Enter
}

void clear_terminal(){
    /* Utilizando caracter ANSI, é possível manipular o terminal
    * Funciona na maioria dos terminais atuais */
    printf("\033[H\033[J"); // Limpa o terminal usando caracteres de escape ANSI
}

void clear_line(){
    printf("\033[1A"); //utilizando ANSI: Sobe o cursor uma linha acima
    printf("\033[K"); //apagar linha do cursor
}

void shuffle(Card *deck, int size){
    srand(time(NULL)); //INICIALIZAR SEMENTE DE NÚMEROS ALEatéRIOS (pad?o: tempo atual)

    for(int i = 0; i < size;i++){
        int swap = rand()%(size - 1); //gerar um valor de posição aleatério de 0 a size-1

        //trocar as posi??es
        Card temp = deck[i]; //Armazenando em uma variável (tipo carta) local a ser trocada
        deck[i] = deck[swap]; //adicionar à posição analisada uma carta aleatéria (pode ser ela mesma tb)
        deck[swap] = temp; //adicionando a carta retirada de volta ao baralho na posição da outra carta
    }
}


void remove_card(Card *deck, int *size, int index) {
    if (index >= *size || index < 0) {
        printf("índice inválido.\n");
        return;
    }
    int value = 0;
    // Move as cartas após o índice para trás, preenchendo o espaço da carta removida
    for (int i = index; i < (*size - 1); i++) {
        deck[i] = deck[i + 1];
        value +=1;
    }

    (*size)--;
    //printf("%d",value + index);
}


void give_card(Card *deck, int *size_deck, Card *hand, int *num_cards){
    hand[*num_cards] = deck[0];  //adicionar a primeira carta do baralho à mão
    (*num_cards)++; //incrementar o números de cartas na mão
    remove_card(deck, size_deck, 0); //remova a carta do baralho
}


Card *generate_deck(int num_decks) {
    // Inicializar naipes, faces e valores
    char *naipes[4] = {"Paus", "Copas", "Espadas", "Ouros"};
    char *faces[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    const int num_naipes = 4;
    const int num_faces = 13;
    const int cards_per_deck = num_naipes * num_faces;
    const int total_size = cards_per_deck * num_decks;

    // Alocar memória para o baralho
    Card *deck = (Card *)malloc(total_size * sizeof(Card));
    if (deck == NULL) {
        printf("Falha na alocação de memória.\n");
        return NULL;
    }

    //preencher baralho
    for (int k = 0; k < num_decks; k++) {
        for (int i = 0; i < num_naipes; i++) {
            for (int j = 0; j < num_faces; j++) {
                //ponteiro temporário que acessa os elementos do baralho
                Card *current_card = &deck[k * cards_per_deck + i * num_faces + j];

                strcpy(current_card->naipe, naipes[i]);
                strcpy(current_card->face, faces[j]);

                if (strcmp(faces[j], "A") == 0) {
                    current_card->value = 1;
                } else if (strcmp(faces[j], "J") == 0 || strcmp(faces[j], "Q") == 0 || strcmp(faces[j], "K") == 0) {
                    current_card->value = 10;
                } else {
                    current_card->value = atoi(faces[j]);
                }
            }
        }
    }

    return deck;
}

void annul_hand(Player *player){
    //limpar todas as cartas
    for (int i = 0; i < player->num_cards; i++){
        *player->hand[i].face = 0; *player->hand[i].naipe = 0; player->hand[i].value = 0;
    } 
    player->num_cards = 0; // resetando numero de cartas
}

float bet(Player *user, float pre_bid) {
    float aposta;

    if (pre_bid > 0){ //se houver entrada de volor prévio, implica em interesse de aumento
        //double down (2x)
        aposta = 2*pre_bid;
        user->coin -= pre_bid; //retirar da conta do usuario o adicional
        printf("\n\t\tAposta foi dobrada.\n\t\tValor: %.2f coin.\n", aposta);
        return aposta;
    }

    while (1) { // 1 == true
        printf("\nFaça sua aposta (ou um valor negativo, zero para cancelar): ");
        if (scanf("%f", &aposta) != 1 || aposta <= 0 || aposta > user->coin) {
            if (aposta <= 0) {
                printf("Aposta cancelada.\n");
                return 0; // Retorna 0 para indicar aposta cancelada
            } else {
                printf("Aposta inválida. Tente novamente.\n");
                while(getchar() != '\n'); //limpar buffer
                clear_line(); clear_line();//limpar mensagem de erro
            }
        } else {
            user->coin -= aposta;
            printf("Aposta de %.2f realizada. Saldo restante: %.2f\n", aposta, user->coin);
            return aposta; // Retornar a aposta realizada
        }
        
    }
}


int action_player(int key_option){
    /* 5 ações possíveis:
        Hit: pedir mais cartas.
        Stand: Não tirar mais cartas.
        Double down: dobre sua aposta original e pare.
        Split: divida sua mão em duas, caso as duas cartas forem um par idêntico.
        Surrender: retire-se após o crupiê(dealer) distribuir suas cartas iniciais.
    */

    // Mapeamento das ações para strings
    const char *actions[NUM_ACTIONS] = {
        "Hit",           //0
        "Stand",         //1
        "Double down",   //2
        "Split",         //3
        "Mostrar Mesa",  //4
    };

    int choice;
    if (key_option){ //Pensando no momento antes de entrar no jogo
        printf("\nDeseja desistir (Surrender) ou fazer uma aposta?\n");
        printf("1. Surrender (Taxa: 10 coin)\n");
        printf("2. Fazer uma aposta\n");

        while (1){
            printf("Digite o número da opções desejada: ");
            if (scanf("%d", &choice) != 1 || choice < 1 || choice > 2){
                printf("Entrada inválida. Tente novamente.\n");
                clear_line();clear_line();
                while(getchar() != '\n');
                continue;
            }
            
            return (choice == 1) ? 5 : 0; //0 equivalente ao Hit
        }
    }

    while (1) {
        printf("\nEscolha sua ação:\n");
        for (int i = 0; i < NUM_ACTIONS; i++){ //mostrando as possibilidades
            printf("%d. %s\n", i, actions[i]);
        }

        printf("Digite o número da ação desejada: ");

        if (scanf("%d", &choice) != 1){ //O retorno do scanf ? o número de itens bem-sucedidos lidos. (se nn for do mesmo tipo => retorno 0
            printf("Entrada inválida. Tente novamente.\n");
            while(getchar() != '\n'); //limpar buffer
            clear_line(); clear_line();
            continue;
        }

        // Validar a escolha do jogador
        if (choice < 0 || choice >= NUM_ACTIONS){
            printf("Não há tal escolha. Tente novamente. \n");
            while(getchar() != '\n'); //limpar buffer
            clear_line(); clear_line();
            continue;
        }

        return choice;
    }

}

void casino_table(int stage_dealer, Player *dealer, Player *user){ //stage_dealer = 0 => mostrar todas as cartas do delaer
    printf("\t\t******************\n\t\tMESA DO BLACKJACK\n\t\t******************\n\n");
    printf("\t\t\t\t\t\t\tSeu Dinheiro: %.2f\n\t\t\t\t\t\t\tDinheiro da Banca: %.2f\n\n", user->coin, dealer->coin);

    //cartas do dealer
    printf("\tCartas do Dealer:\n\t");
    if (stage_dealer){
        for (int i = 0; i < dealer->num_cards; i++){
            printf("[%s %s]    ", dealer->hand[i].face, dealer->hand[i].naipe);
        }
    }else{
        printf("[XX XX]    "); // Carta oculta do dealer
        printf("[%s %s]    ", dealer->hand[1].face, dealer->hand[1].naipe);
    }

    printf("\n\n\t##############################################\n\n");

    //cartas do jogador
    printf("\tSuas Cartas:\n\t");
    for(int j = 0; j < user->num_cards; j++){
        printf("[%s %s]    ", user->hand[j].face, user->hand[j].naipe);
    }
    printf("\n\n\t##############################################\n\n");

    // Aguardar até que o usuário pressione a tecla espaço
    while(getchar() != '\n'); // limpar possível buffer de teclado
    pausa();
    clear_terminal(); //limpar tela
}


int count_points(Card *hand, int num_cards) {
    int score = 0;
    int num_aces = 0;

    // Calcular a pontuação da mão, contando ?ses e somando outros valores
    for (int i = 0; i < num_cards; i++) {
        if (strcmp(hand[i].face, "A") == 0) {
            num_aces++;
            score += 11; // Considerar o ?s como 11
        } else {
            score += hand[i].value;
        }
    }

    // Ajustar o valor dos ?ses se necess?rio
    while (score > 21 && num_aces > 0) {
        score -= 10; // Reduzir o valor de um ?s de 11 para 1
        num_aces--;
    }

    return score;
}

int compare_scores(int score_player, int num_cards_player, int score_dealer, int num_cards_dealer) {
    if (score_player == 21 && num_cards_player == 2) {
        // Blackjack do jogador
        return 1;
    }
    
    if (score_dealer == 21 && num_cards_dealer == 2) {
        // Blackjack do dealer
        return -1;
    }

    if (score_player > 21) {
        return (score_dealer > 21) ? 0 : -1;
    }
    
    if (score_dealer > 21) {
        return 1;
    }
    
    if (score_player > score_dealer) {
        return 1;
    }
    
    if (score_player < score_dealer) {
        return -1;
    }
    
    if (score_dealer == score_player && score_player != 21) {
        return 0;
    }
    
    return (score_player == 21) ? 1 : -1;
}



int round_21(Player *dealer, Player *user){
    int score_dealer = 0, score_user = 0;
    /*
    - cria-se o baralho
    - embaralha o deck
    - distribui as cartas
    - (função mostrar cartas em jogo)
    - dealer uma aberta e outra fechada
    - loop (enquanto o jogador fazer ação ou passar 21) de ação

    Finalizada a rodada do jogador, segue a ação do dealer:
        - comprar carta. Parar de comprar cartas se for igual ou passar de 16 (ex.de outra.: até 16, o pr?ximo, aleatério para pegar mais uma)
        - compara os valores e retorna: vit?ria, derrota ou empate
        - dado o resultado, retorna a aposta ao jogador, subtraindo do dealer
        - se o dealer chegar a 0 ou menos, retornar para o main: [vit?ria (Quebrou a banca!), derrota (Saiu de mãos abanando!) ((talvez pausa))]
        - ao fim, apaga-se o baralho
    */
    int num_decks = 1;
    int size_deck = 52 * num_decks;
    Card *deck = generate_deck(num_decks); //alocando o ponteiro local da função para o deck global

    shuffle(deck, size_deck); //embaralhar

    //distribuir duas cartas para jogador e, depois, para o dealer
    for (int i = 0; i < 4; i++){
        if (i < 2){
            give_card(deck, &size_deck, user->hand, &user->num_cards);
        }else{
            give_card(deck, &size_deck, dealer->hand, &dealer->num_cards);
        }
    }

    //mostra mesa
    casino_table(0, dealer, user);

    //Ações do jogador ***************************
    int action, first_motion = 1; // ação && condi??o de primeiro acesso (para split e double down)
    action = action_player(1); // Perguntar se desistir?

    if (action == 5){//surrender
            printf("\n\t\t******************\n\t\tPartida Finalizada\n\t\t******************\n\n");
            user->coin -= 10; //jogador paga a taxa
            dealer->coin += 10; //banca ganha a taxa
            return 0;
    }

    //apostar um valor float
    float stake = bet(user, -1);
    if (stake <= 0){
            printf("\n\t\t******************\n\t\tPartida Finalizada\n\t\t******************\nAposta recusada!\n\n");
            return 0;
    }

    while (score_user <= 21){
        clear_terminal(); //limpar prompt
        action = action_player(0);
        if (action == 1){ //stand
            casino_table(0, dealer, user);
            break;
        }
        if (action == 0){//hit
            first_motion = 0; //false -> foi usado primeira ação
            give_card(deck,&size_deck, user->hand, &user->num_cards);
            score_user = count_points(user->hand, user->num_cards);
            casino_table(0, dealer, user);
        }
        if (action == 2 && first_motion){//Double down
            first_motion = 0;
            stake = bet(user,stake); //bet -. valor da aposta x2 (realizado na função)
            give_card(deck,&size_deck, user->hand, &user->num_cards);
            score_user = count_points(user->hand, user->num_cards);
            casino_table(0, dealer, user);
            break;

        }
        if (action == 3 && first_motion){//split  //**** em manutenção ****
            first_motion = 1; //clocar em 0 quando add a funcionalidade
            //bet
            //criar um player local para essa rodada
            //jogar com dois hands
            printf("\n********Em ajuste*******\n");
            continue;

        }

        if (action == 4){//mostrar mesa
            casino_table(0, dealer, user);
        }
    }

    if (user->num_cards == 2 && score_user == 21){
        printf("\n\t\tMr(s).%s fez um BLACKJACK !!\n\n", user->name);
    }

    printf("\n\n\t***************\n\tVez do Dealer!\n\t***************\n\n");

    //A??es do dealer ***************************
    score_dealer = count_points(dealer->hand, dealer->num_cards); //contar previamente com valor ocultado
    while (score_dealer <= 16){
        // Mostrar carta ocultada
        casino_table(1, dealer, user);
        //dealer pega carta
        give_card(deck,&size_deck, dealer->hand, &dealer->num_cards);
        //contar mão (dealer)
        score_dealer = count_points(dealer->hand, dealer->num_cards);
    }

    //Ultima amostragem da mesa (?ltima carta q o dealer pegou)
    casino_table(1, dealer, user);

    // Analisar Resultado da partida *********************************
    int outcome = compare_scores(score_user, user->num_cards, score_dealer, dealer->num_cards);

    if (outcome == 1) {
        printf("\n***************\nMr(s).%s venceu!\n***************\n", user->name);
    } else if (outcome == -1) {
        printf("\n***************\nDealer venceu!\n***************\n");
    } else if(outcome == 0 && score_user == score_dealer && score_user <= 21 ){ //empate = jogador Não estorou e tem mesmo valor do dealer
        printf("\n***************\nEmpate!\n***************\n");
    }else{
        printf("\n***************\nEstorou, Mr(s).%s!\n***************\n", user->name);
    }

    //Retornar resultado da aposta

    //vitoria (1:1); derrota; empate (1:0); caso blackjack vitoria (3:2)
    if (outcome == 1 && user->num_cards == 2 && score_user == 21){ //nature blackjack win
            user->coin += 2.5 * stake; //recebe a aposta de volta + lucro (1.5)
            dealer->coin -= 1.5 * stake; //pagar lucro
    }else if (outcome == 1){ // Win
        user->coin += 2 * stake; //recebe a aposta + seu lucro (1)
        dealer->coin -= stake;//pagar lucro
    }else if (outcome == 0 && score_user <= 21){ //draw
        user->coin += stake; //recebe de volta a aposta
    }else{// lose
        dealer->coin += stake; //dealer ganha a aposta
    }

    printf("\n***************\nSaldo: %.2f.\n***************\n", user->coin);
    printf("\n\t\t******************\n\t\tPartida Finalizada\n\t\t******************\n\n");
    return 0;
}



void rounds(Player *dealer, Player *user){
    int stop_key = 0;

    //While o dealer Não perder e o jogador Não perde  e Não houver stop: retornar func round
    while (user->coin > 0 && dealer->coin > 0 && !stop_key){
        // Resetar mão do dealer e do jogaor sempre que começar nova partida
        annul_hand(user);
        annul_hand(dealer);

        round_21(dealer, user); //sem referenciador de ponteiro, pois j? est? referenciada por rounds
        pausa();
        clear_terminal();

        printf("\n***************\nEsperando próxima partida\n***************\n1. Continuar\n2. Sair\n***************\n");
        while((scanf("%d", &stop_key) != 1) || (stop_key != 1 && stop_key != 2)){
            printf("Entrada inválida. Digite novamente.\n");
            sleep(2);
            // Usa um caractere de escape ANSI para apagar a linha anterior
            clear_line();
        }

        if (stop_key == 2){ 
            //redefinir stop como verdadeiro
            stop_key = 1; 
            return; //sair da função rounds
        }else{
            //no caso 1, s? setar valor de stop_key para 0
            stop_key = 0;
            clear_terminal();
        } 
    }
}

/*
/////////////////////////// Funções descartadas ////////////////////////////////


void remove_card_cls(Card **deck, int *size, int index){
    //criar novo array com o tamanho - 1 (referente a retirada do elemento)
    int new_size = *size - 1;//decremento de size
    Card *new_deck = (Card*)malloc(new_size * sizeof(Card));

    if (new_deck == NULL){
        printf("Falha na alocação de memória.\n");
        exit(1);
    }

    if (index >= *size || index < 0){
        printf("índice inválido.\n");
        return;
    }


    for (int i = 0, j = 0; i < *size; i++){
        if (i == index){ //nesse caso, j Não ser? incrementado, o q implica que Não haver? espaço em branco no novo array
            continue;
        }
        new_deck[j] = *deck[i];
        j++;
    }

    free(*deck); //liberar a memória do deck (array) original

    // Novo tamanho
    *size = new_size;

    // definir o novo array no local do antigo
    *deck = new_deck;

}


void generate_deck_cls(Card *deck, int *size, int num_decks){
    // Inicializar naipes, faces e valores; Obs.: a ?nica face a receber dois valores poss?veis ? o As (1 ou 11) *isso ser? utilizado a frente
    char *naipes[4] = {"Paus","Copas","Espadas","Ouros"};
    char *faces[13] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};

    deck = (Card *)realloc(deck, 52 * num_decks * sizeof(Card)); //realocando o deck para satisfazer o num de baralhos

    for (int k = 0; k < num_decks; k++){
        for (int i = 0; i < 4; i++){ //para cada naipe (4 naipes), 13 faces
                for(int j = 0; j < 13; j++){
                    strcpy(deck[k*52 + i*13 + j].naipe, naipes[i]); //strcpy(*destino, *origem) usado para alocar uma string de um lugar para outro
                    strcpy(deck[k*52 + k*52 + i*13 + j].face, faces[j]);

                    if (strcmp(faces[j],"A") == 0){ //strcmp para comparar strings (retorna 0 se forem identicas)
                        deck[k*52 + i*13 + j].value = 1;
                    }else if(strcmp(faces[j],"J") == 0 ||strcmp(faces[j],"Q") == 0||strcmp(faces[j],"K") == 0){
                        deck[k*52 + i*13 + j].value = 10;
                    }else{
                        deck[k*52 + i*13 + j].value = atoi(faces[j]); //atoi(string) usado para converter de string para valor num?rico
                    }
                }
        }
    }
}

*/

void art_ascii(int key){
    const char* menu_principal =
    " _____________________________________________ \n"
    "|.'',             BLACKJACK               ,''.|\n"
    "|.'.'',                                 ,''.'.|\n"
    "|.'.'.'',        1. NOVO JOGO         ,''.'.'.|\n"
    "|.'.'.'.'',      2. CONTINUAR       ,''.'.'.'.|\n"
    "|.'.'.'.'.|      3. SAIR            |.'.'.'.'.|\n"
    "|.'.'.'.'.|===;                 ;===|.'.'.'.'.|\n"
    "|.'.'.'.'.|:::|',             ,'|:::|.'.'.'.'.|\n"
    "|.'.'.'.'.|---|'.|, _______ ,|.'|---|.'.'.'.'.|\n"
    "|.'.'.'.'.|:::|'.|'|???????|'|.'|:::|.'.'.'.'.|\n"
    "|,',',',',|---|',|'|???????|'|,'|---|,',',',',|\n"
    "|.'.'.'.'.|:::|'.|'|???????|'|.'|:::|.'.'.'.'.|\n"
    "|.'.'.'.'.|---|','   /%%%\\   ','|---|.'.'.'.'.|\n"//
    "|.'.'.'.'.|===:'    /%%%%%\\    ':===|.'.'.'.'.|\n"//
    "|.'.'.'.'.|%%%%%%%%%%%%%%%%%%%%%%%%%|.'.'.'.'.|\n"//
    "|.'.'.'.','       /%%%%%%%%%\\       ','.'.'.'.|\n"
    "|.'.'.','        /%%%%%%%%%%%\\        ','.'.'.|\n"
    "|.'.','         /%%%%%%%%%%%%%\\         ','.'.|\n"
    "|.','          /%%%%%%%%%%%%%%%\\          ','.|\n"
    "|;____________/%%%%%%%%%%%%%%%%%\\____________;|\n";

    const char* inicio =
    ".             .        .     .    |--|--|--|--|--|--|  |===|==|   /    i     \n"
    "        .            _____________|__|__|__|__|__|_ |  |===|==|  *  . /=\    \n"
    "__ *            .  /|_____________________________|-|  |===|==|       |=|  . \n"
    "__|  .      .   . //|_____________________________| :----------------------. \n"
    "__|   /|\\     _|_ //|      ooooooooooooooooooooo  |-|                      | \n"
    "__|  |/||__   ||l|/,|------8 %%%%%%%%%%%%%%%%  8 -| |  1. ENTRAR           | \n"
    "__|._|/||||.l |[=|/,|------8 %   CASSINO    %  8 -|-|  2. SAIR             | \n"
    "__|[+|-|-||||li|[=| |------8 % GOL D. ROGER %  8 -| |                      | \n"
    "_-----.|/| _/:\\_[=|`|------8 %%%%%%%%%%%%%%%%  8 -|-|                      | \n"
    " /|  /||//8/ : \\8| `|------8ooooooooooooooooooo8 -| |                      | \n"
    "/=| //||/ /  .  \\  \\|____________  ____  _________|-|                      | \n"
    "==|//||  /   .   \\ \\|____________ |X|  | _________| `---==------------==---'\n"
    "==| ||  /         \\`|____________ |X|  | _________|     ||            ||    \n"
    "==| |~ /     .     \\                                                          \n"
    "LS|/  /             \\______________________________________________________   \n";

    const char* specify =
"            ,:' `..;       \n"
"            `. ,;;'#       \n"
"            +;;'#####      \n"
"             /- #,)##      \n"
"             `   \\ ##     \n"
"              =  )/ \\     \n"
"              `-'/ / \\    \n"
"               /\\/.-.\\   \n"
"               |  (    |   \n"
"               |  |   ||   \n"
"               |  |   ||   \n"
"           _.-----'   ||   \n"
"          / \\________,'|  \n"
"         (((/  |       |   \n"
"         //    |       |   \n"
"        //     |\\      |  \n"
"       //      |  \\    |  \n"
"      //       |    \\  |  \n"
"     //        |    | \\|  \n"
"    //         \\    \\     \n"
"   //          |\\    \\    \n"
"  o*           |.'\\   \\   \n"
"              _\\   \\.-'\\ \n"
"            (___.-(__.'\\/ \n";


    if (key == 1){printf("%s\n", menu_principal);}else if(key == 0){printf("%s\n", inicio);}else{printf("%s\n", specify);}
}
