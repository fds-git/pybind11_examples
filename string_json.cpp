#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <chrono>

namespace py = pybind11;
using json = nlohmann::json;


std::string str_match_result(
    py::array_t<double> similarities,
    py::array_t<std::array<char, 32>> face_ids,
    py::array_t<int> list_ids,
    py::array_t<int> face_indexes,
    py::array_t<int> list_ids_to_search,
    int max_list_index,
    int limit)
{
    // Request a py::buffer_info object, which contains detailed information about the buffer underlying the NumPy array.
    py::buffer_info similarities_buf = similarities.request();
    py::buffer_info list_ids_buf = list_ids.request();
    py::buffer_info face_ids_buf = face_ids.request();
    py::buffer_info face_indexes_buf = face_indexes.request();
    py::buffer_info list_ids_to_search_buf = list_ids_to_search.request();

    if (similarities_buf.ndim != 1 || list_ids_buf.ndim != 1 || face_ids_buf.ndim != 1 || face_indexes_buf.ndim != 1 || list_ids_to_search_buf.ndim != 1)
        throw std::runtime_error("Number of dimensions must be one");

    if ((similarities_buf.size != list_ids_buf.size) || (similarities_buf.size != face_ids_buf.size) || (similarities_buf.size != face_indexes_buf.size))
        throw std::runtime_error("Input shapes must be the same");

    double *similarities_ptr = static_cast<double *>(similarities_buf.ptr);
    int *list_ids_ptr = static_cast<int *>(list_ids_buf.ptr);
    int *face_indexes_ptr = static_cast<int *>(face_indexes_buf.ptr);
    int *list_ids_to_search_ptr = static_cast<int *>(list_ids_to_search_buf.ptr);
    auto uids_array = face_ids.unchecked<1>();

    //Формируем вспомогательный массив
    std::vector<int> helper(max_list_index, 0);

    //Заполняем его значениями limit в тех ячейках, индекс которых
    //равен значениям из list_ids_to_rearch
    for(int i = 0; i < list_ids_to_search_buf.shape[0]; ++i) {
        helper[list_ids_to_search_ptr[i]] = limit;
    }

    //Формируем результирующий json
    std::ostringstream json_string;
    json_string << "[";

    for (long int i = 0; i < similarities_buf.shape[0]; ++i)
    {
        int list_idx = list_ids_ptr[i];
        int face_idx = face_indexes_ptr[i];
        if (helper[list_idx]) {
            helper[list_idx]--;

            std::string face_id;
            face_id.reserve(32);
            for (char c : uids_array(face_idx)) {
                face_id += c;
            }

            double similarity = similarities_ptr[i];
            if (i > 0) {
                json_string << ",";
            }
            json_string << "{\"face_id\": \"" << face_id << "\", \"similarity\": " << similarity << "}";
        }
        else {
            continue;
        }
    }
    json_string << "]";

    return json_string.str();
}


