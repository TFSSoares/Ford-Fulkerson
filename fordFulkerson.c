#include <stdio.h>
#include <string.h>

#define V 6 // Número de vértices

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

    return maxFlow;
}


int main() {

    /*
        Exemplo de grafo:

              16      13
         (0) ---> (1) ---> (2)
          |         |       |
        10|       12|     14|
          v         v       v
         (3) <--- (4) <--- (5)
             9       7
    */

    int graph[V][V] = {
        {0, 16, 13, 0, 0, 0},
        {0, 0, 10, 12, 0, 0},
        {0, 4, 0, 0, 14, 0},
        {0, 0, 9, 0, 0, 20},
        {0, 0, 0, 7, 0, 4},
        {0, 0, 0, 0, 0, 0}
    };

    int source = 0;
    int sink = 5;

    int maxFlow = fordFulkerson(graph, source, sink);

    printf("Fluxo máximo = %d\n", maxFlow);

    return 0;
}