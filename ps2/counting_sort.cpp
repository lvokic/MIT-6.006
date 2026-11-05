#include <_stdlib.h>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void counting_sort(vector<int>& nums, int exp, int n) {
  	vector<int> output(nums.size(), 0);
	vector<int> digits(nums.size(), 0);
	vector<int> count(n, 0);

	int size = nums.size();
	for (int i = 0; i < size; ++i) {
		digits[i] = (nums[i] / exp) % n;
		count[digits[i]]++;
	}
	for (int i = 1; i < n; ++i)
		count[i] += count[i - 1];

	for (int i = size - 1; i >= 0; --i) {
		int digit = digits[i];
		output[--count[digit]] = nums[i];
	}
	swap(nums, output);
}

void radix_sort(vector<int>& nums, int exp, int n) {
	int maxium = *max_element(nums.begin(), nums.end());
	while (maxium / exp > 0) {
		counting_sort(nums, exp, n);
		exp *= n;
	}
}

int main() {
	vector<int> nums {3,13,49,23,45,68,92,104,102,119,234,221};
	radix_sort(nums, 1, 10);
	for (const auto num : nums) {
		cout << num << " ";
	}
}
