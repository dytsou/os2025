#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MAX_NUMBERS 10000
#define MAX_LINE_LENGTH 100000

// Structure to hold data for sorting thread
typedef struct {
    int *array;
    int left;
    int right;
} SortData;

// Merge function for merge sort
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    free(L);
    free(R);
}

// Merge sort function
void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Thread function to sort a portion of the array
void* sort_thread(void* arg) {
    SortData* data = (SortData*)arg;
    merge_sort(data->array, data->left, data->right);
    pthread_exit(NULL);
}

// Function to process a single test case
void process_test_case(int numbers[], int count, FILE* output_file) {
    if (count == 0) return;
    
    int *arr = (int *)malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) {
        arr[i] = numbers[i];
    }
    
    // Divide into two halves
    int mid = count / 2;
    
    // Create thread data structures
    SortData data1, data2;
    data1.array = arr;
    data1.left = 0;
    data1.right = mid - 1;
    
    data2.array = arr;
    data2.left = mid;
    data2.right = count - 1;
    
    // Create two sorting threads
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, sort_thread, &data1);
    pthread_create(&thread2, NULL, sort_thread, &data2);
    
    // Wait for both threads to complete
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    // Merge the two sorted halves
    merge(arr, 0, mid - 1, count - 1);
    
    // Write to output file
    for (int i = 0; i < count; i++) {
        fprintf(output_file, "%d", arr[i]);
        if (i < count - 1) {
            fprintf(output_file, " ");
        }
    }
    fprintf(output_file, "\n");
    
    free(arr);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
        return 1;
    }
    
    FILE *output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error: Cannot open output file %s\n", argv[2]);
        fclose(input_file);
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    int numbers[MAX_NUMBERS];
    
    while (fgets(line, sizeof(line), input_file) != NULL) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;
        
        // Skip empty lines
        if (strlen(line) == 0) continue;
        
        // Parse numbers from the line
        int count = 0;
        char *token = strtok(line, " ");
        while (token != NULL && count < MAX_NUMBERS) {
            numbers[count] = atoi(token);
            count++;
            token = strtok(NULL, " ");
        }
        
        // Process this test case
        if (count > 0) {
            process_test_case(numbers, count, output_file);
        }
    }
    
    fclose(input_file);
    fclose(output_file);
    
    return 0;
}

