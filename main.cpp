#include <stdio.h>
#include <stdlib.h>

// Structure to represent subset sum with cardinality information
struct SubsetSum {
    int sum;
    int cardinality;
};

// Function to calculate subset sums of set S up to u
struct SubsetSum* AllSubsetSums(int S[], int n, int u);

// Function to perform XOR operation on two sets of subset sums
struct SubsetSum* XORSubsetSums(struct SubsetSum* S1, int n1, struct SubsetSum* S2, int n2, int u);

// Function to print subset sums
void printSubsetSums(struct SubsetSum* subsetSums, int n);

int main() {
    int S[] = {1, 2, 3}; // Example input set
    int n = sizeof(S) / sizeof(S[0]);
    int u = 10; // Upper bound

    struct SubsetSum* subsetSums = AllSubsetSums(S, n, u);
    printSubsetSums(subsetSums, u + 1);

    free(subsetSums);

    return 0;
}

// Function to calculate subset sums of set S up to u
struct SubsetSum* AllSubsetSums(int S[], int n, int u) {
    if (n == 1) {
        // Base case: If S contains only one element x
        struct SubsetSum* result = (struct SubsetSum*)malloc(2 * sizeof(struct SubsetSum));
        result[0].sum = 0;
        result[0].cardinality = 0;
        result[1].sum = S[0];
        result[1].cardinality = 1;
        return result;
    }

    // Step 3: Choose an arbitrary subset T of S of size n/2
    int* T = (int*)malloc((n / 2) * sizeof(int));
    for (int i = 0; i < n / 2; i++) {
        T[i] = S[i];
    }

    // Calculate subset sums for T and S\T recursively
    struct SubsetSum* subsetSumsT = AllSubsetSums(T, n / 2, u);
    struct SubsetSum* subsetSumsST = AllSubsetSums(S + n / 2, n - n / 2, u);

    // Calculate XOR of subset sums and apply upper bound
    struct SubsetSum* result = XORSubsetSums(subsetSumsT, 1 << (n / 2), subsetSumsST, 1 << (n - n / 2), u);

    free(T);
    free(subsetSumsT);
    free(subsetSumsST);

    return result;
}

// Function to perform XOR operation on two sets of subset sums
struct SubsetSum* XORSubsetSums(struct SubsetSum* S1, int n1, struct SubsetSum* S2, int n2, int u) {
    int maxSize = n1 * n2;
    struct SubsetSum* result = (struct SubsetSum*)malloc(maxSize * sizeof(struct SubsetSum));
    int count = 0;

    // Iterate over subset sums of S1 and S2
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            int sum = S1[i].sum + S2[j].sum;
            if (sum <= u) {
                result[count].sum = sum;
                result[count].cardinality = S1[i].cardinality + S2[j].cardinality;
                count++;
            }
        }
    }

    // Apply upper bound and return the final subset sums
    struct SubsetSum* finalResult = (struct SubsetSum*)malloc(count * sizeof(struct SubsetSum));
    for (int i = 0; i < count; i++) {
        finalResult[i] = result[i];
    }

    free(result);
    return finalResult;
}

// Function to print subset sums
void printSubsetSums(struct SubsetSum* subsetSums, int n) {
    printf("Subset Sums:\n");
    for (int i = 0; i < n; i++) {
        printf("Sum: %d, Cardinality: %d\n", subsetSums[i].sum, subsetSums[i].cardinality);
    }
}
