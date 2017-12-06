import numpy as np
import math

import geom
import json

def parse_code (bincode):
    splited = str(bincode)[2:][:-1].split(':')
    piece_num = int(splited[0])
    shapes    = list(map(
        lambda s: list(map(lambda num: float(num),
            filter (lambda num: num != '', s.split(' ')))),
        splited[1:]))
    def shape_ctor(nums):
        nums = nums[1:]
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

def ctor_point(p):
    return {'x':p[0][0], 'y':p[1][0]}

def ctor_shape(ps):
    return [ctor_point(p) for p in ps]

def ctor_shapes(ss):
    return {'shapes': [ctor_shape(s) for s in ss]}

def round_shape(s):
    def round_pt (pt):
        return [[int(pt[0][0])], [int(pt[1][0])]]
    return [round_pt(pt) for pt in s]

def compile_shape_codes_to_dict(codes):
    all_pieces = []
    all_frames = []
    for code in codes:
        pieces, frames = parse_code(code)
        for piece in pieces:
            all_pieces.append(ctor_shapes([round_shape(normalize(shape)) for shape in expand_piece(set_orig(piece))]))
        for frame in frames:
            all_frames.append(round_shape(normalize(frame)))
    piece_dict = {'pieces': all_pieces}
    frame_dict = ctor_shapes(all_frames)
    return piece_dict, frame_dict

def compile_place_codes_to_dict(codes):
    all_pieces = []
    for code in codes:
        pieces, _ = parse_code(code)
        for piece in pieces:
            all_pieces.append(piece)
    dict = {'pieces': ctor_shapes(all_pieces)}
    return dict

def dump_shape_json(codes):
    piece_dic, frame_dic = compile_shape_codes_to_dict(codes)
    piece_file = open('piece.json', 'w')
    frame_file = open('frame.json', 'w')
    json.dump(piece_dic, piece_file, indent=4, sort_keys=True)
    json.dump(frame_dic, frame_file, indent=4, sort_keys=True)

def dump_place_json(codes):
    dic = compile_place_codes_to_dict(codes)
    place_file = open('place.json', 'w')
    json.dump(dic, place_file)

def load_result(file):
    nums = [int(line.strip()) for line in file]
    assert len(nums) % 2 == 0, 'result is invalid format'
    shapes = []
    shape = []
    for i in range(0, len(nums), 2):
        x = nums[i]
        y = nums[i+1]
        if len(shape) != 0 and shape[0][0] == x and shape[0][1] == y:
            shapes.append(shape)
            shape = []
        else:
            shape.append((x, y))
    return shapes

# 頂点一つを原点と合わせる処理
def set_orig(shape):
    orig_x = shape[0][0][0]
    orig_y = shape[0][1][0]
    return [[[pt[0][0]-orig_x], [pt[1][0]-orig_y]] for pt in shape]

def normalize(shape):
    min_x = np.min([pt[0][0] for pt in shape])
    min_y = np.min([pt[1][0] for pt in shape])
    return [[[pt[0][0]-min_x], [pt[1][0]-min_y]] for pt in shape]
