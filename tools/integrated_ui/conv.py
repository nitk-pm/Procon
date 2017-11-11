import numpy as np
import math

def parse_code (str):
    splited = str.split(':')
    piece_num = int(splited[0])
    shapes    = list(map(
        lambda s: list(map(lambda num: float(num),
            filter (lambda num: num != '', s.split(' ')))),
        splited[1:]))
    def shape_ctor(nums):
        assert len(nums) % 2 == 0, "illegal data"
        shape = []
        for i in range(0, len(nums), 2):
            shape.append(np.asarray([[nums[i]], [nums[i+1]]]))
        return shape
    pieces_base = shapes[:piece_num]
    frames_base = shapes[piece_num:]
    # piece, frame
    return list(map(shape_ctor, pieces_base)), list(map(shape_ctor, frames_base))

def expand_piece (base_shape):
    rotateds = []
    for angle in [0., math.pi/2., math.pi, math.pi*3./2.]:
        rotated = geom.rotate(base_shape, angle)
        rotateds.append(rotated)
        rotateds.append(geom.invert(rotated))
    return rotateds
