from watchdog.events import FileSystemEventHandler
from watchdog.observers import Observer
import subprocess


class Handler(FileSystemEventHandler):

    callback_list = []

    def handler(self, func, *args):
        return func(*args)

    def on_created(self, event):
        if event.is_directory:
            return
        print('created')
        data = subprocess.run(
            ['zbarimg', event.src_path],
            stdout=subprocess.PIPE
        )

        for callback_list in self.callback_list:
            self.handler(
                callback_list,
                event.src_path,
                data.stdout.decode('utf-8')
            )


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
