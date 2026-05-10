#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getNumberOfVertices();
void buildGraph(int vertices, int graph[vertices][vertices]);
int *splitToIntArray(char str[], int *size);
void fillMatrixWithZero(int vertices, int graph[vertices][vertices]);
void showMatrix(int vertices, int graph[vertices][vertices]);

int main()
{
    int vertices = getNumberOfVertices();
    int graph[vertices][vertices];

    fillMatrixWithZero(vertices, graph);
    buildGraph(vertices, graph);

    showMatrix(vertices, graph);

    return 1;
}

int getNumberOfVertices()
{
    FILE *arquivo = fopen("grafo.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        return 0;
    }

    char linha[100];

    fgets(linha, sizeof(linha), arquivo);

    fclose(arquivo);
    return atoi(linha);
}

void fillMatrixWithZero(int vertices, int graph[vertices][vertices])
{
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            graph[i][j] = 0;
        }
    }
}

void buildGraph(int vertices, int graph[vertices][vertices])
{
    FILE *arquivo = fopen("grafo.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    char linha[100];

    int size;
    int controller = 1;

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        if (controller >= 5)
        {

            int *arr = splitToIntArray(linha, &size);
            graph[arr[0] - 1][arr[1] - 1] = arr[2];
        }

        controller++;
    }

    fclose(arquivo);
}

int *splitToIntArray(char str[], int *size)
{
    int *arr = malloc(100 * sizeof(int));

    *size = 0;

    char *token = strtok(str, " ");

    while (token != NULL)
    {
        arr[*size] = atoi(token);

        (*size)++;

        token = strtok(NULL, " ");
    }

    return arr;
}

void showMatrix(int vertices, int graph[vertices][vertices])
{
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }
}