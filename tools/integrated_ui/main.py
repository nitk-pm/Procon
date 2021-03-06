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
from kivy.graphics import Line
from kivy.graphics.texture import Texture
from kivy.clock import Clock
from PIL import Image
from enum import Enum
import cv2
import conv
import json
import os

# カメラ(一度に複数開けないので)
cap = cv2.VideoCapture(0)

class ResultCanvas(Widget):
    should_draw_shapes = []
    all_shapes = []
    draw_shape_num = 0

    def __init__(self, **kwargs):
        super(ResultCanvas, self).__init__(**kwargs)

    def set_shape(self, shapes):
        self.all_shapes = shapes

    def next(self):
        if self.draw_shape_num < len(self.all_shapes):
            self.draw_shape_num += 1
            self.should_draw_shapes = self.all_shapes[0:self.draw_shape_num]
        self.update_lines()

    def previous(self):
        if self.draw_shape_num > 0:
            self.draw_shape_num -= 1
            self.should_draw_shapes = self.all_shapes[0:self.draw_shape_num]
        self.update_lines()

    def update_lines(self):
        self.canvas.clear()
        self.canvas.add(Color(1,0,0,1))
        for line in [self.shape2line(shape) for shape in self.should_draw_shapes]:
            self.canvas.add(line)

    def shape2line(self, shape):
        off_x = self.pos[0] + 10
        off_y = self.pos[1] + 10
        # 101x65 -> キャンバスへの縮尺変換
        # 横方向の比率に合わせる
        ratio = float(self.size[0] - 10) / 101.0
        pts = []
        for pt in shape:
            pts.extend([pt[0]*ratio+off_x, pt[1]*ratio+off_y])
        print([int(x) for x in pts])
        return Line(points=pts, width=2, close=True)


class PieceSuggestCanvas(Widget):
    pass

class SolverCanvas(Widget):
    pass

class LoadCanvas(Widget):
    pass

class Base(TabbedPanel):
    pass

class Code(Label):
    shape = []
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

    def add_code (self, name, shape):
        code = Code(name, shape)
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

class Mode(Enum):
    ShapeQR = 0
    PlaceQR = 1
    ShpaeCV = 2
    NoWork  = 3

class ResultPanel(TabbedPanelItem):
    def __init__(self, **kwargs):
        super (ResultPanel, self).__init__(**kwargs)

    def camera_open(self, id):
        cap = cv2.VideoCapture(id)

    def reload(self):
        if os.path.exists('./test.txt'):
            self.ids.canvas.set_shape(conv.load_result(open('./test.txt', 'r')))
            self.ids.previous_button.disabled = False
            self.ids.next_button.disabled = False

    def set_camera(self, id):
        cap = cv2.VideoCapture(id)

class LoadPanel(TabbedPanelItem):
    img_rect = None
    code = None
    frame = None
    mode = Mode.NoWork

    def __init__(self, **kwargs):
        super(LoadPanel, self).__init__(**kwargs)
        cap = cv2.VideoCapture(0)
        self.update_trigger = Clock.schedule_interval(self.update, 1 / 10.)
        self.camera_stop = False

    def set_camera(self, id):
        cap = cv2.VideoCapture(id)

    def open_image_select_popup(self):
        self.popup = FilePopup(self)
        self.popup.open()
        return 0

    def start_adding_shape_from_img_file (self):
        return 0

    def add_shape_from_img_file(self, img):
        self.frame = cv2.imread(img[0])
    
    def add_shape_from_qr(self):
        self.mode = Mode.ShapeQR
    
    def add_place_from_qr(self):
        self.mode = Mode.PlaceQR

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
        if self.mode == Mode.ShapeQR or self.mode == Mode.PlaceQR:
            ret, orig = cap.read()
            from pyzbar.pyzbar import decode
            from pyzbar.pyzbar import ZBarSymbol
            frame = cv2.cvtColor(orig, cv2.COLOR_RGB2GRAY)
            codes = decode(Image.fromarray(frame), symbols=[ZBarSymbol.QRCODE])
            frame = cv2.cvtColor(frame, cv2.COLOR_GRAY2RGB)
            if len(codes) > 0 or self.code == None:
                if len(codes) > 0:
                    self.code = codes[0]
                    self.ids.ok.disabled = False
                    self.ids.reject.disabled = False
                self.frame = orig
            self.reflect_img(self.frame)

    def ok (self):
        def add_code(stack):
            stack.add_code('QR{}'.format(len(stack.children)+1), self.code)
        if self.mode == Mode.ShapeQR:
            add_code(self.ids.shape_qr_stack)
        if self.mode == Mode.PlaceQR:
            add_code(self.ids.place_qr_stack)
        self.mode = Mode.NoWork
        self.code = None
        self.ids.ok.disabled = True
        self.ids.reject.disabled = True

    def reject(self):
        self.mode = Mode.NoWork
        self.code = None
        self.ids.ok.disabled = True
        self.ids.reject.disabled = True

    def export(self):
        shape_codes = [code.shape.data for code in self.ids.shape_qr_stack.children]
        place_codes = [code.shape.data for code in self.ids.place_qr_stack.children]
        if len(shape_codes) != 0:
            conv.dump_shape_json(shape_codes)
        if len(place_codes) != 0:
            conv.dump_place_json(place_codes)


Builder.load_file('ui.kv')

class MainApp(App):
    def build(self):
        return Base()

if __name__ == '__main__':
    from kivy.config import Config
    Config.set('input', 'mouse', 'mouse,disable_multitouch')
    MainApp().run()
