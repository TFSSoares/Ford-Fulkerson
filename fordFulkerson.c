#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int V;

void showVerticeMaxFlow(int graph[V][V], int residualGraph[V][V], int source, int sink)
{
    printf("\n\n-----------------------------------------------------\n");
    printf("Valor passado por cada vertice  fonte: %d, escoadouro: %d\n", source, sink);

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            int fluxo = graph[i][j] - residualGraph[i][j];

            if (fluxo > 0)
            {
                printf("%d -> %d : %d\n", i + 1, j + 1, fluxo);
            }
        }
    }
}

void showResidualGraph(int V, int residualGraph[V][V])
{
    printf("\nGrafo residual\n");
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            printf("%d ", residualGraph[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Função para encontrar caminho aumentante usando BFS
int bfs(int residualGraph[V][V], int source, int sink, int parent[])
{
    int visited[V];
    memset(visited, 0, sizeof(visited));

    int queue[V];
    int front = 0, rear = 0;

    queue[rear++] = source;
    visited[source] = 1;
    parent[source] = -1;

    while (front < rear)
    {
        int u = queue[front++];

        for (int v = 0; v < V; v++)
        {
            if (!visited[v] && residualGraph[u][v] > 0)
            {
                queue[rear++] = v;
                parent[v] = u;
                visited[v] = 1;

                if (v == sink)
                    return 1;
            }
        }
    }

    return 0;
}

// Algoritmo Ford-Fulkerson
int fordFulkerson(int graph[V][V], int source, int sink)
{
    int u, v;

    // Grafo residual
    int residualGraph[V][V];

    for (u = 0; u < V; u++)
    {
        for (v = 0; v < V; v++)
        {
            residualGraph[u][v] = graph[u][v];
        }
    }

    int parent[V];
    int maxFlow = 0;

    // Enquanto existir caminho aumentante
    while (bfs(residualGraph, source, sink, parent))
    {

        // Encontra o menor valor residual no caminho
        int pathFlow = 999999;

        for (v = sink; v != source; v = parent[v])
        {
            u = parent[v];

            if (residualGraph[u][v] < pathFlow)
            {
                pathFlow = residualGraph[u][v];
            }
        }

        // Atualiza capacidades residuais
        for (v = sink; v != source; v = parent[v])
        {
            u = parent[v];

            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    showVerticeMaxFlow(graph, residualGraph, source + 1, sink + 1);
    showResidualGraph(V, residualGraph);

    return maxFlow;
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

void buildGraph(int vertices, int graph[vertices][vertices], int **source, int **sink)

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
        if (controller == 3)
            *source = splitToIntArray(linha, &size);
        else if (controller == 4)
            *sink = splitToIntArray(linha, &size);
        else if (controller >= 5)
        {

            int *arr = splitToIntArray(linha, &size);
            graph[arr[0] - 1][arr[1] - 1] = arr[2];
        }

        controller++;
    }

    fclose(arquivo);
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
    printf("\n");
}

void getSourceSinkPair(int source[], int sink[])
{
    for (int i = 0; i < source[0]; i++)
    {
        for (int j = 0; j < sink[0]; j++)
        {
            printf("(%d, %d)\n", source[i + 1], sink[j + 1]);
        }
    }
}

int main()
{
    V = getNumberOfVertices();

    int graph[V][V];

    int *source;
    int *sink;

    fillMatrixWithZero(V, graph);
    buildGraph(V, graph, &source, &sink);

    printf("Grafo:\n");
    showMatrix(V, graph);

    printf("Pares fonte e escoadouro\n");
    getSourceSinkPair(source, sink);

    for (int i = 0; i < source[0]; i++)
    {
        for (int j = 0; j < sink[0]; j++)
        {
            int maxFlow = fordFulkerson(graph, source[i + 1] - 1, sink[j + 1] - 1);
            printf("Fluxo maximo = %d\n", maxFlow);
        }
    }

    return 0;
}