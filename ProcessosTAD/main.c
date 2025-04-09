#include <stdio.h>
#include "processo.h"

int main()
{

    Processo processos[MAX_PROCESSOS];
    int total = 0;

    if (!lerArquivoCSV("processo_043_202409032338.csv", processos, &total))
    {
        return 1;
    }

    //contador de processos
    printf("\nTotal de processos lidos: %d (caso queira, e possivel alterar a quantidade de processos lidos no codigo)\n", total);

    //id por ordem crescente
    printf("\nID por ordem crescente\n");
    ordenarPorId(processos, total);
    salvarCSV("ordenado_por_id.csv", processos, total);

    //data por ordem descrescente
    printf("\nData de ajuizamento por ordem decrescente\n");
    ordenarPorData(processos, total);
    salvarCSV("ordenado_por_data.csv", processos, total);

    //contador de classe
    int classeBuscada = 12553; // alterar a classe do processo
    int totalClasse = contarPorClasse(processos, total, classeBuscada);
    printf("\nTotal de processos com id_classe %d: %d (caso queira, e possivel alterar a classe desejada no codigo) \n", classeBuscada, totalClasse);

    //contador de processos na base de dados
    int assuntosUnicos = contarAssuntosUnicos(processos, total);
    printf("\nTotal de processos presentes na base de dados: %d\n", assuntosUnicos);

    //contador de processos com mais de um assunto
    printf("\nProcessos com mais de um assunto: \n");
    listarProcessosComVariosAssuntos(processos, total);

    //contador de quantidade de dias em tramitação
    printf("\nDias em tramitacao de cada processo:\n");
    for (int i = 0; i < total; i++)
    {
        int dias = calcularDiasTramitando(processos[i].data_ajuizamento);
        printf("Processo %s - %d dias\n", processos[i].numero, dias);
    }

    return 0;
}
