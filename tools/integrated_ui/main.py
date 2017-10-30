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
        self.color = [0,0,0,1]

    def on_size(self, *args):
        self.canvas.before.clear()
        with self.canvas.before:
            Color(1, 1, 1, 1)
            Rectangle(pos=self.pos, size=self.size)
        

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
        print(parent)
        self.load_panel = parent

    def cancel(self):
        self.files = []
        self.dismiss()

    def send_files(self, files):
        self.load_panel.conv(files)
        self.dismiss()

class LoadPanel(TabbedPanelItem):
    def __init__(self, **kwargs):
        super(LoadPanel, self).__init__(**kwargs)

    def add_image(self):
        self.popup = FilePopup(self)
        self.popup.open()
        return 0

    def conv(self, files):
        self.ids.shape_image_stack.add_code([])
    
    def add_shape(self):
        return 0
    
    def add_place(self):
        return 0

Builder.load_file('ui.kv')

class MainApp(App):
    def build(self):
        return Base()

if __name__ == '__main__':
    MainApp().run()
