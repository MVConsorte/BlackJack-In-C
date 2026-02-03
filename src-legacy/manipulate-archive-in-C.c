#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

void file_write(void); //Primeira aula = file*, fprintf, fopen, fclose
int file_read(void); //segunda aula = fcanf, "r"
void file_fgets(void); //3º aula = fgets (file get string)
void file_fput(void); //4º aula = fputs, fputc
void file_copyMetter(void); //5º aula = troca de informações entre arquivos
void file_stdin_fropen(void); //6º aula = stdin, freopen (arquivo apontar para outro)
void file_getc_EOF(void); //7º aula = fgetc e getc, EOF

//////////////////////////////////////////////////////////////////////

int main(void){
    setlocale(LC_ALL, "portuguese"); //definindo para portugues

    

    printf("\n");
    return 0;
}
////////////////////////////// Funções externas /////////////////////

void copyMatter(FILE *file_read, FILE *file_write){
        char leitor[1000];

        while (fgets(leitor, 1000, file_read) != NULL){ //lendo e armazenando em "leitor"
            fputs(leitor, file_write); //anexando a string ao arquivo
        } 
        // OBS.: caso não coloque {} no while => só pegará a última linha do arquivo lido
    }

//////////////////////////////// Aulas ///////////////////////////////

/*
    MACRO:
     Em C, uma macro é uma construção que permite que voce defina um identificador para representar um valor, uma expressão
     ou uma sequência de código.

     Geralmente, são definidas utilizando a diretiva: #define.

     Quando código fonte é compilado, o pré-processador substitui todas as ocorrências da macro pelo seu conteudo antes de compilar.

*/

void file_write(void){
    FILE *file = NULL; //ponteiro para tipo FILE

    //ponteiro apontando à um arquivo no programa
    /* 
        fopen("ENDEREÇO - LOCALIZAÇÃO", "(r || w || a || ba || br || bw)");
        r = ler         br = binário ler             OBS.: adicionar '+' a frente do tipo, permite outras manipulações.
        w = escrever    bw = binário excrever        ex.: r+ permite tb w+  .
        a = alterar     ba = binário alterar

        Na parte de enderço: perceba que vc deve sempre colocar "\\" para tratar como "\"
        Obs.: Caso adicione somente o nome do arquivo, ele criará no diretório em que o programa está.
    */
    file = fopen("teste.txt", "w"); // OBS.: se o arquivo já existir, "w" (substituirá arquivo) 

    // file print format = fprintf(FILE "ponteiro", "Texto a escrever");
    fprintf(file, "This is a fucking JOJO reference!");

    //  file close = fclose(File "ponteiro"); -> fechar o arquivo | por segurança, sempre fechar
    fclose(file);
}

int file_read(void){
    FILE *file;
    file = fopen("teste.txt", "r"); // "r" = ler arquivo

    // Deve-se considerar o caso de erro no nome do arquivo (apontando a lugar nenhum)
    if (file == NULL){
        printf("Arquivo não pode ser aberto\n");
        return 0;
    }

    int dia, mes, ano;

    // file scanner format = fscanf(FILE "ponteiro", scanf)
    // OBS.: caso o arquivo a ser lido não exista, fscanf não será executado
    // Limitação do fscanf: requer formatação perfeita -> limitado à formatação 
    // é interessante para o caso de haver controle total dos dados
    fscanf(file, "%d %d %d", &dia, &mes, &ano);  
    printf("%d %d %d\n", dia, mes, ano); //observar leitura

    fclose(file); //fechar arquivo
    }


void file_fgets(void){
    FILE *file;
    file = fopen("teste.txt","r");

    if (file == NULL){ //controle de arquivo
        printf("Não foi possível abrir o arquivo");
        getchar();
        exit(0);
    }

    char frase[150];

    // file get string = fgets(ARRAY, TAM_ARRAY, file*)
    // OBS.: fgets só lê uma linha por vez dentro do arquivo (até encontrar "\n")
    // limitação fgets: geralmente necessita de código auxiliar para conversões e formatações
    while (fgets(frase, 100, file) != NULL){
        printf("%s", frase);
    }

    fclose(file);
}

void file_fput(void){
    FILE *file;
    file = fopen("teste.txt", "a"); // "a" = append (anexar ao final do arquivo)

    if (file == NULL){
        printf("Arquivo não pode ser aberto.\n");
        getchar();
        exit(1);
    }

    fprintf(file, "\nUntil the World Goes Cold\n");

    // Repassar um vetor de characteres && character = 3
    char frase[] = "The Darkest Dungeon.\n", caracter = 'h';

    // file put string = fputs(frase, FILE*)
    fputs(frase, file);

    // file put character = fputc(char, File*)
    fputc(caracter, file); // ele desloca o ponteiro pós anexação

    fclose(file);
}

void file_copyMetter(void){
    void copyMatter(FILE *file_read, FILE *file_write);

    FILE *file1 = fopen("teste.txt","r");

    FILE *file2 = fopen("numero.txt","w");

    if (file1 == NULL || file2 == NULL){
        printf("Não foi possível abrir o arquivo.\n");
        return 1;
    }
    
    copyMatter(file1, file2);

    fclose(file1); fclose(file2);
}

void file_stdin_fropen(void){
    int range = 100;
    char frase[range];

    /* Parte 1

    // standard in (entrda padrão) = stdin (ele busca dados da entrada base do aparelho) -> PC = teclado
    fgets(frase, range, stdin);
    // OBS.: por padrão, fgets adiciona um elemento nulo ("\0") na string ao final => range - 1
    printf("%s", frase);

    // Permite redirecionar stdin para teste.txt = anexar a primeira linha do teste.txt para stdin
    freopen("teste.txt", "r", stdin);
    fgets(frase, range, stdin); 
    printf("%s", frase);

    */

    FILE *file = fopen("teste.txt","r");

    fgets(frase, range, file);
    printf("%s\n", frase);

    freopen("teste2.txt", "r", file);

    fgets(frase, range, file);
    printf("%s\n", frase);
}

void file_getc_EOF(void){
        /*
            Originalmente, getc foi definido como uma macro (#define getc)    
            Posteriormente, foi implementado a função de getc = fgetc().
            Ou seja, getc é uma macro ou função, já fgetc() é sempre uma função.
        */
        
    char y[100];

        /*
        // anexar dos dados da entrada padrão, o primeiro elemento ao array y, na posição 0
        y[0] = fgetc(stdin); // stdin -> dados da entrada padrão // nesse caso, fgetc == getc

        // Portanto, fgetc() retorna o primeiro caracter do parentesis
        printf("%c", y[0]);
        */

    FILE *file = fopen("teste2.txt","r");

    /* PARTE 1 

    int i = 0;
    while ((y[i] = fgetc(stdin)) != '\n'){
        ++i; //Ler a anexar cada string para uma posição adiante
    } 
    y[++i] = '\0';  //informando caracter nulo ao final da leitura => pausar (e sobrescreverá '\n' no  caso y[i])

    printf("%s", y);

    */

   /*
        EOF (END OF FILE) -> final de arquivo
        utilizado como critério quando a leitura é propria do algorítimo
   */

    int i = 0;
    while ((y[i] = getc(file)) != EOF){ //getc() pode ser executada como macro ou função
    ++i; //Ler a anexar cada string para uma posição adiante
    } 
    y[i] = '\0';  //informando caracter nulo ao final da leitura => pausar e sobrescreverá EOF (sobrescreverá o cachê) 

    printf("%s", y);
}