#include "library.h"

namespace py = pybind11;


PYBIND11_MODULE(mylibrary, m) {
    m.def("add", &add);
    m.def("get_random", &get_random);
    m.def("multiply", &multiply);
    m.def("vector_as_array", &vector_as_array, "Generate numpy array of size");
    m.def("get_dict", &get_dict, "Get Python Dict");
    m.def("add_arrays", &add_arrays, "Add two NumPy arrays");
    m.def("match_result", &match_result, "Matcher");
    m.def("numpy_to_dict", &numpy_to_dict, "Numpy array to dict");
};