/*
        Testar no terminal compartilhado <jefferson>
             ./AV2
------------------------------------------------------------------O que terá no struch
typedef struct {

    int codigo;

    float nota_enade;
    float idd;
    float doutores;
    float mestres;                  //principais dados que ser printado depois.
    float regime_trabalho;
    float organizacao_didatica;
    float infraestrutura;
    float oportunidades;

    int num_alunos;

    float cpc_continuo;  // calculado depois
    int cpc_faixa;      // calculado depois

} Tcurso;

--------------------------------------------------------------Funções que serão utilizadas
 Calcular CPC CONTINUO (média ponderada) - função int

 Converter para CPC FAIXA - função int ()

 Ler os dados do arquivo e passar para o vetor de struct - função int (ARQUIVO READ)

 Usuario cadastra novo curso




Existe limite de curso????






 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CURSOS 10

// STRUTCH -------------------------------
typedef struct
{

    int codigo;

    float nota_enade;
    float idd;
    float doutores;
    float mestres; // principais dados que ser printado depois.
    float regime_trabalho;
    float organizacao_didatica;
    float infraestrutura;
    float oportunidades;

    int num_alunos;

    

} Tcursos;
/*Perguntas para o Leo:----------------------------------------

- é necessario criar um arquivo caso ele não exista inicialmente? 
   SIM

- Há necessidade de criar funções específicas ou trabalhar de forma mais abrangente?
 (Isso economiza memória, faz ser mais rápido??)

 - desafio até então passo a passo do algoritmo
   1  . ler o arquivo
   Ao ler o arquivo deve transformar ele em inteiro e depois float para novamente inteiro.
   o passo a passo para transformar outros tipos são bem complexos
   e alguns que não podemos utilizar "strok"
   isso nos atrapalhou e nao sabemos por onde começar basicamente,
   se é necessario uma função para leitura ou se é necessario
   abrir ela diretamente na main.


   2. é necessario , salvar nas informações os valores de cpc continuo e faixa no struch?

*/

// FUNÇÕES CABEÇALHO----------------------------------
// ler e converte a string do arquivo para o vetor de struch.
int lerArq(char nomeArquivo[], Tcursos curso[], int *totCursos);
// Adiciona um curso novo manualmente e passa para o arquivo original (gerar arquivo temporario)
int addCurso(char nomeArquivo[], Tcursos curso[], int *totCursos);
// Calcula o CPC continuo e CPC Faixa e mostra os resultados
//int gerarRel(char nomeArquivo[], Tcursos curso[], int *totCursos);

// Função auxiliar para leitura do arquivo do enade
int cpcCONT(int cpc_FAIXA);
int igc(char nomeArquivo[], Tcursos curso[], int *totCursos);

void printarCurso(Tcursos curso); // testar o struch

// MAIN ----------------------------------
int main()
{

    // declaração de variáveis
    int resultado, menu = 0, totcursos = 0, count = 0;
    Tcursos curso[MAX_CURSOS];

    // lendo o arquivo
    resultado = lerArq("EnadeCursos.txt", curso, &totcursos);

    // testando se o vetor foi preenchido corretamente
    while (count < totcursos)
    {
        // Imprime o primeiro curso lido
        printarCurso(curso[count]);
        count++;
    }
    // exibindo o resultado
    switch (resultado)
    {
    case -1:
        printf("\n\nError!\n Arquivo não encontrado.");
        break;

    case 0: // arquivo vazio e pergunta de adicionar curso

        while ((menu != 1) && (menu != 2)){ // enquanto não escolher as opções , não vai mudar
            printf("\n\nArquivo vazio!\n\n Deseja criar um curso?\n\n 1-sim\n\n 2-não");
            scanf("%d", &menu);
            
            switch (menu){
            case 1:
                totcursos +=addCurso("EnadeCursos.txt", curso, &(totcursos)-1);
                break;
            case 2:

                return 1;
                break; // encerra o programa

            default:
                printf(" valor invalido");

                break; // sai do segundo switch
            }
        }
        break; // sai do primeiro switch
    };

   
        // menu de navegação após leitura------------------

        menu=1;

         while ((menu!=0)){ // enquanto não escolher as opções , não vai mudar
            printf("\n\n<<< <Olá, seja bem vindo(a) ao Enade:>>>>> \n Selecione uma escolha:\n 1-Adicionar curso \n 2-Gerar relatorio \n0- Sair do programa \n\n");
            scanf("%d", &menu);
            
            switch (menu){
                case 0: //sai do programa
                printf("\n\nAté logo");
                break; 
                case 1: //adiconar curso
                totcursos=addCurso("EnadeCursos.txt", curso, &totcursos);
                resultado=lerArq("EnadeCursos.txt", curso, &totcursos);

                break;
                
                case 2:
                count=0;
                 while (count < totcursos)
                 {
                 // Imprime o primeiro curso lido
                    printarCurso(curso[count]);
                     count++;
                 }
                break;
            /*
                case 2: //gerar relatorio

               resultado=gerarRel("EnadeCursos.txt", curso, &totcursos);
                break; // encerra o programa
*/
            default:

                printf(" valor invalido");
                break; 
            }
        }
        
           

    return 0;
}


