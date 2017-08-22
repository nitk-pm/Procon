import sys
from cx_Freeze import setup, Executable

base = None

if sys.platform == 'win32':
    base = 'Win32GUI'

options = {
    'build_exe': {
        'excludes': [
            'logging',
            'email',
            'html',
            'http',
            'pydoc_data',
            'unittest',
            'urllib',
            'xml'
        ]
    }
}

setup(
    name='sample',
    version='1.0',
    description='converter',
    options=options,
    executables=[Executable('main.py', base=base)]
)
