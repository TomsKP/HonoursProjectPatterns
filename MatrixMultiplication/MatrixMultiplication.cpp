#include <iostream>
#include "projectPattern.h"

int matrix1[3][2] = { {1, 2},
                      {3, 4},
                      {5, 6} };

int matrix2[2][2] = { {7, 8},
                      {9, 10} };

int resultMatrix[3][2];

struct argData {
    int start;
    int end;
    int row;
};


void* multiplyMatricesParallel(void* ptr);

void* multiplyMatricesParallel(void* ptr) {
    argData* argument = (argData*)ptr;
    int row = argument->row;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            resultMatrix[row][i] += matrix1[row][j] * matrix2[j][i];
        }
    }
    return 0;
}

void multiplyMatrices(int mat1[3][2], int mat2[2][2], int result[3][2]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

int main() {
    parallelPatternFor parallel;


    //multiplyMatrices(matrix1, matrix2, resultMatrix);
    parallel.parallelFor(10, multiplyMatricesParallel);


    std::cout << "Resultant Matrix:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            std::cout << resultMatrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}