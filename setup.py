from setuptools import setup, Extension

api_module = Extension('chess_engine', sources=['api.c', 'interface.cpp', 'Board.cpp', 'move_gen.cpp', 'move_gen_t.cpp'], extra_compile_args=['-std=c++20'], language = 'c++')

setup(name='chess_engine', version='1.0', description = "An interface to the C++ chess engine", 
      ext_modules=[api_module])

