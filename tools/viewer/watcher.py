from watchdog.events import FileSystemEventHandler
from watchdog.observers import Observer
import subprocess


class Handler(FileSystemEventHandler):

    callback_list = []

    def handler(self, func, *args):
        return func(*args)

    def run_command(self, event):
        if event.is_directory:
            return
        if event.src_path.rsplit('.', 1)[-1] != 'png':
            return
        data = subprocess.run(
            ['zbarimg', event.src_path],
            stdout=subprocess.PIPE
        )

        for callback_list in self.callback_list:
            self.handler(
                callback_list,
                event.src_path,
                self.shaping_data(data.stdout.decode('utf-8'))
            )

    def on_created(self, event):
        self.run_command(event)

    def on_moved(self, event):
        self.run_command(event)

    def on_modified(self, event):
        self.run_command(event)

    def shaping_data(self, data):
        data_list = [d.replace('QR-Code:', '') for d in data.splitlines()]
        head = []
        tail = None
        total = 0
        for d in data_list:
            num_str, shapes = d.split(':', 1)
            num = int(num_str)
            shapes_num = len(shapes.split(':'))
            total += num
            if num != shapes_num:
                tail = shapes
            else:
                head.append(shapes)
        head.append(tail)
        return '{}:{}'.format(total, ':'.join(head))


class Watcher(object):

    def __init__(self, callback=None):
        self.observer = Observer()
        self.handler = Handler()
        self.watch = None
        if callback is not None:
            self.register_callback(callback)

    def register_callback(self, func):
        self.handler.callback_list.append(func)

    def start(self, path):
        if self.watch is None:
            self.watch = self.observer.schedule(
                self.handler,
                path,
                recursive=True
            )
            self.observer.start()

    def stop(self):
        if self.watch is not None:
            self.observer.stop()
            self.observer.join()
            self.watch = None


class Test(object):

    def callback(self, data):
        print(data)


if __name__ == '__main__':
    test = Test()
    watcher = Watcher()
    watcher.register_callback(test.callback)

    watcher.start('./')
    try:
        import time
        while True:
            time.sleep(1)
    except:
        watcher.stop()
