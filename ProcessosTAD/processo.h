#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_ASSUNTOS 10
#define MAX_PROCESSOS 500 //alterar a quantidade de processos lidos

typedef struct
{
    int id;
    char numero[25];
    char data_ajuizamento[30];
    int id_classe;
    int assuntos[MAX_ASSUNTOS];
    int qtd_assuntos;
    int ano_eleicao;
} Processo;

int lerArquivoCSV(const char *nomeArquivo, Processo processos[], int *total);
void ordenarPorId(Processo processos[], int total);
void ordenarPorData(Processo processos[], int total);
int contarPorClasse(Processo processos[], int total, int classe);
int contarAssuntosUnicos(Processo processos[], int total);
void listarProcessosComVariosAssuntos(Processo processos[], int total);
int calcularDiasTramitando(const char *data);
void salvarCSV(const char *nomeArquivo, Processo processos[], int total);

#endif
