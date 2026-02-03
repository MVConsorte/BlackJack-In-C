#ifndef BLACKJACK_H
#define BLACKJACK_H

/* Vantagens de usar enum, define e const:

   enum:
   - Tipo Seguro: enums são tratados como tipos distintos em C, o que ajuda a evitar erros de tipo.
   - Melhor para Depuração: os valores de enum podem ser mais fáceis de identificar durante a depuração 
   porque o depurador pode mostrar o nome simbólico.
   - Agrupamento Lógico: enums permitem agrupar constantes relacionadas, o que pode tornar o código mais 
   organizado e legível.

   #define:
   - Simplicidade: #define é simples de usar e direto para substituição de texto.
   - Flexibilidade: #define pode ser usado para criar macros complexas, não apenas para constantes.
   - Escopo Global: #define cria símbolos com escopo global, o que pode ser útil em alguns casos.

   const:
   - Verificação de Tipo: const proporciona verificação de tipo, o que ajuda a prevenir erros.
   - Escopo Limitado: const tem escopo limitado, o que reduz a possibilidade de conflitos de nomes.
   - Semântica Clara: const expressa claramente a intenção de que o valor não deve ser modificado.

*/


// Enumeração para constantes do jogo de Blackjack
enum BlackjackConstants {
    NUM_ACTIONS = 5, // Número de ações possíveis na função action_player()
    MAX_NAIPE_SIZE = 10, // Tamanho máximo para o array naipe (string do naipe)
    MAX_FACE_SIZE = 3, // Tamanho máximo para o array face (string da faceta)
    MAX_NAME_SIZE = 50, // Tamanho máximo para o nome do jogador
    MAX_HAND_SIZE = 12 // Número máximo de cartas na mão do jogador
};

struct Card {
    char naipe[MAX_NAIPE_SIZE];
    char face[MAX_FACE_SIZE];
    int value;
};
typedef struct Card Card;

struct Player {
    char name[MAX_NAME_SIZE]; // Nome do jogador
    float coin;               // Dinheiro do jogador
    Card hand[MAX_HAND_SIZE]; // Mão do jogador
    int num_cards;            // Número de cartas do jogador
};
typedef struct Player Player;


// Declaração das funções /////////////////////////////////////

Card *generate_deck(int num_decks);  //função que gera o baralho (ou baralhos) -> retorna um pointeiro Card Deck

void shuffle(Card *deck, int size);  //função que embaralha

void remove_card(Card *deck, int *size, int index); //função que remove uma carta

void give_card(Card *deck, int *size_deck, Card *hand, int *num_cards);  //função que fornece uma carta

void annul_hand(Player *player); //função que reseta a mão do user (player ou dealer)

float bet(Player *user, float pre_bid);  //função que retorna um valor de aposta, além de atualizar conta do usuário #contém dialogo

int action_player(int key_option); //função que retorna a ação escolhida pelo usuário

void casino_table(int stage_dealer, Player *dealer, Player *user);  //função que mostra a mesa atual de Blackjack

int count_points(Card *hand, int num_cards); //função que conta pontuação de uma certa mão

int compare_scores(int score_player, int num_cards_player, int score_dealer, int num_cards_dealer); //função que analisa as pontuações e retorne o resultado (0: empate, -1: derrota, 1:vitória do player)

int round_21(Player *dealer, Player *user); // função que executa uma rodada

void pausa(); //função de pausa

void clear_terminal(); //função que limpa o terminal

void clear_line(); //função que limpa a linha anterior do terminal

void rounds(Player *dealer, Player *user); //sistema para loop de rodadas

void art_ascii(int key);

void blackJack_game(Player *banca, Player *jogador); //função propriamente do jogo (acopla as demais estruturas)

#endif // BLACKJACK_H