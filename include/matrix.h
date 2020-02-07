#ifndef MATRIX
#define MATRIX

#include <vector>
#include <string>
#include <functional>

using namespace std;


long double my_rand();

vector<vector<long double>> generate_binary_matrix(int n, int m);

vector<vector<long double>> generate_random_matrix(int n, int m, bool sign);

vector<vector<long double>> random_submatrix(vector<vector<long double>> matrix, int size_x, int size_y);

vector<vector<long double>> read_matrix();

vector<vector<long double>> read_matrix_file(string path);

void output_matrix(vector<vector<long double>> &matrix);

void output_matrix_file(string path, vector<vector<long double>> &matrix);

vector<vector<long double>> apply_permutation_columns(vector<vector<long double>> &matrix, vector<int> &permutation);

vector<vector<long double>> apply_random_permutation_columns(vector<vector<long double>> &matrix);

vector<vector<long double>> apply_permutation_rows(vector<vector<long double>> &matrix, vector<int> &permutation);

vector<vector<long double>> apply_random_permutation_rows(vector<vector<long double>> &matrix);

void print_matrix(vector<vector<long double>> &matrix);

vector<vector<long double>> distance_matrix_columns(vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function);

vector<vector<long double>> distance_matrix_rows(vector<vector<long double>> &matrix, function<long double(vector<vector<long double>>&)> cost_function);

#endif