#include "library.h"
#include <iostream>
#include <string>
#include <array>

#define NPY_NO_DEPRECATED_API NPY_1_10_API_VERSION

int add(const int &a, const int &b) {
    return a + b;
}

pybind11::object get_random(const pybind11::list &items) {
    if (items.size()) {
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution uint_dist(0, static_cast<int>(items.size()) - 1);
        return items[uint_dist(rng)];
    } else {
        return pybind11::none();
    }
}

std::vector<int> multiply(const std::vector<double>& input)
{
  std::vector<int> output(input.size());

  for ( size_t i = 0 ; i < input.size() ; ++i )
    output[i] = 10*static_cast<int>(input[i]);

  return output;
}

pybind11::array_t<double> vector_as_array(std::size_t size) {
    auto temp_vector = std::vector<double>(size);
    return pybind11::array(size, temp_vector.data());
}

pybind11::dict get_dict(void) {
    pybind11::dict dict;
    dict[pybind11::str("key")] = pybind11::str("value");
    return dict;
}

pybind11::array_t<double> add_arrays(pybind11::array_t<double> input1, pybind11::array_t<double> input2) {
    pybind11::buffer_info buf1 = input1.request(), buf2 = input2.request();

    if (buf1.ndim != 1 || buf2.ndim != 1)
        throw std::runtime_error("Number of dimensions must be one");

    if (buf1.size != buf2.size)
        throw std::runtime_error("Input shapes must match");

    /* No pointer is passed, so NumPy will allocate the buffer */
    auto result = pybind11::array_t<double>(buf1.size);

    pybind11::buffer_info buf3 = result.request();

    double *ptr1 = static_cast<double *>(buf1.ptr);
    double *ptr2 = static_cast<double *>(buf2.ptr);
    double *ptr3 = static_cast<double *>(buf3.ptr);

    for (size_t idx = 0; idx < buf1.shape[0]; idx++)
        ptr3[idx] = ptr1[idx] + ptr2[idx];

    return result;
}

pybind11::dict match_result(pybind11::array_t<double> similarities, pybind11::array_t<double> list_ids, 
                            pybind11::array_t<int> face_ids, pybind11::array_t<int> list_ids_to_search, 
                            int max_list_index, int limit)
{
    pybind11::buffer_info similarities_buf = similarities.request();
    pybind11::buffer_info list_ids_buf = list_ids.request();
    pybind11::buffer_info face_ids_buf = face_ids.request();
    pybind11::buffer_info list_ids_to_search_buf = list_ids_to_search.request();

    if (similarities_buf.ndim != 1 || list_ids_buf.ndim != 1 || face_ids_buf.ndim != 1 || list_ids_to_search_buf.ndim != 1)
        throw std::runtime_error("Number of dimensions must be one");

    if ((similarities_buf.size != list_ids_buf.size) || (similarities_buf.size != face_ids_buf.size))
        throw std::runtime_error("Input shapes must match");

    double *similarities_ptr = static_cast<double *>(similarities_buf.ptr);
    double *list_ids_ptr = static_cast<double *>(list_ids_buf.ptr);
    int *face_ids_ptr = static_cast<int *>(face_ids_buf.ptr); //Разобраться, как сделать через
    int *list_ids_to_search_ptr = static_cast<int *>(list_ids_to_search_buf.ptr);

    //Формируем вспомогательный массив
    int *helper = new int [max_list_index];

    //Заполняем его значениями limit в тех ячейках, индекс которых
    //равен значениям из list_ids_to_rearch
    for(int i = 0; i < list_ids_to_search_buf.shape[0]; ++i) {
        helper[list_ids_to_search_ptr[i]] = limit;
    }

    // Формируем итоговую структуру Dict[List] (пустую)
    // ключ - индекс листа из list_ids_to_search
    // значение - python List (пустой).
    pybind11::dict result;
    for(int i = 0; i < list_ids_to_search_buf.shape[0]; ++i) {
        pybind11::list new_list;
        result[pybind11::int_{list_ids_to_search_ptr[i]}] = new_list;
    }

    // Заполняем итоговую структуру
    for (long int i = 0; i < similarities_buf.shape[0]; ++i)
    {
        int list_idx = list_ids_ptr[i];
        if (helper[list_idx]) {
            helper[list_idx]--;
            pybind11::dict item_result;
            item_result["id"] = face_ids_ptr[i];
            item_result["similarity"] = similarities_ptr[i];
            pybind11::list(result[pybind11::int_{list_idx}]).append(item_result); 
        }
        else {
            continue;
        }
    }
    delete(helper);
    return result;
}

pybind11::dict numpy_to_dict(pybind11::array_t<std::array<char, 32>> array)
{
    pybind11::dict result;
    auto r = array.unchecked<1>();
    for (pybind11::ssize_t i = 0; i < r.shape(0); i++) {
        result[pybind11::int_{i}] = r(i);
    }

    // pybind11::buffer_info buf = array.request();
    // double *ptr = static_cast<double *>(buf.ptr);
    // auto ptr = static_cast<char[32]*>(buf.ptr);

    
    


    return result;
}


//Сделать умножение на число через векторизацию по ссылке
// pybind11::dict get_doubled_numpy(void) {
//     pybind11::dict dict;
//     dict[pybind11::str("key")] = pybind11::str("value");
//     return dict;
// }
