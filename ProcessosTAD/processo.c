#include <stdio.h>
#include <stdlib.h> //alocar memoria
#include <string.h> //manipular as strings
#include <time.h>   //calcular o tempo
#include "processo.h"

void limparAspasDuplas(char *linha)
{
    int i = 0, j = 0;
    while (linha[i])
    {
        if (linha[i] == '"' && linha[i + 1] == '"')
        {
            linha[j++] = '"';
            i += 2;
        }
        else
        {
            linha[j++] = linha[i++];
        }
    }
    linha[j] = '\0';
}

int lerCSV(const char *nomeArquivo, Processo processos[], int *total)
{
    FILE *arquivo = fopen(nomeArquivo, "r");

    char linha[512];
    fgets(linha, sizeof(linha), arquivo);

    int i = 0;
    while (fgets(linha, sizeof(linha), arquivo) && i < MAX_PROCESSOS)
    {
        limparAspasDuplas(linha);

        char id[20], numero[30], data[30], classe[15], assuntos[100], ano[10];
        int lidos = sscanf(linha, "%[^,],\"%[^\"]\",%[^,],{%[^}]},{%[^}]},%s",
                           id, numero, data, classe, assuntos, ano);

        if (lidos != 6)
        {
            continue;
        }

        processos[i].id = atoi(id);
        strcpy(processos[i].numero, numero);
        strcpy(processos[i].data_ajuizamento, data);
        processos[i].id_classe = atoi(classe);
        processos[i].ano_eleicao = atoi(ano);
        processos[i].qtd_assuntos = 0;

        char *token = strtok(assuntos, ",");
        while (token && processos[i].qtd_assuntos < MAX_ASSUNTOS)
        {
            processos[i].assuntos[processos[i].qtd_assuntos++] = atoi(token);
            token = strtok(NULL, ",");
        }

        i++;
    }

    fclose(arquivo);
    *total = i;
    return 1;
}

void ordemId(Processo processos[], int total)
{
    int i, j;
    for (i = 0; i < total - 1; i++)
    {
        for (j = i + 1; j < total; j++)
        {
            if (processos[i].id > processos[j].id)
            {
                Processo temp = processos[i];
                processos[i] = processos[j];
                processos[j] = temp;
            }
        }
    }
}

time_t converterData(const char *data)
{
    struct tm t = {0};
    sscanf(data, "%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday);
    t.tm_year -= 1900;
    t.tm_mon -= 1;
    return mktime(&t);
}

void ordemData(Processo processos[], int total)
{
    int i, j;
    for (i = 0; i < total - 1; i++)
    {
        for (j = i + 1; j < total; j++)
        {
            if (converterData(processos[i].data_ajuizamento) < converterData(processos[j].data_ajuizamento))
            {
                Processo temp = processos[i];
                processos[i] = processos[j];
                processos[j] = temp;
            }
        }
    }
}

int contadorClasse(Processo processos[], int total, int classe)
{
    int cont = 0;
    for (int i = 0; i < total; i++)
    {
        if (processos[i].id_classe == classe)
        {
            cont++;
        }
    }
    return cont;
}

int contarAssuntosUnicos(Processo processos[], int total)
{
    int unicos[1000];
    int total_unicos = 0;

    for (int i = 0; i < total; i++)
    {
        for (int j = 0; j < processos[i].qtd_assuntos; j++)
        {
            int atual = processos[i].assuntos[j];
            int encontrado = 0;
            for (int k = 0; k < total_unicos; k++)
            {
                if (unicos[k] == atual)
                {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado)
            {
                unicos[total_unicos++] = atual;
            }
        }
    }

    return total_unicos;
}

void variosAssuntos(Processo processos[], int total)
{
    for (int i = 0; i < total; i++)
    {
        if (processos[i].qtd_assuntos > 1)
        {
        }
    }
}

int calcularTramitacao(const char *data)
{
    time_t hoje = time(NULL);
    time_t data_processo = converterData(data);
    return (int)((hoje - data_processo) / (60 * 60 * 24));
}

void salvarCSV(const char *nomeArquivo, Processo processos[], int total)
{
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo)
    {
        return;
    }

    fprintf(arquivo, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

    for (int i = 0; i < total; i++)
    {
        fprintf(arquivo, "%d,\"%s\",%s,{%d},{",
                processos[i].id,
                processos[i].numero,
                processos[i].data_ajuizamento,
                processos[i].id_classe);

        for (int j = 0; j < processos[i].qtd_assuntos; j++)
        {
            fprintf(arquivo, "%d", processos[i].assuntos[j]);
            if (j < processos[i].qtd_assuntos - 1)
            {
                fprintf(arquivo, ",");
            }
        }

        fprintf(arquivo, "},%d\n", processos[i].ano_eleicao);
    }

    fclose(arquivo);
    printf("Arquivo %s foi criado.\n", nomeArquivo);
}
