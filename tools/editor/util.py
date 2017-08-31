from PyQt5.QtCore import Qt, QPointF
from PyQt5.QtGui import QPolygonF
import re


def convert_from_str(p_str):
    if re.match(r'\(\d*, \d*\)', p_str) is None:
        return None
    p = re.findall(r'([+-]?[0-9]+\.?[0-9]*)', p_str)
    return QPointF(int(p[0]), int(p[1]))


def convert_from_point(point):
    return '({:.0f}, {:.0f})'.format(point.x(), point.y())


class ClosedGraphDetector(object):

    def __init__(self, project_data=None):
        if project_data is not None:
            self.setup(project_data)

    def setup(self, project_data):
        self.graph = {}
        self.hash = []
        self.paths = []
        for node in project_data['items']:
            self.graph[node['node']] = [link for link in node['linked_nodes']]

    def search(self):
        # 閉路グラフの検出
        for node in self.graph.keys():
            self.bfs(node, node)

        # 内部に辺が含まれている閉路グラフを除去
        self.correct_with_edge()

        # 内部にノードが含まれている閉路グラフを除去
        self.correct_with_point()

        # 不要なノードを除去
        self.correct_with_angle()

        # 開始点の調整
        # self.adjust()

        return self.paths

    def bfs(self, start, goal):
        queue = []
        for n in self.graph[start]:
            new_path = [start]
            new_path.append(n)
            queue.append(new_path)
        while len(queue) > 0:
            path = queue.pop(0)
            node = path[-1]
            if node == goal and self.check(path):
                self.paths.append(path)
                self.hash.append(set(path))
                return
            for n in self.graph[node]:
                if n not in path:
                    new_path = path[:]
                    new_path.append(n)
                    queue.append(new_path)
                elif n == start:
                    new_path = path[:]
                    new_path.append(n)
                    queue.append(new_path)

    def check(self, path):
        p = set(path)
        if path[0] in path[1:-1]:
            return False
        return len(path) > 3 and p not in self.hash

    def correct_with_edge(self):
        from itertools import product
        path_set = [set(path) for path in self.paths]
        length = len(path_set)
        find = [False for i in range(length)]
        for i, j in product(range(length), range(length)):
            if i != j:
                s = path_set[i] & path_set[j]
                if s in path_set:
                    index = i if len(path_set[i]) > len(path_set[j]) else j
                    find[index] = True
        paths = []
        for i in range(length):
            if not find[i]:
                paths.append(self.paths[i])
        self.paths = paths

    def correct_with_point(self):
        paths = []
        for i, path in enumerate(self.paths):
            polygon = self.to_polygon(path)
            find = False
            for pos in self.graph.keys():
                if pos not in path:
                    p = convert_from_str(pos)
                    if polygon.containsPoint(p, Qt.WindingFill):
                        find = True
                        break
            if not find:
                paths.append(path)
        self.paths = paths

    def to_polygon(self, path):
        points = [convert_from_str(p) for p in path]
        return QPolygonF(points)

    def correct_with_angle(self):
        from PyQt5.QtCore import QLineF
        for index, path in enumerate(self.paths):
            length = len(path)
            find = [False for i in range(length)]
            for i in range(length - 2):
                p1 = convert_from_str(path[i])
                p2 = convert_from_str(path[i + 1])
                p3 = convert_from_str(path[i + 2])
                l1, l2 = QLineF(p1, p2), QLineF(p2, p3)
                if l1.angleTo(l2) == 0.0:
                    find[i + 1] = True
            new_path = []
            for i in range(length):
                if not find[i]:
                    new_path.append(path[i])
            self.paths[index] = new_path

    def adjust(self):
        for index, path in enumerate(self.paths):
            new_path = path[1:]
            m_node = self.min_point(new_path)
            while m_node == new_path[0]:
                new_path = new_path[:-1] + new_path[:-1]
            new_path.append(m_node)
            self.paths[index] = new_path

    def min_point(self, path):
        m_p_str = path[0]
        m_p = convert_from_str(m_p_str)
        for node in path:
            p = convert_from_str(node)
            if m_p.y() > p.y():
                m_p_str = node
                m_p = p
            elif m_p.y() == p.y() and m_p.x() > p.x():
                m_p_str = node
                m_p = p
        return m_p_str
