#include <iostream>
#include "projectPattern.h"

int matrix1[3][2] = { {1, 2},
                      {3, 4},
                      {5, 6} };

int matrix2[2][2] = { {7, 8},
                      {9, 10} };

int resultMatrix[3][2];

void multiplyMatrices(int mat1[3][2], int mat2[2][2], int result[3][2]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < 2; ++k) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

int main() {

    multiplyMatrices(matrix1, matrix2, resultMatrix);

    std::cout << "Resultant Matrix:\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            std::cout << resultMatrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}