std::string str_match_result_v2(
    py::array_t<double> similarities,
    py::dict face_ids,
    py::array_t<int> list_ids,
    py::array_t<int> face_indexes,
    py::array_t<int> list_ids_to_search,
    int max_list_index,
    int limit)
{
    // Request a py::buffer_info object, which contains detailed information about the buffer underlying the NumPy array.
    auto start = std::chrono::high_resolution_clock::now();
    py::buffer_info similarities_buf = similarities.request();
    py::buffer_info list_ids_buf = list_ids.request();
    py::buffer_info face_indexes_buf = face_indexes.request();
    py::buffer_info list_ids_to_search_buf = list_ids_to_search.request();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Получение буфера " << duration.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    if (similarities_buf.ndim != 1 || list_ids_buf.ndim != 1 || face_indexes_buf.ndim != 1 || list_ids_to_search_buf.ndim != 1)
        throw std::runtime_error("Number of dimensions must be one");

    if ((similarities_buf.size != list_ids_buf.size) || (similarities_buf.size != face_indexes_buf.size))
        throw std::runtime_error("Input shapes must be the same");
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Проверка размерности " << duration.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    double *similarities_ptr = static_cast<double *>(similarities_buf.ptr);
    int *list_ids_ptr = static_cast<int *>(list_ids_buf.ptr);
    int *face_indexes_ptr = static_cast<int *>(face_indexes_buf.ptr);
    int *list_ids_to_search_ptr = static_cast<int *>(list_ids_to_search_buf.ptr);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Получение указателей " << duration.count() << std::endl;

    // Формируем результирующий словарь
    start = std::chrono::high_resolution_clock::now();
    std::unordered_map<int, std::vector<std::pair<py::str, double>>> result_map;
    for (long int i = 0; i < similarities_buf.shape[0]; ++i)
    {
        int list_idx = list_ids_ptr[i];
        int face_idx = face_indexes_ptr[i];
        if (result_map[list_idx].size() < limit)
        {
            py::str face_id = face_ids[py::int_{face_idx}];
            double similarity = similarities_ptr[i];
            result_map[list_idx].push_back(std::make_pair(face_id, similarity));
        }
    }
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Формироваине словаря " << duration.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    // Формируем JSON из словаря
    json response;
    for (long int i = 0; i < list_ids_to_search_buf.shape[0]; ++i)
    {
        int list_id = list_ids_to_search_ptr[i];
        auto iter = result_map.find(list_id);
        if (iter != result_map.end()) // if there is no key in map - end
        {
            json list_data;
            for (const auto &value : iter->second)
            {
                list_data.push_back({{"id", value.first}, {"similarity", value.second}});
            }
            response.push_back(list_data);
        }
    }
    std::string json_str = response.dump();

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Формирование json из словаря " << duration.count() << std::endl;

    return json_str;
}


std::string old_str_match_result_v2(
    py::array_t<double> similarities,
    py::dict face_ids,
    py::array_t<int> list_ids,
    py::array_t<int> face_indexes,
    py::array_t<int> list_ids_to_search,
    int max_list_index,
    int limit)
{
    // Request a py::buffer_info object, which contains detailed information about the buffer underlying the NumPy array.
    py::buffer_info similarities_buf = similarities.request();
    py::buffer_info list_ids_buf = list_ids.request();
    py::buffer_info face_indexes_buf = face_indexes.request();
    py::buffer_info list_ids_to_search_buf = list_ids_to_search.request();

    if (similarities_buf.ndim != 1 || list_ids_buf.ndim != 1 || face_indexes_buf.ndim != 1 || list_ids_to_search_buf.ndim != 1)
        throw std::runtime_error("Number of dimensions must be one");

    if (similarities_buf.size != list_ids_buf.size)
        throw std::runtime_error("Input shapes must be the same");

    double *similarities_ptr = static_cast<double *>(similarities_buf.ptr);
    int *list_ids_ptr = static_cast<int *>(list_ids_buf.ptr);
    int *face_indexes_ptr = static_cast<int *>(face_indexes_buf.ptr);
    int *list_ids_to_search_ptr = static_cast<int *>(list_ids_to_search_buf.ptr);

    //Формируем вспомогательный массив
    std::vector<int> helper(max_list_index, 0);

    //Заполняем его значениями limit в тех ячейках, индекс которых
    //равен значениям из list_ids_to_rearch
    for(int i = 0; i < list_ids_to_search_buf.shape[0]; ++i) {
        helper[list_ids_to_search_ptr[i]] = limit;
    }

    //Формируем результирующий json
    std::ostringstream json_string;
    json_string << "[";

    for (long int i = 0; i < similarities_buf.shape[0]; ++i)
    {
        int list_idx = list_ids_ptr[i];
        int face_idx = face_indexes_ptr[i];
        if (helper[list_idx]) {
            helper[list_idx]--;
            py::str face_id = face_ids[pybind11::int_{face_idx}];
            double similarity = similarities_ptr[i];
            if (i > 0) {
                json_string << ",";
            }
            json_string << "{\"face_id\": \"" << face_id << "\", \"similarity\": " << similarity << "}";
        }
        else {
            continue;
        }
    }
    json_string << "]";

    return json_string.str();
}
