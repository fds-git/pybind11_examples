import mylibrary
import numpy as np
import time
import random
from uuid import uuid4

print(f"Test 1: {mylibrary.add(40, 2)}")

qwe = ['a','b','c','d','e']

print(f"Test 2: {mylibrary.get_random(qwe)}")

res = mylibrary.multiply([1., 2.])
print(f"Test 3: {res} {type(res)}")

res = mylibrary.multiply(np.array([1., 2.]))
print(f"Test 4: {res} {type(res)}")

res = mylibrary.vector_as_array(5)
print(f"Test 5: {res} {type(res)}")

res = mylibrary.get_dict()
print(f"Test 6: {res} {type(res)}")

a = np.array([1,2,3,4,5]*1_000_000, dtype=np.double)
b = np.array([5,4,3,2,1]*1_000_000, dtype=np.double)
start = time.time()
res = mylibrary.add_arrays(a, b)
print(f"Time: {time.time() - start}")
#print(f"Test 7: {a} + {b} = {res} {type(res)}")

#============================ТЕСТИРОВАНИЕ МАТЧЕРА================================

# num_vectors = 300000
# num_lists = 1000
# similarities = np.random.rand(num_vectors)
# #face_ids = np.array([uuid4().hex for i in range(num_vectors)])
# face_ids = np.array([i for i in range(num_vectors)])
# list_ids = np.array([random.randint(0, num_lists-1) for i in range(num_vectors)])
# list_ids_to_rearch = np.array([i for i in range(num_lists)])
# max_list_index = np.max(list_ids)
# limit = 5

# #print(face_ids)
# start = time.time()
# result = mylibrary.match_result(similarities, list_ids, face_ids, list_ids_to_rearch, max_list_index, limit)
# print(f"Total time: {time.time() - start}")
# print(result)

#similarities = np.random.rand(10)

similarities = np.array([uuid4().hex for i in range(10)])
print(similarities)
result = mylibrary.numpy_to_dict(similarities)
print(result)