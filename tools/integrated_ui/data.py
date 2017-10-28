#!/usr/bin/python
# -*- codign: utf-8 -*-

import unittest

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, p):
        return Point(self.x + p.x, self.y + p.y)

    def __sub__(self, p):
        return Point(self.x - p.x, self.y - p.y)

    def __mul__(self, p):
        return Point(self.x * p.x + self.y * p.y)

class Shape:
    def __init__(self, points):
        self.points = points
