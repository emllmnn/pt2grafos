#include <stdio.h>
#include <stdbool.h>
#include <float.h>
#include <stdlib.h>
#include <malloc.h>

int no_vertices;

struct nodeD {
    int vert;             
    float *arestas;     
};

struct node {
    int vertex;             
    struct node *prox;
};

struct Graph{
    int numVertices;
    struct node **adjLists;
};

struct node *createNode(int v){
    struct node *newNode = malloc(sizeof(struct node));
    newNode-> vertex = v;
    newNode-> prox = NULL;
    return newNode;
}

struct Graph *createAGraph(int vertices){
    struct Graph *graph = malloc(sizeof(struct Graph));
    graph-> numVertices = vertices;
    graph-> adjLists = malloc(vertices * sizeof(struct node*));
    for(int i=0; i<vertices; i++)
        graph-> adjLists[i] = NULL;
    return graph;
}

void addVerticesList(struct Graph *graph, int s, int d){
    struct node *newNode = createNode(d);
    newNode-> prox = graph-> adjLists[s];
    graph-> adjLists[s] = newNode;
    newNode = createNode(s);
    newNode-> prox = graph-> adjLists[d];
    graph-> adjLists[d] = newNode;
}

void printGraphList(struct Graph *graph){
    for(int v=0; v<graph->numVertices; v++){
        struct node *temp = graph-> adjLists[v];
        printf("\nvértice %d: ",v+1);
        while(temp){
            printf("-> %d ", temp-> vertex+1);
            temp = temp-> prox;
        }
    }
}

void init(int adj[no_vertices][no_vertices]){
    for(int i=0; i<no_vertices; i++)
        for(int j=0; j<no_vertices; j++)
            adj[i][j]=0;
}

void addVertices(int adj[no_vertices][no_vertices], FILE *file){
    int s, d;
    while(fscanf(file, "%d %d", &s, &d) != EOF){
        adj[s-1][d-1] = 1;
        adj[d-1][s-1] = 1;
    }
}

void printGraph(int adj[no_vertices][no_vertices]){
    for(int i = 0; i < no_vertices; i++){
        for(int j = 0; j < no_vertices; j++)
            printf(" %d ", adj[i][j]);
        printf("\n");
    }
}

void dfs(struct Graph* graph, int vertex, int *visited) {
    visited[vertex] = 1;
    printf("%d ", vertex+1); 
    struct node *adjList = graph->adjLists[vertex];
    while (adjList != NULL) {
        int adjVertex = adjList->vertex;
        if (!visited[adjVertex]) {
            dfs(graph, adjVertex, visited);
        }
        adjList = adjList->prox;
    }
}

int countingArestas(int adj[no_vertices][no_vertices]) {
    int arestas = 0;
    for (int i = 0; i < no_vertices; i++) {
        for (int j = 0; j < no_vertices; j++) {
            if (adj[i][j] == 1) {
                arestas++;
            }
        }
    }

    return arestas/2;
}

