import networkx as nx

def ler_grafo(arquivo_nome):
    G = nx.Graph()  
    with open(arquivo_nome, 'r') as arquivo:
        numVertices = int(arquivo.readline().strip())  
        for linha in arquivo:
            if len(linha.split()) != 3:
                continue  
            u, v, peso = map(int, linha.split())
            G.add_edge(u, v, weight=peso)  
    return G

def dijkstra(G, origem, destino):
    try:
        distancia, caminho = nx.single_source_dijkstra(G, origem, destino)
        print(f"menor distância de {origem} para {destino}: {distancia}")
        print("caminho:", " -> ".join(map(str, caminho)))
    except nx.NetworkXNoPath:
        print(f"não há caminho entre {origem} e {destino}")

arquivo_nome = "grafo1.txt"
#arquivo_nome = "grafo2.txt"
#arquivo_nome = "grafo3.txt"
#arquivo_nome = "grafo4.txt"
#arquivo_nome = "grafo5.txt"
grafo = ler_grafo(arquivo_nome)
destino = int(input("vértice de destino: "))
dijkstra(grafo, 1, destino)
