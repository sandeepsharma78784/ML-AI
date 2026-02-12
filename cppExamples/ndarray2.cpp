#include <iostream>
#include <vector>
#include <stdexcept>

class NDArrayNode;  // forward declare

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

    // flat index calculation
    size_t flatIndex(const std::vector<size_t>& idx) const {
        if (idx.size() != dims.size())
            throw std::invalid_argument("Index dimension mismatch");

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

    // SINGLE constructor
    NDArrayNode(NDArray* p, const std::vector<size_t>& idx)
        : parent(p), indices(idx) {}


    // NDArrayNode indexing
    NDArrayNode operator[](size_t i) {

        // Too many dimensions
        if (indices.size() >= parent->dims.size())
            throw std::out_of_range("Too many indices applied.");

        size_t dim = indices.size();

        // Out-of-bounds check
        if (i >= parent->dims[dim])
            throw std::out_of_range("Index out of bounds!");

        std::vector<size_t> next = indices;
        next.push_back(i);

        return NDArrayNode(parent, next);
    }


    // Convert NDArrayNode → int&
    operator int&() {
        size_t flat = parent->flatIndex(indices);
        return parent->data[flat];
    }

    // Assign value to element
    NDArrayNode& operator=(int v) {
        size_t flat = parent->flatIndex(indices);
        parent->data[flat] = v;
        return *this;
    }
};


// NDArray top-level operator[]
NDArrayNode NDArray::operator[](size_t i) {
    if (dims.empty())
        throw std::logic_error("No dimensions defined");

    if (i >= dims[0])
        throw std::out_of_range("Top-level index out of bounds!");

    return NDArrayNode(this, std::vector<size_t>{i});
}



// ------------------ TEST --------------------
int main() {
    NDArray arr({2, 3, 4});   // 2x3x4 array

    arr[1][2][3] = 99;
    arr[1][2][3] = 100;

    std::cout << arr.data[(1*3*4) + (2*4) + 3] << "\n"; // prints 100

    int x = arr[1][2][3];
    std::cout << x << "\n"; // prints 100

    // Testing bounds check (should throw)
    try {
        arr[1][0][9] = 5;  // 3rd dimension is size 4
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}
