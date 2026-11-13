#include <vector>
#include <string_view>

using namespace std;

class CountAnagram {
private:
    string_view src_{};
    string_view dst_{};

public:
    explicit CountAnagram(string &src) : src_(src) {}

    int count_anagram_substrings(string &dst, const int k) {
        if (src_.size() < k)
            return 0;
        int res = 0;
        dst_ = dst;

        vector<int> src_window(26, 0);
        vector<int> dst_window(26, 0);
        for (auto ch : dst_) 
            dst_window[ch - 'a']++;
        for (int i = 0; i < k; ++i)
            src_window[src_[i] - 'a']++;
        if (src_window == dst_window)
            res++;
        
        int start = k;
        while (start < src_.size()) {
            char replace = src_[start - k];
            char ch = src_[start];
            src_window[replace - 'a']--;
            src_window[ch - 'a']++;
            if (src_window == dst_window)
                res++;
            start++;
        }
        return res;
    }

    
};