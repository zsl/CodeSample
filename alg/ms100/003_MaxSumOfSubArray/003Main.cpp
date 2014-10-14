#include <iostream>

int maxSubarray(int a[], int size) 
{
	//if (size<=0) error( ¡° error array size
	int sum = 0;
	int cur = 0;
	for (int i = 0; i < size; ++i){
		if (cur < 0)
			cur = 0;

		cur += a[i];

		if (sum < cur){
			sum = cur;
		}
	}
	return sum;
}

int main()
{
	int arr[] = {1, -2, 3, 10, -10, 7, 2, -5};
	std::cout << maxSubarray(arr, sizeof arr / sizeof arr[0]);
}