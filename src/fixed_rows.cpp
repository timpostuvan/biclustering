#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
#include "matrix.h"
#include "fixed_rows.h"

using namespace std;


Solution::Solution(int n, int m, long double cost, vector<vector<long double>> &matrix, vector<int> &permutation){
	this -> n = n;
	this -> m = m;
	this -> cost = cost;
	this -> matrix = matrix;
	this -> permutation = permutation;
}

void Solution::update(long double cost, vector<vector<long double>> &matrix, vector<int> permutation){
	this -> cost = cost;
	this -> matrix = matrix;
	this -> permutation = permutation;
}

void Solution::print(){
	cout << "Size: " << n << " " << m << "\n";
	cout << "Cost: " << cost << "\n";
	cout << "Matrix:\n";
	print_matrix(matrix);
}

Solution& Solution::operator=(const Solution &sol){
	this -> n = sol.n;
	this -> m = sol.m;
	this -> cost = sol.cost;
	this -> matrix = sol.matrix;
	this -> permutation = sol.permutation;
	return *this;
}

bool Solution::operator< (const Solution &sol) const{
	if(sol.cost == -1.0){
		return true;
	}
	if(this -> cost == -1.0){
		return false;
	}
	return this -> cost < sol.cost;
}


void all_permutations(vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function){
	int n = matrix.size();
	int m = matrix[0].size();

	vector<int> permutation;
	for(int i = 0; i < m; i++){
		permutation.push_back(i);
	}

	Solution best_solution = Solution(n, m, -1.0, matrix, permutation);
	do{
		vector<vector<long double>> new_matrix = apply_permutation_columns(matrix, permutation);
		
		long double new_cost = cost_function(new_matrix);
		if(best_solution.cost == -1.0 || best_solution.cost > new_cost){
			best_solution.update(new_cost, new_matrix, permutation);
		}
	} while(next_permutation(permutation.begin(), permutation.end()));

	best_solution.print(); 
}


void random_permutations(int iterations, vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function){
	srand(time(NULL));
	
	int n = matrix.size();
	int m = matrix[0].size();

	vector<int> permutation;
	for(int i = 0; i < m; i++){
		permutation.push_back(i);
	}

	Solution best_solution = Solution(n, m, -1.0, matrix, permutation);
	for(int iter = 0; iter < iterations; iter++){
		random_shuffle(permutation.begin(), permutation.end());
		vector<vector<long double>> new_matrix = apply_permutation_columns(matrix, permutation);
		
		long double new_cost = cost_function(new_matrix);
		if(best_solution.cost == -1.0 || best_solution.cost > new_cost){
			best_solution.update(new_cost, new_matrix, permutation);
		}
	} 

	best_solution.print(); 
}