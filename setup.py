import glob
from pathlib import Path

from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

ROOT_DIR = Path(__file__).parent
LIBS_DIR =  ROOT_DIR.joinpath("libs")
include_dirs = [ROOT_DIR.joinpath("include")]
include_dirs.extend(list(lib.joinpath("include") for lib in LIBS_DIR.iterdir()))

sources = glob.glob('*.cpp')

__version__ = "0.0.1"

ext_modules = [
    Pybind11Extension(
        name="mylibrary",
        sources=sources,
        include_dirs=include_dirs, #[pybind11.get_include()], Example: passing in the version to the compiled code
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
