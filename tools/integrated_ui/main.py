#!/usr/bin/python3
# -*- codign: utf-8 -*-

from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.tabbedpanel import TabbedPanel
from kivy.uix.widget import Widget
from kivy.uix.label import Label
from kivy.lang import Builder
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
    pass

class CodeStack(BoxLayout):
    pass

Builder.load_file('ui.kv')

class MainApp(App):
    def build(self):
        return Base()

if __name__ == '__main__':
    MainApp().run()
