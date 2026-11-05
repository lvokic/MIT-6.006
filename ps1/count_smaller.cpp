#include <vector>

using namespace std;

class Solution {
public:
    struct Item {
        int first;
        int second;
    };

    void merge(vector<Item>& nums, vector<int>& smaller, int left, int middle, int right) {
        int left_index = left;
        int right_index = middle + 1;
        const int right_start = middle + 1;
        const int merge_size = right - left + 1;
        vector<Item> tmp;
        tmp.reserve(merge_size);
        while (left_index <= middle && right_index <= right) {
            if (nums[right_index].first < nums[left_index].first) {
                tmp.push_back(nums[right_index]);
                ++right_index;
            } else {
                const int original_index = nums[left_index].second;
                const int smaller_count = right_index - right_start;
                smaller[original_index] += smaller_count;
                tmp.push_back(nums[left_index]);
                ++left_index;
            }
        }
        while (left_index <= middle) {
            const int original_index = nums[left_index].second;
            const int smaller_count = right_index - right_start;
            smaller[original_index] += smaller_count;
            tmp.push_back(nums[left_index]);
            ++left_index;
        }
        while (right_index <= right) {
            tmp.push_back(nums[right_index]);
            ++right_index;
        }
        for (int index = 0; index < merge_size; ++index) {
            nums[left + index] = tmp[index];
        }
    }

    void mergeSort(vector<Item>& nums, vector<int>& smaller, int left, int right) {
        if (left >= right)
            return;
        const int middle = left + (right - left) / 2;
        mergeSort(nums, smaller, left, middle);
        mergeSort(nums, smaller, middle + 1, right);
        merge(nums, smaller, left, middle, right);
    }

    vector<int> countSmaller(vector<int>& nums) {
        const int size = static_cast<int>(nums.size());
        vector<int> res(size, 0);
        vector<Item> pass(size);
        for (int i = 0; i < size; ++i) {
            pass[i].first = nums[i];
            pass[i].second = i;
        }
        mergeSort(pass, res, 0, size - 1);
        return res;
    }
};
