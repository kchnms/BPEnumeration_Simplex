#include "functions.h"
#include <algorithm>
#include <tuple>

using namespace std;



vector <double> gaussian_reverse(Matrix A, vector_t B) {
	vector<double> X(B.size());
	//fill(X.begin(), X.end(), 0);
	for (int i = A.cols - 1; i >= 0; i--) {
		double beta = B[i];
		for (int j = A.cols - 1; j > i; j--) {
			beta -= A(i, j) * X[j];
		}
		X[i] = beta / A(i, i);
		if (fabs(X[i]) < 1e-6) {
			X[i] = 0;
		}
	}
	return X;
};

vector <double> solve(Matrix A, vector_t B) {
	double C, S;
	for (int i = 0; i < A.cols - 1; i++) {
		for (int m = i + 1; m < A.cols; m++) {
			C = A(i, i) / sqrt(A(i, i) * A(i, i) + A(m, i) * A(m, i));
			
			S = A(m, i) / sqrt(A(i, i) * A(i, i) + A(m, i) * A(m, i));
			if (isnan(C) || (isnan(S))) {
				return vector<double> { BAD_DETERMINANT };
			}
			for (int k = i; k < A.cols; k++) {
				double A1 = C * A(i, k) + S * A(m, k);
				double A2 = C * A(m, k) - S * A(i, k);
				/*if (fabs(A1) < 1e-6) {
					A1 = 0;
				}
				if (fabs(A2) < 1e-6) {
					A2 = 0;

				}*/
				A(i, k) = A1;
				A(m, k) = A2;
			}
			double B1 = C * B[i] + S * B[m];
			double B2 = C * B[m] - S * B[i];
			B[i] = B1;
			B[m] = B2;
		}
	}

	if (determinant_upper_triangle(A) != 0){
		return gaussian_reverse(A, B);
	}
	else {
		return { BAD_DETERMINANT };
	}
	
};

double determinant_upper_triangle(Matrix A) {
	double det = 1;
	if (A.rows != A.cols) {
		throw std::invalid_argument("matrix is not square");
	}
	for (int i = 0; i < A.cols; i++) {
		if (fabs(A(i, i)) < 1e-6) { return det = 0; };
		det *= A(i, i);
	}
	if (fabs(det) < 1e-6) {
		return det;
	}
	
}


double evaluate(vector_t obj_function, vector<double> solution, set<int> variable_indices) {
    double result_value = 0;
    reverse(solution.begin(), solution.end());
    for (auto index : variable_indices) {
 		result_value += obj_function[index] * (solution.back());
		solution.pop_back();
    }
	return result_value;
}

vector<set<int>> generate_combinations(vector<int> column_set, int k) {
	vector<int> combination;
	vector <set<int>> combinations;

	go(column_set, combination, combinations, 0, k);

	/*vector<set<int>> combinations;
	vector<int> current_permutation(column_set.begin(), column_set.end());
	do {
		set<int> combination(current_permutation.begin(), current_permutation.begin() + k);
		if (auto it = find(combinations.begin(), combinations.end(), combination) == combinations.end()) {
			combinations.push_back(combination);
			cout << combinations[0];
		}
	} while (next_permutation(current_permutation.begin(), current_permutation.end()));
	return  combinations;*/
	return combinations;
}

void go(vector <int>& column_set, vector <int>& combination, vector <set<int>> &combmas, int offset, int k) {
	if (k == 0) {
		set<int> cur_comb(combination.begin(), combination.end());
		combmas.push_back(cur_comb);
		return;
	}
	for (int i = offset; i <= column_set.size() - k; ++i) {
		combination.push_back(column_set[i]);
		go(column_set, combination, combmas,i + 1, k - 1);
		combination.pop_back();
	}
}