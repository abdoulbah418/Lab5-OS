#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
int MAX;


// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 0. Get the matrix size from the command line and assign it to MAX
    char size[5];
    printf("Enter the size of matrix: ");
    fgets(size, 5, stdin);
    MAX = atoi(size);

    int matA[MAX][MAX]; 
    int matB[MAX][MAX]; 

    int matSumResult[MAX][MAX];
    int matDiffResult[MAX][MAX]; 
    int matProductResult[MAX][MAX];

    // 0.2 Define functions
    void fillMatrix(int matrix[MAX][MAX]) {
        for(int i = 0; i<MAX; i++) {
            for(int j = 0; j<MAX; j++) {
                matrix[i][j] = rand()%10+1;
            }
        }
    }

    void printMatrix(int matrix[MAX][MAX]) {
        for(int i = 0; i<MAX; i++) {
            for(int j = 0; j<MAX; j++) {
                printf("%5d", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    // // Fetches the appropriate coordinates from the argument, and sets
    // // the cell of matSumResult at the coordinates to the sum of the
    // // values at the coordinates of matA and matB.
    void* computeSum(void* args) { // pass in the number of the ith thread
      // Unpack the coordinates from the argument.
      int* coords = (int*) args;
      int i = coords[0];
      int j = coords[1];
      
      // Calculate the sum of the values at the coordinates of matA and matB.
      int sum = matA[i][j] + matB[i][j];
      
      // Store the result in the corresponding cell of matSumResult.
      matSumResult[i][j] = sum;
      
      // Free the memory that was allocated for the coordinates.
      free(coords);
      
      return NULL;
    }

    // // Fetches the appropriate coordinates from the argument, and sets
    // // the cell of matSumResult at the coordinates to the difference of the
    // // values at the coordinates of matA and matB.
    void* computeDiff(void* args) { // pass in the number of the ith thread
      // Unpack the coordinates from the argument.
      int* coords = (int*) args;
      int i = coords[0];
      int j = coords[1];
      
      // Calculate the difference of the values at the coordinates of matA and matB.
      int diff = matA[i][j] - matB[i][j];
      
      // Store the result in the corresponding cell of matSumResult.
      matDiffResult[i][j] = diff;
      
      // Free the memory that was allocated for the coordinates.
      free(coords);
      
      return NULL;
    }

    // // Fetches the appropriate coordinates from the argument, and sets
    // // the cell of matSumResult at the coordinates to the inner product
    // // of matA and matB.
    void* computeProduct(void* args) { // pass in the number of the ith thread
      // Unpack the coordinates from the argument.
      int* coords = (int*) args;
      int i = coords[0];
      int j = coords[1];
      
      // Calculate the product of the values at the coordinates of matA and matB.
      int product = matA[i][j] * matB[i][j];
      
      // Store the result in the corresponding cell of matSumResult.
      matProductResult[i][j] = product;
      
      // Free the memory that was allocated for the coordinates.
      free(coords);
      
      return NULL;
    }





    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);

    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    pthread_t threads[MAX][MAX][3];

    // 4. Create a thread for each cell of each matrix operation.
    // 
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute.
    // 
    // One way to do this is to malloc memory for the thread number i, populate the coordinates
    // into that space, and pass that address to the thread. The thread will use that number to calcuate 
    // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
    for (int i = 0; i < MAX; i++) {
      for (int j = 0; j < MAX; j++) {
          // Allocate memory for the coordinates.
          int* coords = (int*) malloc(sizeof(int) * 2);
          coords[0] = i;
          coords[1] = j;

          // Create a thread that will run the computeSum function, passing in the coordinates.
          int sresult = pthread_create(&threads[i][j][0], NULL, computeSum, (void*) coords);
          
      }
    }
    for (int i = 0; i < MAX; i++) {
      for (int j = 0; j < MAX; j++) {
          // Allocate memory for the coordinates.
          int* coords = (int*) malloc(sizeof(int) * 2);
          coords[0] = i;
          coords[1] = j;

          // Create a thread that will run the computeSum function, passing in the coordinates.
          int dresult = pthread_create(&threads[i][j][1], NULL, computeDiff, (void*) coords);
      }
    }
    for (int i = 0; i < MAX; i++) {
      for (int j = 0; j < MAX; j++) {
          // Allocate memory for the coordinates.
          int* coords = (int*) malloc(sizeof(int) * 2);
          coords[0] = i;
          coords[1] = j;

          // Create a thread that will run the computeSum function, passing in the coordinates.
          int presult = pthread_create(&threads[i][j][2], NULL, computeProduct, (void*) coords);
      }
    }

    // 5. Wait for all threads to finish.
    for (int i = 0; i < MAX; i++) {
      for (int j = 0; j < MAX; j++) {
        for (int k = 0; k < 3; k++) {
          pthread_join(threads[i][j][k], NULL);
        }
      }
    }

    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
  
}