void countingGrau(int adj[no_vertices][no_vertices]){
    int graus=0, first;
    FILE *file = fopen("grafo.txt", "r");
    if (file == NULL) {
        printf("erro ao abrir o arquivo!\n");
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%d", &no_vertices);
    for (int i = 0; i < no_vertices; i++) {
        for (int j = 0; j < no_vertices; j++) {
            if (adj[i][j] == 1) {
                graus++;
            }
        }
        fscanf(file, "%d %*d", &first);
        printf("\n%d %d", first, graus);  
        graus = 0;
    }
}

void dfsConexos(struct Graph *graph, int v, int *visited, int *component, int *size, int *idx) {
    visited[v] = 1; 
    component[*idx] = v + 1;  
    (*size)++;  
    (*idx)++;  
    struct node* adj = graph->adjLists[v];
    while (adj != NULL) {
        if (!visited[adj->vertex]) {
            dfsConexos(graph, adj->vertex, visited, component, size, idx);
        }
        adj = adj->prox;
    }
}

void components_conexos(struct Graph *graph) {
    int visited[no_vertices];  
    for (int i = 0; i < no_vertices; i++) {
        visited[i] = 0;
    }
    int component[no_vertices];  
    int size;  
    int idx;  
    int num_components = 0;  
    for (int v = 0; v < graph->numVertices; v++) {
        if (!visited[v]) {
            size = 0;
            idx = 0;
            printf("\ncomponente %d: ", ++num_components);
            dfsConexos(graph, v, visited, component, &size, &idx);
            printf("\ntamanho: %d, \nvértices: ", size);
            for (int i = 0; i < size; i++) {
                printf("\n%d ", component[i]);
            }
            printf("\n");
        }
    }
    printf("número total de componentes conexos: %d\n", num_components);
}

void bfs(struct Graph *graph, int ini){
    int visited[graph-> numVertices+1], queue[graph-> numVertices+1], front=-1, rear=-1;
    for(int count=0; count<graph-> numVertices; count++)
        visited[count]=0;
    visited[ini] = 1;
    queue[++rear] = ini;
    front ++;
    while(front<=rear){
        int currentVertex = queue[front++];
        printf("%d\t", currentVertex+1);
        struct node *temp = graph->adjLists[currentVertex];
        while(temp) {
            int adjVertex = temp->vertex;
            if (!visited[adjVertex]) {
                visited[adjVertex] = 1;
                queue[++rear] = adjVertex;
            }
            temp = temp->prox;
        }
    }
}

int minDistance(float dist[], bool arrSet[], int no_vertices) {
    float min = FLT_MAX;
    int min_index;

    for (int v = 0; v < no_vertices; v++) {
        if (!arrSet[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void printPath(int parent[], int destino) {
    if (parent[destino] == -1) {
        printf("caminho não encontrado!\n");
        return;
    }

    int path[destino + 1];
    int pathLength = 0;
    int current = destino;
    while (current != -1) {
        path[pathLength++] = current;
        current = parent[current];
    }
    printf("Caminho mais curto: ");
    for (int i = pathLength - 1; i >= 0; i--) {
        printf("%d ", path[i] + 1);  
        if (i > 0) printf("-> ");
    }
    printf("\n");
}

void dijkstra(struct nodeD graphh[], int origem, int destino, int no_vertices) {
    float dist[no_vertices]; 
    bool arrSet[no_vertices]; 
    int parent[no_vertices];  
    for (int i = 0; i < no_vertices; i++) {
        dist[i] = FLT_MAX;
        arrSet[i] = false;
        parent[i] = -1;  
    }
    dist[origem] = 0;
    for (int count = 0; count < no_vertices - 1; count++) {
        int u = minDistance(dist, arrSet, no_vertices);
        arrSet[u] = true;
        for (int v = 0; v < no_vertices; v++) {
            if (!arrSet[v] && graphh[u].arestas[v] != 0 &&
                dist[u] != FLT_MAX && dist[u] + graphh[u].arestas[v] < dist[v]) {
                dist[v] = dist[u] + graphh[u].arestas[v];
                parent[v] = u; 
            }
        }
    }
    printf("distância do vértice %d até o vértice %d: %.2f\n", origem + 1, destino + 1, dist[destino]);
    printPath(parent, destino);
}

void newFile(){
    FILE *file = fopen("grafo.txt", "w");
    if(file == NULL){
        printf("erro ao criar arquivo!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "5\n");
    //teste bfs
    /*fprintf(file, "1 2\n");
    fprintf(file, "1 5\n");
    fprintf(file, "2 5\n");
    fprintf(file, "3 4\n");
    fprintf(file, "3 5\n");
    fprintf(file, "4 5\n");*/
    //teste dijkstra
    fprintf(file, "1 2 0.1\n");
    fprintf(file, "1 5 1.0\n");
    fprintf(file, "2 5 0.2\n");
    fprintf(file, "4 3 -9.5\n");
    fprintf(file, "5 3 5.0\n");
    fprintf(file, "5 4 2.3\n");
    fclose(file);
}

int main() {
    newFile();
    FILE *file = fopen("grafo.txt", "r");
    if (file == NULL) {
        printf("erro ao abrir o arquivo!\n");
        return 1;
    }
    
    int a, b;
    float p;
    bool temPeso = false;
    fscanf(file, "%d", &no_vertices);
    char linha[100];
    while (fgets(linha, sizeof(linha), file)) {
        if (sscanf(linha, "%d %d %f", &a, &b, &p) == 3) {
            temPeso = true;
            break;
        } else if (sscanf(linha, "%d %d", &a, &b) == 2) {
            temPeso = false;
            break;
        }
    }
    
    /*int s, d, adj[no_vertices][no_vertices];
    init(adj);
    addVertices(adj, file);
    int m = 0;
    m = countingArestas(adj);
    printf("\n# n = %d", no_vertices);
    printf("\n# m = %d", m);
    countingGrau(adj);
    fclose(file);
    printGraph(adj);*/
    
    fclose(file);
    
    file = fopen("grafo.txt", "r");
    if (file == NULL) {
        printf("erro ao abrir o arquivo!\n");
        return 1;
    }

    fscanf(file, "%d", &no_vertices);
    int origem, destino;
    
    if(temPeso){
        struct nodeD graphD[no_vertices];
        for (int i = 0; i < no_vertices; i++) {
            graphD[i].vert = i;
            graphD[i].arestas = (float *)malloc(no_vertices * sizeof(float));
            for (int j = 0; j < no_vertices; j++) {
                graphD[i].arestas[j] = 0;
            }
        }
        while(fscanf(file, "%d %d %f", &a, &b, &p) != EOF){
            graphD[a-1].arestas[b-1] = p;
            graphD[b-1].arestas[a-1] = p;
        }
        printf("\nescolha o vértice de origem (1 a %d): ", no_vertices);
        scanf("%d", &origem);
        printf("\nescolha o vértice de destino (1 a %d): ", no_vertices);
        scanf("%d", &destino);
        if (origem < 1 || origem > no_vertices || destino < 1 || destino > no_vertices) {
            printf("\nvértices inválidos!");
            return -1;
        }
        origem -= 1;
        destino -= 1;
        dijkstra(graphD, origem, destino, no_vertices);
        for (int i = 0; i < no_vertices; i++) {
            free(graphD[i].arestas);
        }
    }else{
        struct Graph *graph = createAGraph(no_vertices);
        while(fscanf(file, "%d %d", &a, &b) != EOF){
            addVerticesList(graph, a-1, b-1);
        }
        printf("\nescolha o vértice de origem (1 a %d): ", no_vertices);
        scanf("%d", &origem);
        if (origem < 1 || origem > no_vertices) {
            printf("\nvértice de origem inválido!");
            return -1;
        }
        bfs(graph, origem-1);
    }
    fclose(file);
    
    /*printGraphList(graph);
    
    int ch, inicio;
    
    int* visited = malloc(no_vertices * sizeof(int));
    
    for (int i = 0; i < no_vertices; i++) {
        visited[i] = 0;
    }
    
    components_conexos(graph);
     
    do{
        printf("\nDigite 1 para realizar a busca por largura: ");
        printf("\nDigite 2 para realizar a busca por profundidade: ");
        scanf("%d",&ch);
        switch(ch){
            case 1:
                printf("Digite o vértice pelo qual deseja iniciar: ");
                scanf("%d", &inicio);
                bfs(graph, inicio);
            break;
            case 2:
                printf("Digite o vértice pelo qual deseja iniciar: ");
                scanf("%d", &inicio);
                dfs(graph, inicio, visited);
                free(visited);
                    }
    }while(ch != 3);*/

    return 0;
}


