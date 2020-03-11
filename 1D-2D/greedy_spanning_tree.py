#!/usr/bin/env python3.7

# Copyright 2019, Gurobi Optimization, LLC

# Solve a traveling salesman problem on a randomly generated set of
# points using lazy constraints.   The base MIP model only includes
# 'degree-2' constraints, requiring each node to have exactly
# two incident edges.  Solutions to this model may contain subtours -
# tours that don't visit every city.  The lazy constraint callback
# adds new constraints to cut them off.

import sys
import math
import time
import random
import numpy as np
import matplotlib.pyplot as plt
from itertools import combinations
import gurobipy as gp
from gurobipy import GRB



def variance22(matrix, x, y):
    n, m = matrix.shape

    score = 0
    for i in range(n - 1):
        mean = matrix[i][x] + matrix[i][y] + matrix[i + 1][x] + matrix[i + 1][y]
        mean = mean / 4.0
        
        score += (matrix[i][x] - mean) ** 2
        score += (matrix[i][y] - mean) ** 2
        score += (matrix[i + 1][x] - mean) ** 2
        score += (matrix[i + 1][y] - mean) ** 2

    return score 



def distance_matrix_columns(matrix):
    n, m = matrix.shape

    distance_matrix = np.zeros((m, m))
    
    for i in range(m):
        for j in range(m):
            if(i == j):
                distance_matrix[i, j] = 0.0
                continue
            
            distance_matrix[i, j] = variance22(matrix, i, j)

    return m, distance_matrix



def matrix_score(matrix):
    n, m = matrix.shape
    score = 0.0
    for i in range(n - 1):
        for j in range(m - 1):
            mean = matrix[i][j] + matrix[i][j + 1] + matrix[i + 1][j] + matrix[i + 1][j + 1]
            mean = mean / 4.0
            
            score += (matrix[i][j] - mean) ** 2
            score += (matrix[i][j + 1] - mean) ** 2
            score += (matrix[i + 1][j] - mean) ** 2
            score += (matrix[i + 1][j + 1] - mean) ** 2

    return score


def apply_column_permutation(matrix, permutation):
    n, m = matrix.shape

    matrix = matrix[:, np.array(permutation)]
    return matrix


def random_matrix(n, m):
    matrix = np.random.rand(n, m)
    return matrix


def read_matrix_file(file_path):
    f = open(file_path, "r")

    n, m = list(map(int, f.readline().split(" ")))

    matrix = list()
    for i in range(n):
        line = f.readline().split(" ")
#       line.pop()                      # remove newline

        line = list(map(float, line))
        matrix.append(line)

    f.close()

    matrix = np.array(matrix)

    '''
    for i in range(n):
        for j in range(m):
            print(matrix[i][j], end=" ")
        print() 
    '''
    return matrix


# ------------------------------------------------------------------------------


def rooting(root, x):
    if(root[x] == x):
        return x

    root[x] = rooting(root, root[x])
    return root[x]


def greedy_spanning_tree(matrix):
    n, distance_matrix = distance_matrix_columns(matrix)
    
    distances = list()
    for i in range(n):
        for j in range(i):
            distances.append((distance_matrix[i][j], i, j))        

    distances.sort()


    root = [i for i in range(n)]
    path_ends = [1 for i in range(n)]
    path_size = [1 for i in range(n)]
    adjacent = [[] for i in range(n)]


    final_cost = 0.0
    for i in range(len(distances)):
        a = distances[i][1]
        b = distances[i][2]
        cost = distances[i][0]

        ra = rooting(root, a)
        rb = rooting(root, b)

        if(path_ends[a] and path_ends[b] and ra != rb):
            final_cost += cost

            if(path_size[ra] > 1):
                path_ends[a] = 0
            
            if(path_size[rb] > 1):
                path_ends[b] = 0

            path_size[rb] += path_size[ra]
            path_size[ra] = 0
            root[ra] = rb

            adjacent[a].append(b)
            adjacent[b].append(a)
    

    current = 0
    for i in range(n):
        if(path_ends[i]):
            current = i
            break


    prev = -1
    nxt = 0
    permutation = []

    while(nxt != -1):
        permutation.append(current)
        nxt = -1
        for v in adjacent[current]:
            if(v != prev):
                nxt = v

        prev = current
        current = nxt

    return final_cost, np.array(permutation)



def greedy_2D(matrix):
    score = matrix_score(matrix)
    new_score = 0.0

    scores = list()
    scores.append(score)

    while(True):
        new_score, permutation = greedy_spanning_tree(matrix)
        
        print(new_score)

        matrix = apply_column_permutation(matrix, permutation)
       
        matrix = np.transpose(matrix)

        assert new_score - matrix_score(matrix) < 10**-2

        if(score - new_score < 10**-2):
            break
        
        score = new_score
        scores.append(score)

    return scores



if (__name__ == "__main__"):
    #matrix = read_matrix_file("matrix.txt")
    #matrix = read_matrix_file("radiation.csv")
    #matrix = read_matrix_file("brown_selected.txt")
    matrix = random_matrix(50, 50)

    print(greedy_2D(matrix))