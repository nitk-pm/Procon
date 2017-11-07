#!/usr/bin/python3
# -*- codign: utf-8 -*-

from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.tabbedpanel import TabbedPanel
from kivy.uix.tabbedpanel import TabbedPanelItem
from kivy.uix.widget import Widget
from kivy.uix.label import Label
from kivy.uix.popup import Popup
from kivy.lang import Builder
from kivy.graphics import Color
from kivy.graphics import Rectangle
from kivy.graphics.texture import Texture
from kivy.clock import Clock
from PIL import Image
import cv2

class ResultCanvas(Widget):
    pass

class PieceSuggestCanvas(Widget):
    pass

class SolverCanvas(Widget):
    pass

class LoadCanvas(Widget):
    pass

class Base(TabbedPanel):
    pass

class Code(Label):
    def __init__(self, text, shape, **kwargs):
        super(Code, self).__init__(**kwargs)
        self.shape = shape
        self.text = text
        self.size_hint_max_y = 50

class CodeStack(BoxLayout):
    def __init__(self, **kwargs):
        super(CodeStack, self).__init__(**kwargs)
        self.orientation = 'vertical'
        self.codes = []

    def add_code (self, shape):
        code = Code("l1", shape)
        self.codes.append(code)
        self.add_widget(code)

    def clear(self):
        for code in self.codes:
            self.remove_widget(code)

class FilePopup(Popup):
    def __init__(self, parent,**kwargs):
        super(FilePopup, self).__init__(**kwargs)
        self.load_panel = parent

    def cancel(self):
        self.files = []
        self.dismiss()

    def send_files(self, img):
        self.load_panel.add_shape_from_img_file(img)
        self.dismiss()

class LoadPanel(TabbedPanelItem):

    img_rect = None
    code = None
    frame = None

    def __init__(self, **kwargs):
        super(LoadPanel, self).__init__(**kwargs)
        self.cap = cv2.VideoCapture(0)
        self.update_trigger = Clock.schedule_interval(self.update, 1 / 10.)
        self.in_qr_mode = True
        self.camera_stop = False

    def open_image_select_popup(self):
        self.popup = FilePopup(self)
        self.popup.open()
        return 0

    def start_adding_shape_from_img_file (self):
        return 0

    def add_shape_from_img_file(self, img):
        self.frame = cv2.imread(img[0])
        self.in_qr_mode = False
    
    def add_shape_from_qr(self, code):
        return 0
    
    def add_place_from_qr(self, code):
        return 0

    def reflect_img (self, img):
        img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
        img = cv2.flip(img, 0)
        size = self.ids.load_canvas.size
        pos  = self.ids.load_canvas.pos
        img = cv2.resize(img, (int(size[0]), int(size[1])))
        texture = Texture.create(size=(img.shape[1], img.shape[0]))
        texture.blit_buffer(img.tostring())
        if self.img_rect == None:
            with self.ids.load_canvas.canvas:
                self.img_rect = Rectangle(texture=texture, pos=self.ids.load_canvas.pos, size=self.ids.load_canvas.size)
        else:
            self.img_rect.pos = pos
            self.img_rect.size = size
            self.img_rect.texture = texture

    def update(self, event):
        if self.in_qr_mode:
            #frame = self.get_frame()
            ret, orig = self.cap.read()
            from pyzbar.pyzbar import decode
            from pyzbar.pyzbar import ZBarSymbol
            frame = cv2.cvtColor(orig, cv2.COLOR_RGB2GRAY)
            codes = decode(Image.fromarray(frame), symbols=[ZBarSymbol.QRCODE])
            frame = cv2.cvtColor(frame, cv2.COLOR_GRAY2RGB)
            print(codes)
            if len(codes) > 0 or self.code == None:
                if len(codes) > 0:
                    self.code = codes[0]
                self.frame = orig
            self.reflect_img(self.frame)
                

        

Builder.load_file('ui.kv')

class MainApp(App):
    def build(self):
        return Base()

if __name__ == '__main__':
    from kivy.config import Config
    Config.set('input', 'mouse', 'mouse,disable_multitouch')
    MainApp().run()
