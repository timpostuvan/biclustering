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
	
	void update(long double cost, vector<vector<long double>> &matrix, vector<int> permutation);

	void print();

	Solution& operator=(const Solution &sol);

	bool operator< (const Solution &sol) const;
};


void all_permutations(vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function);

void random_permutations(int iterations, vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function);

#endif