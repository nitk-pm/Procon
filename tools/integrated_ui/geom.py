import numpy as np
import math

def rotate (shape, angle):
    mat = np.asarray([  
        [math.cos(angle), -math.sin(angle)],
        [math.sin(angle),  math.cos(angle)]])
    return list(map(lambda p: mat.dot(p), shape))

def invert (shape):
    mat = np.asarray([
            [1., 0.],
            [0., -1.]])
    return list(map(lambda p: mat.dot(p), shape))
