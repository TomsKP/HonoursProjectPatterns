#include <iostream>
#include "projectPattern.h"
#include "tbb/tbb.h"
#include <chrono>
using namespace std::chrono;

int matrix1[3][2] = { {1, 2},
                      {3, 4},
                      {5, 6} };

int matrix2[2][2] = { {7, 8},
                      {9, 10} };

int bigMatrix1[3000][3000];

int bigMatrix2[3000][3000];
 
int bigResultMatrix[3000][3000];

int resultMatrix[3][2];

struct argData {
    int start;
    int end;
    int threadCount;
};


void* multiplyMatricesParallel(void* ptr);

void* multiplyMatricesParallel(void* ptr) {
    argData* argument = (argData*)ptr;
    int row = argument->start;
    int endRow = argument->end;
    for (row; row < endRow; row++) {
        for (int j = 0; j < 3000; j++) {
            for (int k = 0; k < 3000; k++) {
                bigResultMatrix[row][j] += bigMatrix1[row][k] * bigMatrix2[k][j];
            }
        }
    }
    return 0;
}


//Ideas for this implementation came from: https://stackoverflow.com/questions/10607215/simplest-tbb-example and https://homepages.math.uic.edu/~jan/mcs572f16/mcs572notes/lec11.html
//Still not 100% on the working behind it, but fairly confident I know how to use TBB at this extremely basic level
class MatrixMultiplierTBB {
public:
    void operator()(const tbb::blocked_range<int>& r) const {
        for (int row = r.begin(); row != r.end(); row++) {
            for (int i = 0; i < 3000; i++) {
                for (int j = 0; j < 3000; j++) {
                    bigResultMatrix[row][i] += bigMatrix1[row][j] * bigMatrix2[j][i];
                }
            }
        }
    }
};

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
    parallelPatterns parallel;

    
    for (int i = 0; i < 3000; i++) {
        for (int j = 0; j < 3000; j++) {
            bigMatrix1[i][j] = 42;
        }
    }

    for (int i = 0; i < 3000; i++) {
        for (int j = 0; j < 3000; j++) {
            bigMatrix2[i][j] = 42;
        }
    }

    auto start = high_resolution_clock::now();
    //multiplyMatrices(matrix1, matrix2, resultMatrix);
    parallel.parallelFor2D(3000, multiplyMatricesParallel);
    //tbb::parallel_for(tbb::blocked_range<int>(0, 3), MatrixMultiplierTBB());

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    std::cout << duration.count() << std::endl;


    /*std::cout << "Resultant Matrix:\n";
    for (int i = 0; i < 2000; i++) {
        for (int j = 0; j < 2000; j++) {
            std::cout << bigResultMatrix[i][j] << " ";
        }
        std::cout << "\n";
    }*/

    return 0;
}