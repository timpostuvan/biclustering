#!/usr/bin/env python3.7

# Copyright 2020, Gurobi Optimization, LLC

# Solve a traveling salesman problem on a randomly generated set of
# points using lazy constraints.   The base MIP model only includes
# 'degree-2' constraints, requiring each node to have exactly
# two incident edges.  Solutions to this model may contain subtours -
# tours that don't visit every city.  The lazy constraint callback
# adds new constraints to cut them off.

import sys
import math
import random
import numpy as np
from itertools import combinations
import gurobipy as gp
from gurobipy import GRB


def random_matrix(n, m):
    matrix = np.random.rand(n, m)
    return matrix


def read_matrix_file(file_path):
    f = open(file_path, "r")

    n, m = list(map(int, f.readline().split(" ")))

    matrix = list()
    for i in range(n):
        line = f.readline().split(" ")
        line.pop()                      # remove newline

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


def write_matrix_file(file_path, matrix):
    f = open(file_path, "w")
    n, m = matrix.shape
    
    f.write(str(n) + " " + str(m) + "\n")
    for i in range(n):
        for j in range(m):
            f.write(str(matrix[i][j]) + " ")
        f.write("\n")
        
    f.close()
# -----------------------------------------------------------------------------------

def variance22(matrix, n, m, i, j, k, l):
    if(i == n - 1 or j == n - 1 or k == m - 1 or l == m - 1):
        return 0

    mean = matrix[i, k] + matrix[i, l] + matrix[j, k] + matrix[j, l]
    mean /= 4.0

    res = (matrix[i, k] - mean) ** 2
    res += (matrix[i, l] - mean) ** 2
    res += (matrix[j, k] - mean) ** 2
    res += (matrix[j, l] - mean) ** 2

    return res




# Callback - use lazy constraints to eliminate sub-tours
def subtourelim(model, where):
    if where == GRB.Callback.MIPSOL:
        # make a list of edges selected in the solution
        x = model.cbGetSolution(model._x)
        y = model.cbGetSolution(model._y)
        selected_x = gp.tuplelist((i, j) for i, j in model._x.keys()
                                if x[i, j] > 0.5)

        selected_y = gp.tuplelist((i, j) for i, j in model._y.keys()
                                if y[i, j] > 0.5)

        # find the shortest cycle in the selected edge list
        tour_x = subtour(rows, selected_x)
        if len(tour_x) < rows:
            # add subtour elimination constr. for every pair of cities in tour
            model.cbLazy(gp.quicksum(model._x[i, j]
                                     for i, j in combinations(tour_x, 2))
                         <= len(tour_x)-1)

        tour_y = subtour(columns, selected_y)
        if len(tour_y) < columns:
            # add subtour elimination constr. for every pair of cities in tour
            model.cbLazy(gp.quicksum(model._y[i, j]
                                     for i, j in combinations(tour_y, 2))
                         <= len(tour_y)-1)


# Given a tuplelist of edges, find the shortest subtour

def subtour(num_nodes, edges):
    unvisited = list(range(num_nodes))
    cycle = range(num_nodes+1)      # initial length has 1 more city
    while unvisited:                # true if list is non-empty
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


# Random matrix of size rows * columns
rows = int(sys.argv[1])
columns = int(sys.argv[2])
matrix = np.random.rand(rows, columns)
write_matrix_file("matrix.txt", matrix)



#matrix = read_matrix_file("matrix.txt")
#matrix = read_matrix_file("optimized_matrix.txt")
rows, columns = matrix.shape


# Add one dummy node per dimension to create cycle
rows += 1
columns += 1


# Cost for all 2 * 2 submatrices
cost = {(i, j, k, l): variance22(matrix, rows, columns, i, j, k, l)
        for i in range(rows) for j in range(i) for k in range(columns) for l in range(k)}

m = gp.Model("Biclustering")
x_keys = [(i, j) for i in range(rows) for j in range(i)]
y_keys = [(i, j) for i in range(columns) for j in range(i)]
#x = m.addVars(x_keys, vtype=GRB.BINARY, name='rows')
#y = m.addVars(y_keys, vtype=GRB.BINARY, name='columns')

x = m.addVars(x_keys, vtype=GRB.CONTINUOUS, ub = 1.0, lb = 0.0, name='rows')
y = m.addVars(y_keys, vtype=GRB.CONTINUOUS, ub = 1.0, lb = 0.0, name='columns')


for i, j in x.keys():
    x[j, i] = x[i, j]

for i, j in y.keys():
    y[j, i] = y[i, j]


m.setObjective(gp.quicksum([cost[i, j, k, l] * ((x[i, j] + y[k, l]) / 2.0) for i, j, k, l in cost.keys()]))


# Add degree-2 constraint
m.addConstrs(x.sum(i, '*') == 2 for i in range(rows))
m.addConstrs(y.sum(i, '*') == 2 for i in range(columns))


"""
# Starting solution
for i in range(rows):
    x[i, (i + 1) % rows].start = 1.0

for i in range(columns):
    y[i, (i + 1) % columns].start = 1.0
"""


# Optimize model
m._x = x
m._y = y
m.Params.lazyConstraints = 1
m.optimize(subtourelim)


sol_x = m.getAttr('X', x)
sol_y = m.getAttr('X', y)

selected_x = gp.tuplelist((i, j) for i, j in sol_x.keys() if sol_x[i, j] > 0.5)
selected_y = gp.tuplelist((i, j) for i, j in sol_y.keys() if sol_y[i, j] > 0.5)

tour_x = subtour(rows, selected_x)
tour_y = subtour(columns, selected_y)

"""
assert len(tour_x) == rows
assert len(tour_y) == columns
"""

print('')
print('Optimal tour rows: %s' % str(tour_x))
print('Optimal tour columns: %s' % str(tour_y))
print('Optimal cost: %g' % m.objVal)
print('')