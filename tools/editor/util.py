from PyQt5.QtCore import Qt, QPointF, QLineF
from PyQt5.QtGui import QPolygonF
import re
import math
from logging import getLogger
logger = getLogger()


def convert_from_str(p_str):
    if re.match(r'\([+-]?\d*, [+-]?\d*\)', p_str) is None:
        return None
    p = re.findall(r'([+-]?[0-9]+\.?[0-9]*)', p_str)
    return QPointF(int(p[0]), int(p[1]))


def convert_from_point(point):
    return '({:.0f}, {:.0f})'.format(point.x(), point.y())


def cross(p1, p2):
    return p1.x() * p2.y() - p1.y() * p2.x()


class PieceDetector(object):

    def __init__(self, project_data=None):
        if project_data is not None:
            self.setup(project_data)

    def setup(self, project_data):
        self.graph = {}
        self.hash = []
        self.paths = []
        for node in project_data['items']:
            self.graph[node['node']] = node['linked-nodes'][:]

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

        # 時計回りに修正
        self.adjust_loopwise()

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

    def adjust_loopwise(self):
        for i in range(len(self.paths)):
            if self.signed_area(self.paths[i]) < 0:
                self.paths[i].reverse()

    def signed_area(self, path):
        points = [convert_from_str(node) for node in path]
        length = len(points)
        area = 0.0
        for i in range(length - 1):
            area += cross(points[i], points[i + 1])
        return area / 2


class FrameDetector(object):

    def __init__(self, project_data=None):
        if project_data is not None:
            self.setup(project_data)

    def setup(self, project_data):
        self.graph = {}
        for node in project_data['items']:
            self.graph[node['node']] = node['linked-nodes'][:]

    def search(self):
        self.path = []
        self.convex_hull_on_graph()
        self.adjust_loopwise()
        return self.path

    def convex_hull_on_graph(self):
        self.path.append(self.min_point())
        self.path.append(self.min_radian_point(self.path[0], '(-1, -1)'))
        while self.path[0] != self.path[-1]:
            p = self.min_radian_point(self.path[-1], self.path[-2])
            self.path.append(p)

    def min_point(self):
        nodes = list(self.graph.keys())
        m_str = nodes[0]
        m_p = convert_from_str(m_str)
        for node in nodes:
            if len(self.graph[node]) == 0:
                continue
            n = convert_from_str(node)
            if m_p.y() > n.y():
                m_str = node
                m_p = convert_from_str(m_str)
            elif m_p.y() == n.y() and m_p.x() > n.x():
                m_str = node
                m_p = convert_from_str(m_str)
        return m_str

    def min_radian_point(self, point_str, prev):
        point = convert_from_str(point_str)
        m_str = self.graph[point_str][0]
        m_p = convert_from_str(m_str)
        prev_p = convert_from_str(prev)
        m_angle = 360.0
        for node in self.graph[point_str]:
            p = convert_from_str(node)
            l1 = QLineF(point, prev_p)
            l2 = QLineF(point, p)
            angle = l1.angleTo(l2)
            if angle != 0 and m_angle > angle:
                m_angle = angle
                m_str = node
                m_p = convert_from_str(m_str)
        return m_str

    def length(self, point):
        return math.sqrt(point.x() * point.x() + point.y() * point.y())

    def adjust_loopwise(self):
        if self.signed_area() < 0:
            self.path.reverse()

    def signed_area(self):
        points = [convert_from_str(node) for node in self.path]
        length = len(points)
        area = 0.0
        for i in range(length - 1):
            area += cross(points[i], points[i + 1])
        return area / 2
