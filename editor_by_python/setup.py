import sys
from cx_Freeze import setup, Executable

base = None

if sys.platform == 'win32':
    base = 'Win32GUI'

setup(
    name='sample',
    version='1.0',
    description='converter',
    executables=[Executable('main.py', base=base)]
)