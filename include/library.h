#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <random>
#include <typeinfo>
#include <iostream>
#include <vector>

#pragma once

int add(const int &, const int &);
pybind11::object get_random(const pybind11::list &);
std::vector<int> multiply(const std::vector<double>&);
pybind11::array_t<double> vector_as_array(std::size_t);
pybind11::dict get_dict();
pybind11::array_t<double> add_arrays(pybind11::array_t<double>, pybind11::array_t<double>);
pybind11::dict match_result(pybind11::array_t<double>, pybind11::array_t<double>, pybind11::array_t<std::array<char, 32>>, pybind11::array_t<int>, int, int);
pybind11::dict match_result_v2(pybind11::array_t<double>, pybind11::array_t<double>, pybind11::dict, pybind11::array_t<int>, int, int);
pybind11::dict numpy_to_dict(pybind11::array_t<std::array<char, 32>>);
pybind11::str get_string(void);
void inplace_multiply(pybind11::array_t<double>, double);
void test_array_uuid(pybind11::array_t<std::array<char, 32>>);
void test_array_doubles(pybind11::array_t<double>);