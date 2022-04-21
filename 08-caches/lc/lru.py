#!/bin/python

input_list = list(map(int, input().split(" ")))
m = input_list[0]
n = input_list[1]
input_list = input_list[2:]

lru_list = []
lru_hits = 0

for p in input_list:
  if p in lru_list:
    # print(p, end = " ", sep = "")
    lru_list.remove(p)
    lru_list.insert(0, p)
    lru_hits += 1
  else:
    if (len(lru_list) == m):
      lru_list.remove(lru_list[len(lru_list) - 1])
    lru_list.insert(0, p)
    # print("x", end = " ", sep = "")
  
  print(*lru_list)