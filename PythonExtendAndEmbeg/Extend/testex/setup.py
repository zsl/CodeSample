from distutils.core import setup, Extension

modulel = Extension('testex',
        sources = ['testex.c', 'testex_module.c'])

setup (name = 'PackageName',
        version = '1.0',
        description = 'This is a demo package',
        ext_modules = [modulel])
