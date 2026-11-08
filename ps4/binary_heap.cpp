#include <cassert>
#include <utility>
#include <vector>

template <typename T>
class MaxBinaryHeap {
private:
    std::size_t size_;
    std::vector<T> heap_;

private:
    void sitfDown(int index) {
        assert(index >= 0);
        while (index < size_) {
            int left = index * 2 + 1;
            int right = index * 2 + 2;
            if (left >= size_) return;
            else if (right >= size_) {
                if (heap_[index] >= heap_[left]) return;
                std::swap(heap_[index], heap_[left]);
                return;
            }   
            int tmp = heap_[left] >= heap_[right] ? left : right;
            if (heap_[index] >= heap_[tmp]) break;
            std::swap(heap_[index], heap_[tmp]);
            index = tmp;
        }
    }

    void siftUp(int index) {
        assert(index < size_);
        while (index > 0) {
            int parent = (index - 1) >> 1;
            if (heap_[index] <= heap_[parent]) break;
            std::swap(heap_[index], heap_[parent]);
            index = parent;
        }
    }

    void buildHeap() {
        for (int i = (size_ - 1) / 2; i >= 0; --i)
            sitfDown(i);
    }

public:
    explicit MaxBinaryHeap(std::vector<T>& in) : heap_(in), size_(in.size()) {
        buildHeap();
    }

    T deleteMax() {
        assert(size_ > 0);
        T res = heap_.front();
        std::swap(heap_[0], heap_[size_ - 1]);
        heap_.pop_back();
        size_--;
        if (size_ > 0) sitfDown(0);
        return res;
    }
    
};