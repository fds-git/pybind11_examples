import mylibrary
import numpy as np
import time
import random
from uuid import uuid4
import json

# print(f"Test 1: {mylibrary.add(40, 2)}")

# qwe = ['a','b','c','d','e']

# print(f"Test 2: {mylibrary.get_random(qwe)}")

# res = mylibrary.multiply([1., 2.])
# print(f"Test 3: {res} {type(res)}")

# res = mylibrary.multiply(np.array([1., 2.]))
# print(f"Test 4: {res} {type(res)}")

# res = mylibrary.vector_as_array(5)
# print(f"Test 5: {res} {type(res)}")

# res = mylibrary.get_dict()
# print(f"Test 6: {res} {type(res)}")

# a = np.array([1,2,3,4,5]*1_000_000, dtype=np.double)
# b = np.array([5,4,3,2,1]*1_000_000, dtype=np.double)
# start = time.time()
# res = mylibrary.add_arrays(a, b)
# print(f"Time: {time.time() - start}")
#print(f"Test 7: {a} + {b} = {res} {type(res)}")

#============================ТЕСТИРОВАНИЕ МАТЧЕРА================================

num_vectors = 3_000_000
num_lists = 1000
N = 1
similarities = np.random.rand(num_vectors)
face_uds_array = np.array([uuid4().hex for i in range(num_vectors)])
face_uids_dict = {i: uuid4().hex for i in range(num_vectors)}
face_indexes = np.array([i for i in range(num_vectors)])
list_ids = np.array([random.randint(0, num_lists-1) for i in range(num_vectors)])
list_ids_to_search = np.array([i for i in range(num_lists)])
max_list_index = np.max(list_ids)
limit = 5

# print("Принимаем массив uid, возвращаем list")
# start = time.time()
# result = mylibrary.match_result(similarities, list_ids, face_uds_array, list_ids_to_search, max_list_index, limit)
# print(f"Total match_result time: {time.time() - start}")
# print("*"*50)

print("Принимаем dict uid, возвращаем list")
timings = []
for i in range(N):
    start = time.time()
    result = mylibrary.match_result_v2(similarities, list_ids, face_uids_dict, face_indexes, list_ids_to_search, max_list_index, limit)
    b_v2 = json.dumps([result[i] for i in list_ids_to_search])
    timings.append(time.time() - start)
    print(f"Total match_result_v2 time: {time.time() - start}")
timings = np.array(timings)
# print(f"max: {timings.max()}")
# print(f"min: {timings.min()}")
# print(f"mean: {timings.mean()}")
# print(f"std: {timings.std()}")
print("*"*50)

print("Принимаем dict uid, возвращаем str")
timings = []
for i in range(N):
    start = time.time()
    result = mylibrary.str_match_result_v2(similarities, face_uids_dict, list_ids, face_indexes, list_ids_to_search, max_list_index, limit)
    timings.append(time.time() - start)
    print(f"Total str_match_result_v2 time: {time.time() - start}")
timings = np.array(timings)
# print(f"max: {timings.max()}")
# print(f"min: {timings.min()}")
# print(f"mean: {timings.mean()}")
# print(f"std: {timings.std()}")
print("*"*50)

#================================================================================


# similarities = np.array([uuid4().hex for i in range(10)])
# print(similarities)
# result = mylibrary.numpy_to_dict(similarities)
# print(result)

# qwe = np.random.rand(10)
# print(qwe)
# mylibrary.inplace_multiply(qwe, 2.)
# print(qwe)

#=======================Массив строк передается по значению==============================
# num_vectors = 3000000
# uids = np.array([uuid4().hex for i in range(num_vectors)])
# start = time.time()
# mylibrary.test_array_uuid(uids)
# print(time.time() - start)
#=====================================================

#=======================Массив чисел передается по ссылке==============================
#num_vectors = 3000000
# doubles = np.random.rand(num_vectors)
# start = time.time()
# mylibrary.test_array_doubles(doubles)
# print(time.time() - start)
#=====================================================