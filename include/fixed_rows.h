#ifndef FIXED_ROWS
#define FIXED_ROWS

#include <vector>
#include <functional>
#include "fixed_rows.h"

using namespace std;

class Solution{
public:

	int n, m;
	long double cost;
	vector<vector<long double>> matrix;
	vector<int> permutation;

	Solution(int n, int m, long double cost, vector<vector<long double>> &matrix, vector<int> &permutation);
	
	void update(long double cost, vector<int> permutation);

	void apply_permutation();

	void print(bool output_matrix);

	Solution& operator=(const Solution &sol);

	bool operator< (const Solution &sol) const;
};

long double lower_bound_1_closest(vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function);

long double lower_bound_spanning_tree(vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function);

long double lower_bound_2_closest(vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function);

Solution all_permutations(vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function);

Solution random_permutations(int iterations, vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function);

int rooting(vector<int> &root, int x);

Solution spanning_tree(vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function);

Solution random_greedy_path_two_ends(int k, vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function);

long double calculate_difference(vector<vector<long double>> &dist_matrix, int x, int y);

Solution tsp_2_opt(int search_iterations, long double difference, vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function);

#endif