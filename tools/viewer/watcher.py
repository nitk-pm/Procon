from watchdog.events import FileSystemEventHandler
from watchdog.observers import Observer
import subprocess


class Handler(FileSystemEventHandler):

    on_read = []

    def handler(self, func, *args):
        return func(*args)

    def on_created(self, event):
        if event.is_directory:
            return
        data = subprocess.run(['zbarimg', event.src_path])

        for on_read in self.on_read:
            self.handler(on_read, data)


class Watcher(object):

    def __init__(self):
        self.observer = Observer()
        self.handler = Handler()

    def register_callback(self, func):
        self.handler.on_read.append(func)

    def start(self):

