#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
#include <tuple>
#include <random>
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

void Solution::update(long double cost, vector<int> permutation){
	this -> cost = cost;
	this -> permutation = permutation;
}

void Solution::apply(vector<vector<long double>> &matrix){
	this -> matrix = apply_permutation_columns(matrix, this -> permutation);
}

void Solution::print(bool output_matrix){
	cout << "Size: " << n << " " << m << "\n";
	cout << "Cost: " << cost << "\n";

	if(output_matrix){
		cout << "Matrix:\n";
		print_matrix(matrix);
	}
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

long double lower_bound(vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function){
	int n = matrix.size();
	int m = matrix[0].size();

	vector<vector<long double>> dist_matrix = distance_matrix_columns(matrix, cost_function);

	long double cost = 0.0;
	long double max_value = 0.0;
	for(int i = 0; i < m; i++){
		long double min_value = -1.0;
		for(int j = 0; j < m; j++){
			if(i != j){
				if(min_value == -1.0 || dist_matrix[i][j] < min_value){
					min_value = dist_matrix[i][j];
				}
			}
		}
		cost += min_value;
		max_value = max(max_value, min_value);
	}

	cost -= max_value;
	return cost;
}


Solution all_permutations(vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function){
	int n = matrix.size();
	int m = matrix[0].size();

	vector<vector<long double>> dist_matrix = distance_matrix_columns(matrix, cost_function);

	vector<int> permutation;
	for(int i = 0; i < m; i++){
		permutation.push_back(i);
	}

	Solution best_solution = Solution(n, m, -1.0, matrix, permutation);
	do{
		long double new_cost = 0.0;
		for(int i = 0; i < m - 1; i++){
			new_cost += dist_matrix[permutation[i]][permutation[i + 1]];
		}

		if(best_solution.cost == -1.0 || best_solution.cost > new_cost){
			best_solution.update(new_cost, permutation);
		}
	} while(next_permutation(permutation.begin(), permutation.end()));

	best_solution.apply(matrix);
	best_solution.print(0); 
	return best_solution;
}


Solution random_permutations(int iterations, vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function){
	srand(time(NULL));
	
	int n = matrix.size();
	int m = matrix[0].size();

	vector<vector<long double>> dist_matrix = distance_matrix_columns(matrix, cost_function);

	vector<int> permutation;
	for(int i = 0; i < m; i++){
		permutation.push_back(i);
	}

	Solution best_solution = Solution(n, m, -1.0, matrix, permutation);
	for(int iter = 0; iter < iterations; iter++){
		random_shuffle(permutation.begin(), permutation.end());
		
		long double new_cost = 0.0;
		for(int i = 0; i < m - 1; i++){
			new_cost += dist_matrix[permutation[i]][permutation[i + 1]];
		}

		if(best_solution.cost == -1.0 || best_solution.cost > new_cost){
			best_solution.update(new_cost, permutation);
		}
	} 

	best_solution.apply(matrix);
	best_solution.print(0); 
	return best_solution;
}


int rooting(vector<int> &root, int x){
	if(root[x] == x){
		return x;
	}

	return root[x] = rooting(root, root[x]);
}

Solution spanning_tree(vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function){
	int n = matrix.size();
	int m = matrix[0].size();

	vector<vector<long double>> dist_matrix = distance_matrix_columns(matrix, cost_function);
	vector<tuple<long double, int, int>> distances;

	for(int i = 0; i < dist_matrix.size(); i++){
		for(int j = 0; j < dist_matrix[i].size(); j++){
			distances.push_back(make_tuple(dist_matrix[i][j], i, j));
		}
	}

	sort(distances.begin(), distances.end());

	vector<vector<int>> adjacent; 
	vector<int> root, path_ends, size;
	root.resize(m);
	path_ends.resize(m);
	size.resize(m);
	adjacent.resize(m);
	for(int i = 0; i < m; i++){
		root[i] = i;
		size[i] = 1;
		path_ends[i] = 1;
	}

	for(int i = 0; i < distances.size(); i++){
		int a = get<1>(distances[i]);
		int b = get<2>(distances[i]);

		int ra = rooting(root, a);
		int rb = rooting(root, b);

		if(path_ends[a] && path_ends[b] && ra != rb){
			if(size[ra] > 1){
				path_ends[a] = 0;
			}
			if(size[rb] > 1){
				path_ends[b] = 0;
			} 

			size[rb] += size[ra];
			size[ra] = 0;
			root[ra] = rb;

			adjacent[a].push_back(b);
			adjacent[b].push_back(a);
		}
	}

	int current = 0;
	for(int i = 0; i < m; i++){
		if(path_ends[i]){
			current = i;
			break;
		}
	}

	int prev = -1;
	int next = 0;
	vector<int> permutation;
	while(next != -1){
		permutation.push_back(current);
		next = -1;
		for(int v : adjacent[current]){
			if(v != prev){
				next = v; 
			}
		}
		prev = current;
		current = next;
	}

	vector<vector<long double>> final_matrix = apply_permutation_columns(matrix, permutation);
	Solution best_solution = Solution(n, m, cost_function(final_matrix), final_matrix, permutation);
	best_solution.print(0);
	return best_solution;
}


Solution random_greedy_path_two_ends(int k, vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function){
	int n = matrix.size();
	int m = matrix[0].size();
	mt19937 gen(time(NULL));

	vector<vector<long double>> dist_matrix = distance_matrix_columns(matrix, cost_function);

	vector<int> used, from;
	used.resize(m);
	from.resize(m);

	int left_end = 0, right_end = 0;
	from[0] = -1;
	used[0] = 1;
	for(int i = 0; i < m - 1; i++){
		vector<tuple<long double, int, int>> v;
		for(int j = 0; j < m; j++){
			if(!used[j]){
				v.push_back(make_tuple(dist_matrix[left_end][j], j, 0));
			}
		}

		if(left_end != right_end){
			for(int j = 0; j < m; j++){
				if(!used[j]){
					v.push_back(make_tuple(dist_matrix[right_end][j], j, 1));
				}
			}
		}

		sort(v.begin(), v.end());

		int ind = min(v.size() - 1, gen() % k);
		int x = get<1>(v[ind]);
		int option = get<2>(v[ind]);
		
		used[x] = 1;
		if(option == 0){
			from[x] = left_end;
			left_end = x;
		}

		if(option == 1){
			from[x] = right_end;
			right_end = x;
		}
	}

	vector<int> permutation;
	permutation.resize(m);

	int index = 0;
	int current = left_end;
	while(current != -1){
		permutation[index++] = current;
		current = from[current];
	}

	index = m - 1;
	current = right_end;
	while(current != -1){
		permutation[index--] = current;
		current = from[current];
	}

	vector<vector<long double>> final_matrix = apply_permutation_columns(matrix, permutation);
	Solution best_solution = Solution(n, m, cost_function(final_matrix), final_matrix, permutation);
	best_solution.print(0);
	return best_solution;
}