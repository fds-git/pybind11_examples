import json
import random
import time
from statistics import mean
from uuid import uuid4

import mylibrary
import numpy as np

num_vectors = 300_000
num_lists = 1000
similarities = np.random.rand(num_vectors)
face_ids = np.array([uuid4().hex for _ in range(num_vectors)])
face_ids_v2 = {i: v for i, v in enumerate(face_ids)}
face_indexes = np.array([i for i in range(num_vectors)])
list_ids = np.array([random.randint(0, num_lists-1) for _ in range(num_vectors)])
list_ids_to_search = np.array([i for i in range(num_lists)])
max_list_index = np.max(list_ids)
limit = 5

print("String version with array _______________________________________")

time_str = []
for _ in range(1):
    start = time.time()
    a = mylibrary.str_match_result(similarities, face_ids, list_ids, face_indexes, list_ids_to_search, max_list_index, limit)
    end = (time.time() - start) * 1000
    time_str.append(end)
    print(end)
    # print(a)

print("AVG time: ", round(mean(time_str), 7))

print("String version with dict _______________________________________")

time_dict = []
for _ in range(1):
    start = time.time()
    a_v2 = mylibrary.str_match_result_v2(similarities, face_ids_v2, list_ids, face_indexes, list_ids_to_search, max_list_index, limit)
    end = (time.time() - start) * 1000
    time_dict.append(end)
    print(end)
    # print(a_v2)
print("AVG time: ", round(mean(time_dict), 7))

print("Dict version with array _______________________________________")

time_str = []
for _ in range(1):
    start = time.time()
    b = mylibrary.match_result(similarities, list_ids, face_ids, list_ids_to_search, max_list_index, limit)
    b = json.dumps([b[i] for i in list_ids_to_search])
    end = (time.time() - start) * 1000
    time_str.append(end)
    print(end)
    # print(a)

print("AVG time: ", round(mean(time_str), 7))

print("Dict version with dict _______________________________________")

time_dict = []
for _ in range(1):
    start = time.time()
    b_v2 = mylibrary.match_result_v2(similarities, list_ids, face_ids_v2, face_indexes, list_ids_to_search, max_list_index, limit)
    b_v2 = json.dumps([b_v2[i] for i in list_ids_to_search])
    end = (time.time() - start) * 1000
    time_dict.append(end)
    print(end)
    # print(a_v2)
print("AVG time: ", round(mean(time_dict), 7))

print(a == a_v2)
print(b == b_v2)
print(json.loads(a_v2) == json.loads(b_v2))
