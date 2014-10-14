from distutils.core import setup, Extension

modulel = Extension('spam',
        sources = ['spammodule.c'])

setup (name = 'PackageName',
        version = '1.0',
        description = 'This is a demo package',
        ext_modules = [modulel])
