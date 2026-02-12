#include <iostream>
#include <vector>

class NDArrayNode; // forward declare

class NDArray {
public:
    std::vector<size_t> dims;
    std::vector<int> data;

    NDArray(std::initializer_list<size_t> d) {
        dims = d;
        size_t total = 1;
        for (size_t x : dims) total *= x;
        data.resize(total, 0);
    }

    // compute flat index
    size_t flatIndex(const std::vector<size_t>& idx) const {
        size_t flat = 0;
        size_t mul = 1;
        for (int i = dims.size() - 1; i >= 0; --i) {
            flat += idx[i] * mul;
            mul *= dims[i];
        }
        return flat;
    }

    NDArrayNode operator[](size_t i);
};


class NDArrayNode {
public:
    NDArray* parent;
    std::vector<size_t> indices;

    // NDArrayNode(NDArray* p, size_t firstIndex)
    //     : parent(p), indices({firstIndex}) {}

    NDArrayNode(NDArray* p, const std::vector<size_t>& prev, size_t next)
        : parent(p), indices(prev) {
        indices.push_back(next);
    }

    // When not last dimension → return another NDArrayNode
    NDArrayNode operator[](size_t i) {
        if (indices.size() + 1 < parent->dims.size()) {
            return NDArrayNode(parent, indices, i);
        }
        else {
            // Last dimension → return a proxy-like object with full indices
            std::vector<size_t> full = indices;
            full.push_back(i);
            return NDArrayNode(parent, full, 0);
        }
    }

    // Conversion: when full index reached → return reference to actual data
    operator int&() {
        size_t flat = parent->flatIndex(indices);
        return parent->data[flat];
    }

    // Assignment to actual element
    NDArrayNode& operator=(int v) {
        size_t flat = parent->flatIndex(indices);
        parent->data[flat] = v;
        return *this;
    }
};


// NDArray operator[] , ye sahi he
NDArrayNode NDArray::operator[](size_t i) {
    //  NDArrayNode node(this, i); ye wala constructor commnet kr diya he
     NDArrayNode node(this, i);
     return node;
}


// ----------- TEST ------------
int main() {
    NDArray arr({2, 3, 4});

    arr[1][2][3] = 99;
    arr[1][2][3] = 100;


    std::cout << arr.data[ (1*3*4) + (2*4) + 3 ] << "\n"; // should print 99

    int x = arr[1][2][3];
    std::cout << x << "\n"; // also prints 99

// below should throw index out of bound
arr[1][0][3] = 99;
 int y = arr[1][0][3];
    std::cout << y << "\n"; // also prints 99
}