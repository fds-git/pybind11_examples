#include "library.h"
#include "string_json.h"

namespace py = pybind11;


PYBIND11_MODULE(mylibrary, m) {
    m.def("add", &add);
    m.def("get_random", &get_random);
    m.def("multiply", &multiply);
    m.def("vector_as_array", &vector_as_array, "Generate numpy array of size");
    m.def("get_dict", &get_dict, "Get Python Dict");
    m.def("add_arrays", &add_arrays, "Add two NumPy arrays");
    m.def("match_result", &match_result, "Matcher");
    m.def("match_result_v2", &match_result_v2, "Matcher using dict for uids");
    m.def("numpy_to_dict", &numpy_to_dict, "Numpy array to dict");
    m.def("get_string", &get_string, "Get string");
    m.def("inplace_multiply", &inplace_multiply, "Check that array of doubles transfers by refence");
    m.def("test_array_uuid", &test_array_uuid, "Check that array of str transfers by value");
    m.def("test_array_doubles", &test_array_doubles, "Check that array of doubles transfers by refence");
    m.def("str_match_result", &str_match_result, "Str matcher");
    m.def("str_match_result_v2", &str_match_result_v2, "Str matcher V2");
};
