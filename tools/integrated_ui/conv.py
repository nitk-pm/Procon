import numpy as np
import math

import geom

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

def compile_codes_to_dict(codes):
    all_pieces = []
    all_frames = []
    def ctor_shapes(shapes):
        def ctor_points(p):
            return {'x': p[0][0], 'y':[1][0]}
        return list(map(lambda shape:
            list(map(ctor_points, shape)), shapes))
    for code in codes:
        pieces, frames = parse_code(code)
        for piece in pieces:
            all_pieces.append(ctor_shapes(expand_piece(piece)))
        for frame in frames:
            all_frames.append(frame)
    piece_dict = {'pieces': all_pieces}
    frame_dict = {'frames': ctor_shapes(all_frames)}
    return piece_dict, frame_dict
