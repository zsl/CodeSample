#include <stdio.h>
#include <string.h>

int arr[] = {4, 2, 9, 8, 0, 5, 7, 1, 3};
#define ARR_SIZE 9

// 排序
void bubble_sort(int arr[], size_t size);
void insertion_sort(int arr[], size_t size);
void quick_sort(int arr[], size_t size);
void heap_sort(int arr[], size_t size);

// 搜索
int binary_search(int arr[], int start, int end, int val);

void print_arr(int arr[], size_t size);

void main(){
    int arr1[ARR_SIZE];

    print_arr(arr, ARR_SIZE);

    memcpy(arr1, arr, sizeof arr);
    printf("bubble sort\n");
    bubble_sort(arr1, ARR_SIZE);
    print_arr(arr1, ARR_SIZE);

    memcpy(arr1, arr, sizeof arr);
    printf("insertion sort\n");
    insertion_sort(arr1, ARR_SIZE);
    print_arr(arr1, ARR_SIZE);

    memcpy(arr1, arr, sizeof arr);
    printf("quick sort\n");
    quick_sort(arr1, ARR_SIZE);
    print_arr(arr1, ARR_SIZE);

    memcpy(arr1, arr, sizeof arr);
    printf("heap sort\n");
    heap_sort(arr1, ARR_SIZE);
    print_arr(arr1, ARR_SIZE);

    printf("arr1 find 5: %d\n", binary_search(arr1, 0, ARR_SIZE, 5));
    printf("arr1 find 9: %d\n", binary_search(arr1, 0, ARR_SIZE, 9));
    printf("arr1 find 0: %d\n", binary_search(arr1, 0, ARR_SIZE, 0));
    printf("arr1 find 6: %d\n", binary_search(arr1, 0, ARR_SIZE, 6));
}

//////////////////////////////////////////////////////////////
// util function
void print_arr(int arr[], size_t size){
    int i;
    for (i = 0; i < size; ++ i)
        printf("%d ", arr[i]);

    putchar('\n');
}

void swap(int arr[], size_t i, size_t j){
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

void copy_backward(int arr[], int start, int end){
    int i;
    for (i = end - 1; i >= start; --i)
        arr[i + 1] = arr[i];
}

//////////////////////////////////////////////////////////////
// sort

void bubble_sort(int arr[], size_t size){
    int i; // 外层循环次数
    int j;

    if (size <= 1) return;

    for (i = size - 1; i > 0; --i)
        for (j = 0; j < i; ++j){
            if (arr[j] > arr[j + 1])
                swap(arr, j, j + 1);
        }
}

void insertion_sort(int arr[], size_t size){
    int i;
    int j;
    int item;

    if (size <= 1) return;

    for (i = 1; i < size; ++i){
        item = arr[i];
        if (item < arr[0]){
            copy_backward(arr, 0, i);
            arr[0] = item;
        }
        else{
            for (j = i - 1; j >= 0; --j){
                if (item >= arr[j]){ // 如果大于等于前面的元素，则找到了自己的位置，应插入到j+1处
                    arr[j + 1] = item;
                    break;
                }
                arr[j + 1] = arr[j];
            }
        }
    }
}

int partition(int arr[], int start, int end){
    int threshold = arr[end - 1];
    int left = 0;
    int right = end - 1;
    while (left < right){
        while(arr[left] <= threshold && left != right) ++left;
        if (left < right) arr[right--] = arr[left];
        while(arr[right] >= threshold && right != left) --right;
        if (left < right) arr[left++] = arr[right];
    }
    arr[right] = threshold;
    return right;
}

void quick_sort(int arr[], size_t size){
    int middle;
    if (size <= 1) return;
    middle = partition(arr, 0, size);
    if (middle > 1) quick_sort(arr, middle);
    if (size - middle - 1 > 1) quick_sort(arr + middle + 1, size - middle - 1);
}

// 堆排序
// 索引从1开始算起
// parent(i) = i / 2
// left(i) =  i*2
// right(i) = i*2 + 1
//       0
//      / \
//     1   2
//    / \ /  \
//   3  4 5  6

void adjust_max_heap(int arr[], int start, int end){
    int i = start;
    int max_index = i;
    while (i * 2 + 1 < end){
        if (arr[max_index] < arr[2 * i + 1]) max_index = 2 * i + 1;
        if (2 * i + 2 < end && arr[max_index] < arr[2 * i + 2]) max_index = 2 * i + 2;
        if (i == max_index) break;
        else{
            swap(arr, i, max_index);
            i = max_index;
        }
    }
}

void build_max_heap(int arr[], size_t size){
    int i;
    for (i = size / 2 - 1; i >= 0; --i)
        adjust_max_heap(arr, i, size);
}
void heap_sort(int arr[], size_t size){
    int i;
    build_max_heap(arr, size);
    // print_arr(arr, size);
    for (i = size - 1; i > 0; --i ){
        swap(arr, 0, i);
        adjust_max_heap(arr, 0, i);
        // print_arr(arr, size);
    }
}

//////////////////////////////////////////////////
// binary_search
int binary_search(int arr[], int start, int end, int val){
    int middle = (end + start) / 2;
    if (val == arr[middle]) return middle;
    if (start >= end) return -1;
    if (val > arr[middle]) return binary_search(arr, middle + 1, end, val);
    if (val < arr) return binary_search(arr, start, middle, val);
}