/// Implementação das funções:
void printarCurso(Tcursos curso)
{
    printf("Código do curso: %d\n", curso.codigo);
    printf("Nota do enade%.2f\n", curso.nota_enade);
    printf("idd: %.2f\n", curso.idd);
    printf("Percentual de doutores: %.2f\n", curso.doutores);
    printf("Percentual de mestres: %.2f\n", curso.mestres);
    printf("Regime de trabalho: %.2f\n", curso.regime_trabalho);
    printf("Organização didática pedagógica: %.2f\n", curso.organizacao_didatica);
    printf("Infraestrutura: %.2f\n", curso.infraestrutura);
    printf("Oportunidades acadêmicas//profissionais: %.2f\n", curso.oportunidades);
    printf("alunos matriculados: %d\n", curso.num_alunos);
    // printf("CPC contínuo: %.2f\n", curso.cpc_continuo);
    // printf("CPC faixa: %d\n", curso.cpc_faixa);
    printf("---------------------------------------\n\n"); // separar os cursos
}

// ler cursos no arquivocodigo
int lerArq(char nomeArquivo[], Tcursos curso[], int *totCursos)
{ //----------------------------------

    FILE *arq = fopen(nomeArquivo, "r"); // leitura do arquivo do enade
    *totCursos = 0;
    if (!arq)
    { // erro de abertura
        return -1;
    }

    // lendo os dados do arquivo e promovendo
    char linha[256]; // vetor que le os arquivos
    int i = 0;
    while (fgets(linha, 256, arq) != NULL){

        int campo = 0, c = 0, j = 0; // campo prenche cada espaço corretamento no struch

        char temp[30]; // para armazenar cada linha do arquivo separadamente

        for (/*c=0 */; linha[c] != '\0' && linha[c] != '\n'; c++)
        {
            if (linha[c] != '|')
            { // enquanto não  encontra a | preenche a string temporaria
                temp[j++] = linha[c];
            }
            else
            {
                temp[j] = '\0'; // fecha string

                j = 0; // zera o j para contagem do proximo curso.

                switch (campo)
                {
                case 0:
                    curso[i].codigo = atoi(temp);
                    break;
                case 1:
                    curso[i].nota_enade = atof(temp);
                    break;
                case 2:
                    curso[i].idd = atof(temp);
                    break;
                case 3:
                    curso[i].doutores = atof(temp);
                    break;
                case 4:
                    curso[i].mestres = atof(temp);
                    break;
                case 5:
                    curso[i].regime_trabalho = atof(temp);
                    break;
                case 6:
                    curso[i].organizacao_didatica = atof(temp);
                    break;
                case 7:
                    curso[i].infraestrutura = atof(temp);
                    break;
                case 8:
                    curso[i].oportunidades = atof(temp);
                    break;
                }

                campo++;
            }
        }

        // Último campo (num_alunos), após o último '|'
        temp[j] = '\0';
        curso[i].num_alunos = atoi(temp);

        i++; // próximo curso
        *totCursos = i;
    }

    fclose(arq);
    return *totCursos;

} // --------------------------------------------------------------------------------------

int addCurso(char nomeArquivo[], Tcursos curso[], int *totCursos) {
    if (*totCursos>=MAX_CURSOS){
        printf("Tamanho maximo de cursos atingidos");
        return *totCursos; 
    }
    int temp=1+*totCursos;

    FILE *arq = fopen(nomeArquivo, "a"); // adicionando no o arquivo do enade

        if (!arq) { // erro de abertura
            return 0;
    }
    

    printf("Informe o código do curso: ");
    scanf("%d", &curso[temp].codigo);
    fprintf(arq,"\n%d|",curso[temp].codigo);

    printf("Informe a nota do ENADE: ");
    scanf("%f", &curso[temp].nota_enade);
    fprintf(arq,"%.2f|",curso[temp].nota_enade);

    printf("Informe o IDD: ");
    scanf("%f", &curso[temp].idd);
    fprintf(arq,"%.2f|",curso[temp].idd);

    printf("Informe o percentual de doutores: ");
    scanf("%f", &curso[temp].doutores);
    fprintf(arq,"%.2f|",curso[temp].doutores);

    printf("Informe o percentual de mestres: ");
    scanf("%f", &curso[temp].mestres);
    fprintf(arq,"%.2f|",curso[temp].mestres);

    printf("Informe o regime de trabalho: ");
    scanf("%f", &curso[temp].regime_trabalho);
    fprintf(arq,"%.2f|",curso[temp].regime_trabalho);

    printf("Informe a organização didática pedagógica: ");
    scanf("%f", &curso[temp].organizacao_didatica);
    fprintf(arq,"%.2f|",curso[temp].organizacao_didatica);

    printf("Informe a infraestrutura");
    scanf("%f", &curso[temp].infraestrutura);
    fprintf(arq,"%.2f|",curso[temp].infraestrutura);

    printf("Informe as oportunidades acadêmicas/profissionais: ");
    scanf("%f", &curso[temp].oportunidades);
    fprintf(arq,"%.2f|",curso[temp].oportunidades);

    printf("Informe o número de alunos matriculados: ");
    scanf("%d", &curso[temp].num_alunos);
    fprintf(arq,"%i",curso[temp].num_alunos);



    fclose(arq);
    return 1; //retornando 1 para somar com total de cursos
}

// int gerarRel(char nomeArquivo[], Tcursos curso[], int *totCursos) {
//     printf("Função gerarRel ainda não implementada!\n");
//     return 0;
// }
