#ifndef MATRIX
#define MATRIX

#include <vector>
#include <string>

using namespace std;


long double my_rand();

vector<vector<long double>> generate_binary_matrix(int n, int m);

vector<vector<long double>> generate_random_matrix(int n, int m);

vector<vector<long double>> read_matrix();

vector<vector<long double>> read_matrix_file(string path);

void output_matrix(vector<vector<long double>> &matrix);

void output_matrix_file(string path, vector<vector<long double>> &matrix);

vector<vector<long double>> apply_permutation_columns(vector<vector<long double>> &matrix, vector<int> &permutation);

vector<vector<long double>> apply_permutation_rows(vector<vector<long double>> &matrix, vector<int> &permutation);

void print_matrix(vector<vector<long double>> &matrix);

#endif