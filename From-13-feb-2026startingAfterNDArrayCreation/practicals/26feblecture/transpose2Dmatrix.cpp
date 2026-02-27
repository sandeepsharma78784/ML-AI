#include <iostream>
#include <vector>
#include<stdint.h>
using namespace std;

class Matrix2D {
private:
    vector<uint32_t> dimensions;   // {rows, cols}
    //shared_ptr<std::vector<int>> collection;
    vector<int> collection;
    bool isTransposed;
    
    uint64_t flatIndex(uint32_t row, uint32_t col) const {
        uint32_t rows = dimensions[0];
        uint32_t cols = dimensions[1];

        if (row >= rows || col >= cols)
            throw std::out_of_range("Index out of range");

        if (!isTransposed) {
            // Row-major
            return row * cols + col;
        } else {
            // Column-major interpretation
            return col * rows + row;
        }
    } 
  public:
    // Constructor
    Matrix2D(uint32_t rows, uint32_t cols)
        : dimensions{rows, cols},
          collection(vector<int>(rows * cols)),
          isTransposed(false) {}

    // Set value
    void set(uint32_t row, uint32_t col, int value) {
        (collection)[flatIndex(row, col)] = value;
    }

    // Get value
    int get(uint32_t row, uint32_t col) const {
        return (collection)[flatIndex(row, col)];
    }
    
      // Transpose (Zero-copy)
    Matrix2D transpose() const {
        Matrix2D result = *this;  // shared_ptr copied (NOT vector)
        swap(result.dimensions[0], result.dimensions[1]);
        result.isTransposed = !isTransposed;
        return result;
    }
    
    
    // print ,rows, cols
        void print() const {
        for (size_t i = 0; i < dimensions[0]; ++i) {
            for (size_t j = 0; j < dimensions[1]; ++j) {
                std::cout << get(i, j) << " ";
            }
            std::cout << "\n";
        }
    }

    uint32_t rows() const { return dimensions[0]; }
    uint32_t cols() const { return dimensions[1]; }
    
  };
int main() 
{
  
     Matrix2D m(2, 3);

    m.set(0, 0, 1);
    m.set(0, 1, 2);
    m.set(0, 2, 3);
    m.set(1, 0, 4);
    m.set(1, 1, 5);
    m.set(1, 2, 6);

    std::cout << "Original:\n";
    m.print();

    Matrix2D t = m.transpose();  // nayi copy bna rahi he collection ki bhi .
    t.set(0,1, 6);  // transose me set krega to issue hoga kyuki ye to Original ko change krega
		 // abhi is case me to separate copy bn rahi he 
// but humara mksd he copy nahi bnana same collection use krna .
    std::cout << "\nTransposed (zero-copy):\n";
    t.print();
    cout << "Hello, World!";
    std::cout << "Original:\n";
    m.print();
    return 0;
}