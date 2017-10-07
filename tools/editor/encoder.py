
if __name__ == '__main__':
    import sys
    import json
    from PyQt5.QtWidgets import QApplication
    if len(sys.argv) == 2:
        from models import Document
        from util import (
            PieceDetector, FrameDetector, OfficialFormat, convert_from_str
        )
        app = QApplication(sys.argv)
        project_data = None
        with open(sys.argv[1], 'r') as file:
            project_data = json.load(file)
        if project_data is not None:
            of = OfficialFormat()
            pieces = PieceDetector(project_data).search()
            p_data = [[convert_from_str(p) for p in f[:-1]] for f in pieces]
            p_data = [of.convert_to_official(p) for p in p_data]
            pieces = [of.remove_offset(p) for p in pieces]
            pieces = [of.convert_to_official(p) for p in pieces]
            frames = FrameDetector(project_data).search()
            frames = [[convert_from_str(p) for p in f[:-1]] for f in frames]
            frames = [of.convert_to_official(f) for f in frames]
            data = '{}:{}:{}'.format(
                len(pieces),
                ':'.join(pieces),
                ':'.join(frames)
            )
            with open('{}.txt'.format(project_data['name']), 'w') as file:
                file.write(data)

            import random
            num = len(p_data)
            rnum = random.randint(int(num * 0.2), int(num * 0.5))
            rcnt = 0
            while rcnt < rnum:
                r = random.randint(1, len(p_data) - 1)
                del p_data[r]
                rcnt += 1
            data = '{}:{}'.format(len(p_data), ':'.join(p_data))
            with open('place_{}.txt'.format(project_data['name']), 'w') as file:
                file.write(data)
