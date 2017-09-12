import os
from util import ProblemDecoder


class ProblemData(object):

    def __init__(self, filename: str, data: str=None):
        self.dir_path, self.filename = os.path.split(filename)
        self.decoder = ProblemDecoder()
        if data is not None:
            self.set_data(data)

    def set_data(self, data: str):
        data_list = data.split(':')
        self.num_piece = int(data_list.pop(0))
        self.set_piece(data_list[:self.num_piece])
        self.set_frame(data_list[self.num_piece:])

    def set_piece(self, data_list: list):
        self.pieces = [self.decoder.expand_patterns(d) for d in data_list]

    def set_frame(self, data_list: list):
        self.frame = [self.decoder.to_polygon(d) for d in data_list]
