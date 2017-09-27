from PyQt5.QtCore import Qt, QPointF, QLineF
from PyQt5.QtGui import QPolygonF
from abc import ABCMeta, abstractmethod
from logging import getLogger
import os
import re
import json
import math
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


def to_polygon(path):
    points = [convert_from_str(p) for p in path]
    return QPolygonF(points)


class BaseDetector(object):
    __metaclass__ = ABCMeta

    def __init__(self, project_data=None):
        if project_data is not None:
            self.setup(project_data)

    def setup(self, project_data):
        self.graph = {}
        for node in project_data['items']:
            if len(node['linked-nodes']) != 0:
                self.graph[node['node']] = node['linked-nodes'][:]

    @abstractmethod
    def search(self):
        pass


class PieceDetector(BaseDetector):

    def search(self):
        self.hash, self.paths = [], []
        self.search_count = 0

        # 閉路グラフの検出
        for i, node in enumerate(self.graph.keys()):
            logger.debug('progress: {}, node: {}'.format(i, node))
            self.bfs(node, node)

        logger.debug('search number of node: {}'.format(self.search_count))

        # 内部に辺が含まれている閉路グラフを除去
        self.correct_with_edge()

        # 内部にノードが含まれている閉路グラフを除去
        self.correct_with_point()

        # 不要なノードを除去
        self.correct_with_angle()

        # 時計回りに修正
        self.correct_loopwise()

        return self.paths

    def bfs(self, start, goal):
        find_count = 0
        queue = []
        for n in self.graph[start]:
            new_path = [start]
            new_path.append(n)
            queue.append(new_path)
        while len(queue) > 0:
            self.search_count += 1
            path = queue.pop(0)
            node = path[-1]
            if node == goal and self.check(path):
                self.paths.append(path)
                self.hash.append(set(path))
                find_count += 1
                if find_count >= 2:
                    return

            for n in self.graph[node]:
                if n not in path and len(set(path)) < 16:
                    new_path = path[:]
                    new_path.append(n)
                    queue.append(new_path)
                elif n == start:
                    new_path = path[:]
                    new_path.append(n)
                    queue.append(new_path)

    def check(self, path):
        if path[0] in path[1:-1]:
            return False
        p = set(path)
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
            polygon = to_polygon(path)
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

    def correct_loopwise(self):
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


class FrameDetector(BaseDetector):

    def search(self):
        self.paths = []
        self.nodes = list(self.graph.keys())
        while True:
            path = self.convex_hull_on_graph()
            self.paths.append(path)
            self.update_nodes(path)
            if len(self.nodes) == 0:
                break
        self.correct_with_angle()
        self.correct_loopwise()
        return self.paths

    def convex_hull_on_graph(self):
        path = []
        path.append(self.min_point())
        path.append(self.min_radian_point(path[0], '(-1, -1)'))
        while path[0] != path[-1]:
            p = self.min_radian_point(path[-1], path[-2])
            path.append(p)
        return path

    def update_nodes(self, path):
        polygon = to_polygon(path)
        nodes = []
        for node in self.nodes:
            flag = False
            for path in self.paths:
                if node in path:
                    flag = True
                    break
            if flag:
                continue
            p = convert_from_str(node)
            if not polygon.containsPoint(p, Qt.WindingFill):
                nodes.append(node)
        self.nodes = nodes

    def min_point(self):
        m_str = self.nodes[0]
        m_p = convert_from_str(m_str)
        for node in self.nodes:
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

    def correct_loopwise(self):
        for i, path in enumerate(self.paths):
            if self.signed_area(path) < 0:
                self.paths[i].reverse()

    def signed_area(self, path):
        points = [convert_from_str(node) for node in path]
        length = len(points)
        area = 0.0
        for i in range(length - 1):
            area += cross(points[i], points[i + 1])
        return area / 2

    def correct_with_angle(self):
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


class BaseFormat(object):
    __metaclass__ = ABCMeta

    def setup(self, document):
        self.document = document

    @abstractmethod
    def save(self, filename):
        pass

    @abstractmethod
    def load(self, filename):
        pass


class DefaultFormat(BaseFormat):

    def save(self, filename):
        self.document.set_project_settings(filename)
        with open(filename, 'w') as file:
            json.dump(self.document.to_dict(), file, indent=2)

    def load(self, filename):
        self.document.set_project_settings(filename)
        with open(filename, 'r') as file:
            self.document.restore(json.load(file))
            self.document.history.clear()


class OfficialFormat(BaseFormat):

    def save(self, filename):
        project_data = self.document.to_dict()
        pieces = PieceDetector(project_data).search()
        pieces = [self.remove_offset(p) for p in pieces]
        pieces = [self.convert_to_official(p) for p in pieces]
        frames = FrameDetector(project_data).search()
        frames = [[convert_from_str(p) for p in f[:-1]] for f in frames]
        frames = [self.convert_to_official(f) for f in frames]
        data = '{}:{}:{}'.format(
            len(pieces),
            ':'.join(pieces),
            ':'.join(frames)
        )
        with open(filename, 'w') as file:
            file.write(data)

    def load(self, filename):
        pass

    def remove_offset(self, path):
        piece = [convert_from_str(p) for p in path[:-1]]
        offset = QPointF(piece[0])
        for p in piece:
            if offset.x() > p.x():
                offset.setX(p.x())
            if offset.y() > p.y():
                offset.setY(p.y())
        for index in range(len(piece)):
            piece[index] = piece[index] - offset
        return piece

    def convert_to_official(self, points):
        data = ['{:.0f} {:.0f}'.format(p.x(), p.y()) for p in points]
        return '{} {}'.format(len(points), ' '.join(data))


class ScreenShotFormat(BaseFormat):

    def save(self, filename):
        from PyQt5.QtGui import QPainter, QImage
        scene = self.document.board.scene()
        w, h = scene.width(), scene.height()
        image = QImage(w, h, QImage.Format_ARGB32)
        with QPainter(image) as painter:
            scene.render(painter)
            image.save(filename)

    def load(self, filename):
        pass
