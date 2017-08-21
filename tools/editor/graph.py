
class Graph(object):
    nodes = {}

    def add_node(self, node):
        if node not in self.nodes:
            self.nodes[node] = []

    def add_edge(self, src, dest):
        self.add_node(src)
        self.nodes[src].append(dest)
        self.add_node(dest)
        self.nodes[dest].append(dest)

    def remove_node(self, node):
        if node not in self.nodes:
            return

        for n in self.nodes[node]:
            self.nodes[n].remove(node)
        del self.nodes[node]

    def remove_edge(self, src, dest):
        if (src not in self.nodes) or (dest not in self.nodes):
            return
        self.nodes[src].remove(dest)
        self.nodes[dest].remove(src)

    def get_closed_paths(self):
        pass

    def shortest_path(self, start, goal):
        pass
