import glob
import os.path as osp
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext
import pybind11


ROOT_DIR = osp.dirname(osp.abspath(__file__))
include_dirs = [osp.join(ROOT_DIR, "include")]

sources = glob.glob('*.cpp')

__version__ = "0.0.1"

ext_modules = [
    Pybind11Extension(
        name="mylibrary",
        sources=sources,
        include_dirs=include_dirs,#[pybind11.get_include()],
        # Example: passing in the version to the compiled code
        define_macros=[("VERSION_INFO", __version__)],
    ),
]


setup(
    name='mylibrary',
    version='1.0',
    author='kwea123',
    author_email='kwea123@gmail.com',
    description='mylibrary',
    long_description='mylibrary',
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext}
)