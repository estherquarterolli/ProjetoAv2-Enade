/*
        Testar no terminal compartilhado <jefferson>
              ./AV2
------------------------------------------------------------------O que terá no struch
typedef struct {

    int codigo;

    float nota_enade;
    float idd;
    float doutores;
    float mestres;                          //principais dados que ser printado depois.
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
typedef struct {
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
// Função auxiliar para leitura do arquivo do enade
float calcCPCcontinuo(Tcursos curso);
int calcCPCFaixa(float cpc_continuo);
void gerarRelatorio(Tcursos cursos[], int totCursos);
void printarCurso(Tcursos curso); // testar o struch

// MAIN ----------------------------------
int main() {
    // system("chcp 65001"); // Resolve problemas de acentuação no terminal Windows
    // system("cls");

    // declaração de variáveis
    int menu = 0, totcursos = 0;
    Tcursos curso[MAX_CURSOS];

    // lendo o arquivo
    lerArq("EnadeCursos.txt", curso, &totcursos);

    // Bloco para tratar o caso de arquivo vazio na primeira execução
    if (totcursos == 0) {
        while ((menu != 1) && (menu != 2)) { // enquanto não escolher as opções , não vai mudar
            printf("\n\nArquivo vazio!\n\n Deseja criar um curso?\n\n 1-sim\n\n 2-não\n");
            scanf("%d", &menu);

            switch (menu) {
                case 1:
                    addCurso("EnadeCursos.txt", curso, &totcursos);
                    break;
                case 2:
                    return 1; // encerra o programa
                default:
                    printf(" valor invalido");
                    break; // sai do segundo switch
            }
        }
    }

    // menu de navegação após leitura------------------
    do { // enquanto não escolher as opções , não vai mudar
        printf("\n\n<<< <Olá, seja bem vindo(a) ao Enade:>>>>> \n Selecione uma escolha:\n 1-Adicionar curso \n 2-Gerar relatorio \n 0-Sair do programa \n\n");
        scanf("%d", &menu);

        switch (menu) {
            case 0: //sai do programa
                printf("\n\nAté logo");
                break;
            case 1: //adiconar curso
                addCurso("EnadeCursos.txt", curso, &totcursos);
                // totcursos=lerArq("EnadeCursos.txt", curso, &totcursos);
                break;
            case 2: //gerar relatorio
                gerarRelatorio(curso, totcursos);
                break;
            default:
                printf(" valor invalido");
                break;
        }
    } while (menu != 0);

    return 0;
}

/// Implementação das funções:
void printarCurso(Tcursos curso) {
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
int lerArq(char nomeArquivo[], Tcursos curso[], int *totCursos) {
    FILE *arq = fopen(nomeArquivo, "r"); // leitura do arquivo do enade
    *totCursos = 0;
    if (!arq) { // erro de abertura
        return 0;
    }

    // lendo os dados do arquivo e promovendo
    char linha[256]; // vetor que le os arquivos
    int i = 0;
    while (fgets(linha, 256, arq) != NULL) {
        int campo = 0, c = 0, j = 0; // campo prenche cada espaço corretamento no struch
        char temp[50]; // para armazenar cada linha do arquivo separadamente

        for (c = 0; linha[c] != '\0' && linha[c] != '\n'; c++) {
            if (linha[c] != '|') { // enquanto não  encontra a | preenche a string temporaria
                temp[j++] = linha[c];
            } else {
                temp[j] = '\0'; // fecha string
                j = 0; // zera o j para contagem do proximo curso.

                switch (campo) {
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
        // Processa o último campo (num_alunos) que ficou no temp
        temp[j] = '\0';
        curso[i].num_alunos = atoi(temp);
        i++; //pprepara o prox curso
    }
    fclose(arq);
    *totCursos = i;
    return i;
}

int addCurso(char nomeArquivo[], Tcursos curso[], int *totCursos) {
    if (*totCursos >= MAX_CURSOS) {
        printf("Tamanho maximo de cursos atingidos");
        return *totCursos;
    }
    int temp = *totCursos;

    FILE *arq = fopen(nomeArquivo, "a"); // adicionando no o arquivo do enade
    if (!arq) { // erro de abertura
        return 0;
    }

    if (temp > 0) {
        fprintf(arq, "\n"); //se o arquivo não estiver vazio adiciona uma quebra de linha
    }

    printf("Informe o código do curso: ");
    scanf("%d", &curso[temp].codigo);
    fprintf(arq, "%d|", curso[temp].codigo);

    printf("Informe a nota do ENADE: ");
    scanf("%f", &curso[temp].nota_enade);
    fprintf(arq, "%.2f|", curso[temp].nota_enade);

    printf("Informe o IDD: ");
    scanf("%f", &curso[temp].idd);
    fprintf(arq, "%.2f|", curso[temp].idd);

    printf("Informe o percentual de doutores: ");
    scanf("%f", &curso[temp].doutores);
    fprintf(arq, "%.2f|", curso[temp].doutores);

    printf("Informe o percentual de mestres: ");
    scanf("%f", &curso[temp].mestres);
    fprintf(arq, "%.2f|", curso[temp].mestres);

    printf("Informe o regime de trabalho: ");
    scanf("%f", &curso[temp].regime_trabalho);
    fprintf(arq, "%.2f|", curso[temp].regime_trabalho);

    printf("Informe a organização didática pedagógica: ");
    scanf("%f", &curso[temp].organizacao_didatica);
    fprintf(arq, "%.2f|", curso[temp].organizacao_didatica);

    printf("Informe a infraestrutura: ");
    scanf("%f", &curso[temp].infraestrutura);
    fprintf(arq, "%.2f|", curso[temp].infraestrutura);

    printf("Informe as oportunidades acadêmicas/profissionais: ");
    scanf("%f", &curso[temp].oportunidades);
    fprintf(arq, "%.2f|", curso[temp].oportunidades);

    printf("Informe o número de alunos matriculados: ");
    scanf("%d", &curso[temp].num_alunos);
    fprintf(arq, "%d", curso[temp].num_alunos);

    fclose(arq);
    (*totCursos)++;
    printf("\nCurso adicionado!");

    return 1; //retornando 1 para somar com total de cursos
}

/* FUNÇÃO AUXILIAR para calcular o CPC Contínuo de UM curso*/
float calcCPCcontinuo(Tcursos curso) {
    return (curso.nota_enade * 0.20) +
           (curso.idd * 0.35) +
           (curso.doutores * 0.15) +
           (curso.mestres * 0.075) +
           (curso.regime_trabalho * 0.075) +
           (curso.organizacao_didatica * 0.075) +
           (curso.infraestrutura * 0.05) +
           (curso.oportunidades * 0.025);
}

