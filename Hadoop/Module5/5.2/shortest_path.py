from collections import defaultdict

class Graph:
    def __init__(self):
        self.nodes = set()
        self.edges = defaultdict(list)
        self.distances = {}

    def add_node(self, value):
        self.nodes.add(value)

    def add_edge(self, from_node, to_node, distance):
        self.edges[from_node].append(to_node)
        self.distances[(from_node, to_node)] = distance


def dijkstra(graph, initial):
    visited = {initial: 0}
    path = {}

    nodes = set(graph.nodes)

    while nodes: 
        min_node = None
        for node in nodes:
            if node in visited:
                if min_node is None:
                    min_node = node
                elif visited[node] < visited[min_node]:
                    min_node = node

        if min_node is None:
            break

        nodes.remove(min_node)
        current_weight = visited[min_node]

        for edge in graph.edges[min_node]:
            weight = current_weight + graph.distances[(min_node, edge)]
            if edge not in visited or weight < visited[edge]:
                visited[edge] = weight
                path[edge] = min_node

    return visited, path
    
import sys

g = Graph()
n, m = map(int, sys.stdin.readline().strip().split())
for i in range(0, m):
    v1, v2, w = map(int, sys.stdin.readline().strip().split())
    g.add_node(v1)
    g.add_node(v2)
    g.add_edge(v1, v2, w)
s, f = map(int, sys.stdin.readline().strip().split())

weight, path = dijkstra(g, s)
if f in weight:
    print(weight[f])
else:
    print('-1')
