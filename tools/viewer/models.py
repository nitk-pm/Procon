import os
from encoder import Encoder


class ProblemData(object):

    def __init__(self, filename: str, data: str):
        self.full_path = filename
        self.dir_path, self.filename = os.path.split(filename)
        self.display_name = self.filename.rsplit('.', 1)[0]
        self.encoder = Encoder()
        if data is not None:
            self.set_data(data)

    def set_data(self, data: str):
        self.data_list = data.split(':')
        self.num_piece = int(self.data_list.pop(0))
        self.set_piece(self.data_list[:self.num_piece])
        self.set_frame(self.data_list[self.num_piece:])

    def set_piece(self, data_list: list):
        self.pieces = [self.encoder.expand_patterns(d) for d in data_list]

    def set_frame(self, data_list: list):
        self.frame = [self.encoder.to_polygon(d) for d in data_list]

    def to_dict_piece(self):
        piece = {'pieces': []}
        for d in self.data_list[:self.num_piece]:
            piece['pieces'].append(self.encoder.to_dict(d))
        return piece

    def to_dict_frame(self):
        frame = {'shapes': []}
        for d in self.data_list[self.num_piece:]:
            frame['shapes'].append(self.encoder.to_dict(d, True))
        return frame
