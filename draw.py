import matplotlib.pyplot as plt
from pandas import DataFrame
import networkx as nx

while 1 == 1:
    graph = input()
    n = ord(graph[0]) - ord('A') + 2
    s = ""
    for i in range(len(graph) - 1, 0, -1):
        k = ord(graph[i]) - 63
        for tmp in range(6):
            s += chr(int(k % 2) + ord('0'))
            k = int(k / 2)

    g = [[0 for i in range(n)] for j in range(n)]
    po = len(s) - 1
    for j in range(n - 1, -1, -1):
        for i in range(j - 1, -1, -1):
            g[j][i] = g[i][j] = ord(s[po]) - ord('0')
            po -= 1
    print(DataFrame(g))
    G = nx.Graph()
    G.add_nodes_from([i for i in range(n)])
    for i in range(n):
        for j in range(i):
            if g[i][j] == 1:
                G.add_edge(i, j)
    nx.draw(G, with_labels=True)
    plt.show()