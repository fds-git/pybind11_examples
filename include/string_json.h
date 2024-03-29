#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <random>
#include <typeinfo>
#include <iostream>
#include <vector>

#pragma once


std::string str_match_result(pybind11::array_t<double>, pybind11::array_t<std::array<char, 32>>, pybind11::array_t<int>, pybind11::array_t<int>, pybind11::array_t<int>, int, int);
std::string str_match_result_v2(pybind11::array_t<double>, pybind11::dict, pybind11::array_t<int>, pybind11::array_t<int>, pybind11::array_t<int>, int, int);
