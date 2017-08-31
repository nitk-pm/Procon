from PyQt5.QtCore import QPointF
from PyQt5.QtGui import QPolygonF
import re


def convert_from_str(p_str):
    pattern = r'\(\d*, \d*\)'
    if re.match(pattern, p_str) is None:
        return None
    p = re.findall(r'([+-]?[0-9]+\.?[0-9]*)', p_str)
    return QPointF(int(p[0]), int(p[1]))


def convert_from_point(point):
    return '({:.0f}, {:.0f})'.format(point.x(), point.y())


class ConvexHull(object):

    def __init__(self, points=None):
        self.points = []
        if points is not None:
            self.setup(points)

    def setup(self, points):
        import re
        self.points.clear()
        for point in points:
            p = re.findall(r'([+-]?[0-9]+\.?[0-9]*)', point)
            self.points.append(QPointF(int(p[0]), int(p[1])))

    def get(self, to_str=True):
        result = []
        result.append(self.min_point())
        result.append(self.min_radian_point(result[-1]))
        while not self.equal(result[0], result[-1]):
            result.append(self.min_radian_point(result[-1]))
        result.pop()

        if to_str:
            result = ['({:.0f}, {:.0f})'.format(p.x(), p.y()) for p in result]
        return result

    def min_point(self):
        m_p = self.points[0]
        for p in self.points:
            if m_p.x() > p.x():
                m_p = p
            elif m_p.x() == p.x() and m_p.y() > p.y():
                m_p = p
        return m_p

    def min_radian_point(self, point):
        m_p = point
        for p in self.points:
            if m_p.x() == p.x() and m_p.y() == p.y():
                m_p = p
            else:
                p1 = m_p - point
                p2 = p - point
                v = self.cross(p1, p2)
                if v > 0 or (v == 0 and self.length(p2) > self.length(p1)):
                    m_p = p
        return m_p

    def equal(self, p1, p2):
        return p1.x() == p2.x() and p1.y() == p2.y()

    def cross(self, p1, p2):
        return p1.x() * p2.y() - p1.y() * p2.x()

    def length(self, p):
        import math
        return math.sqrt(p.x() * p.x() + p.y() * p.y())


class ClosedGraphDetector(object):

    def __init__(self, project_data):
        items = project_data['items']
        self.graph = {}
        self.hash = []
        self.paths = []
        for node in items:
            self.graph[node['pos']] = [link for link in node['linked_nodes']]

    def search(self):
        # 閉路グラフの検出
        for node in self.graph.keys():
            self.bfs(node, node)

        # 閉路グラフの性質を満たさないものを削除
        self.correct_paths()

        return self.paths

    def bfs(self, start, goal):
        queue = []
        for n in self.graph[start]:
            new_path = [start]
            new_path.append(n)
            queue.append(new_path)
        while len(queue) > 0:
            logger.debug('queue: {}'.format(queue))
            path = queue.pop(0)
            node = path[-1]
            if node == goal and self.check(path):
                self.paths.append(path)
                self.hash.append(set(path))
                return
            for n in self.graph[node]:
                if n not in path:
                    new_path = path[:]
                    logger.debug('n: {}, path: {}'.format(n, new_path))
                    new_path.append(n)
                    queue.append(new_path)
                elif n == start:
                    new_path = path[:]
                    logger.debug('n: {}, path: {}'.format(n, new_path))
                    new_path.append(n)
                    queue.append(new_path)

    def check(self, path):
        p = set(path)
        if path.x() in path[1:-1]:
            return False
        return len(path) in range(4, 16) and p not in self.hash

    def correct_paths(self):
        self.contains_edge()

    def contains_edge(self):
        from itertools import product
        path_set = [set(path) for path in self.paths]
        length = len(path_set)
        find = [False for i in range(length)]
        for i, j in product(range(length), range(length)):
            if i != j:
                s = path_set[i] & path_set[j]
                if s in path_set:
                    find[i if i > j else j] = True
        paths = []
        for i in range(length):
            if not find[i]:
                paths.append(path_set[i])
        self.paths = paths

    def contains_point(self):
        pass