/*FUNÇÃO AUXILIAR para obter a Faixa de um CPC Contínuo*/
int calcCPCFaixa(float cpc_continuo) {
    if (cpc_continuo >= 3.945) return 5;
    if (cpc_continuo >= 2.945) return 4;
    if (cpc_continuo >= 1.945) return 3;
    if (cpc_continuo >= 0.945) return 2;
    return 1;
}

/*FUNÇÃO PARA GERAR O RELATÓRIO COMPLETO */
void gerarRelatorio(Tcursos cursos[], int totCursos) {
    if (totCursos == 0) {
        printf("\nNao ha cursos para gerar relatorio.\n");
        return;
    }

    printf("\n\n--- RELATORIO DE CURSOS ---\n\n");
    for (int i = 0; i < totCursos; i++) {
        //calcula o cpc continuo e faixa
        float cpc_continuo_atual = calcCPCcontinuo(cursos[i]);
        int cpc_faixa_atual = calcCPCFaixa(cpc_continuo_atual);

        printf("Codigo do Curso: %d\n", cursos[i].codigo);
        printf("  - CPC Continuo: %.4f\n", cpc_continuo_atual);
        printf("  - CPC Faixa: %d\n", cpc_faixa_atual);
        if (cpc_faixa_atual < 3) {
            printf("  - Classificacao: Insatisfatorio\n");
        } else {
            printf("  - Classificacao: Satisfatorio\n");
        }
        printf("---------------------------------------\n");
    }

    // --- REQUISITO 2: Cursos por Faixa ---
    printf("\n\n--- CURSOS AGRUPADOS POR FAIXA DE CPC ---\n\n");
    for (int faixa = 5; faixa >= 1; faixa--) {
        printf("Cursos com Faixa %d:\n", faixa);
        int encontrou = 0;
        for (int i = 0; i < totCursos; i++) {
            // Recalcula para cada curso para poder comparar
            float cpc_continuo_atual = calcCPCcontinuo(cursos[i]);
            int cpc_faixa_atual = calcCPCFaixa(cpc_continuo_atual);

            if (cpc_faixa_atual == faixa) {
                printf("Codigo: %d\n", cursos[i].codigo);
                encontrou = 1;
            }
        }
        if (!encontrou) {
            printf("Nenhum curso encontrado nesta faixa.\n");
        }
        printf("\n");
    }

    // --- IGC DA INSTITUICAO ---
    printf("\n\n--- IGC DA INSTITUICAO ---\n\n");
    float somaPonderadaCPC = 0;
    int totalAlunos = 0;

    for (int i = 0; i < totCursos; i++) {
        // Recalcula o CPC contínuo para a média ponderada do IGC
        somaPonderadaCPC += calcCPCcontinuo(cursos[i]) * cursos[i].num_alunos;
        totalAlunos += cursos[i].num_alunos;
    }

    if (totalAlunos > 0) {
        float igcContinuo = somaPonderadaCPC / totalAlunos;
        int igcFaixa = calcCPCFaixa(igcContinuo);

        printf("IGC Continuo: %.4f\n", igcContinuo);
        printf("IGC Faixa: %d\n", igcFaixa);
    } else {
        printf("Nao foi possivel calcular o IGC (nenhum aluno matriculado).\n");
    }
}