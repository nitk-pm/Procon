from watchdog.events import FileSystemEventHandler
from watchdog.observers import Observer


class ChangeHandler(FileSystemEventHandler):

    def on_created(self, event):
        if event.is_directory:
            return
        print('created: {}'.format(event.src_path))

    def on_modified(self, event):
        if event.is_directory:
            return
        print('modified: {}'.format(event.src_path))

    def on_moved(self, event):
        if event.is_directory:
            return
        print('moved: {}'.format(event.src_path))

    def on_deleted(self, event):
        if event.is_directory:
            return
        print('deleted: {}'.format(event.src_path))


if __name__ in '__main__':
    while True:
        event_handler = ChangeHandler()
        observer = Observer()
        observer.schedule(event_handler, './', recursive=True)
        observer.start()
        try:
            import time
            while True:
                time.sleep(1)
        except:
            observer.stop()
        observer.join()
