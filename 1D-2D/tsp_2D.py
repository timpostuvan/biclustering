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

    distance_matrix = np.zeros((m + 1, m + 1))
    
    for i in range(m):
        for j in range(m):
            if(i == j):
                distance_matrix[i, j] = 0.0
                continue
            
            distance_matrix[i, j] = variance22(matrix, i, j)



    # add dummy node in order to search for a cycle instead of a path

    return m + 1, distance_matrix



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

    start = 0
    for i in range(m + 1):
        if(permutation[i] == m):
            start = i
            break

    new_permutation = permutation[start:] + permutation[:start]
    del new_permutation[0]

    print(permutation)
    print(new_permutation)

    matrix = matrix[:, np.array(new_permutation)]
    return matrix


# Callback - use lazy constraints to eliminate sub-tours
def subtourelim(model, where):
    if where == GRB.Callback.MIPSOL:
        # make a list of edges selected in the solution
        vals = model.cbGetSolution(model._vars)
        selected = gp.tuplelist((i, j) for i, j in model._vars.keys()
                                if vals[i, j] > 0.5)
        # find the shortest cycle in the selected edge list
        tour = subtour(model._n, selected)
        if len(tour) < model._n:
            # add subtour elimination constr. for every pair of cities in tour
            model.cbLazy(gp.quicksum(model._vars[i, j]
                                     for i, j in combinations(tour, 2))
                         <= len(tour)-1)


# Given a tuplelist of edges, find the shortest subtour

def subtour(n, edges):
    unvisited = list(range(n))
    cycle = range(n+1)  # initial length has 1 more city
    while unvisited:  # true if list is non-empty
        thiscycle = []
        neighbors = unvisited
        while neighbors:
            current = neighbors[0]
            thiscycle.append(current)
            unvisited.remove(current)
            neighbors = [j for i, j in edges.select(current, '*')
                         if j in unvisited]
        if len(cycle) > len(thiscycle):
            cycle = thiscycle
    return cycle



def random_permutation_matrix(matrix):
    n, m = matrix.shape

    permutation_rows =  np.random.permutation(n)
    permutation_columns = np.random.permutation(m)

    matrix = matrix[:, permutation_columns]
    matrix = matrix[permutation_rows, :]

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



def TSP_2D(matrix, iterations):
    best_score = math.inf
    best_scores = list()

    for i in range(iterations):
        matrix = random_permutation_matrix(matrix)

        score = matrix_score(matrix)
        new_score = 0.0

        scores = list()
        scores.append(score)

        while(True):
            n, distance_matrix = distance_matrix_columns(matrix)
            dist = {(i, j): distance_matrix[i, j] for i in range(n) for j in range(i)}
            
            m = gp.Model()
            # Create variables

            vars = m.addVars(dist.keys(), obj=dist, vtype=GRB.BINARY, name='e')
            for i, j in vars.keys():
                vars[j, i] = vars[i, j]  # edge in opposite direction

            # You could use Python looping constructs and m.addVar() to create
            # these decision variables instead.  The following would be equivalent
            # to the preceding m.addVars() call...
            #
            # vars = tupledict()
            # for i,j in dist.keys():
            #   vars[i,j] = m.addVar(obj=dist[i,j], vtype=GRB.BINARY,
            #                        name='e[%d,%d]'%(i,j))


            # Add degree-2 constraint

            m.addConstrs(vars.sum(i, '*') == 2 for i in range(n))

            # Using Python looping constructs, the preceding would be...
            #
            # for i in range(n):
            #   m.addConstr(sum(vars[i,j] for j in range(n)) == 2)


            # Optimize model
            m._n = n
            m._vars = vars
            m.Params.lazyConstraints = 1
            m.optimize(subtourelim)


            vals = m.getAttr('x', vars)
            selected = gp.tuplelist((i, j) for i, j in vals.keys() if vals[i, j] > 0.5)

            tour = subtour(n, selected)
            assert len(tour) == n


            print('')
            print('Optimal tour: %s' % str(tour))
            print('Optimal cost: %g' % m.objVal)
            print('')

            new_score = m.objVal
            
            matrix = apply_column_permutation(matrix, tour)

            matrix = np.transpose(matrix)

            if(score - new_score < 10**-2):
                break
            
            score = new_score
            scores.append(score)


        if(min(scores) < best_score):
            best_score = min(scores)
            best_scores = scores


    return best_scores



if (__name__ == "__main__"):
    #matrix = read_matrix_file("matrix.txt")
    #matrix = read_matrix_file("radiation.csv")
    #matrix = read_matrix_file("brown_selected.txt")
    matrix = random_matrix(10, 10)


    print(TSP_2D(matrix))