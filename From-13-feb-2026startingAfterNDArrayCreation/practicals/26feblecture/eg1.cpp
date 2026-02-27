/*
traspose is working but 
deep copy of collection is gets created because of  
vector class copy constructor.
*/

#include <iostream>
#include <vector>
#include<stdint.h>
using namespace std;

class NDArray{
private:
    vector<uint32_t> dimensions;
    vector<int> collection;
    bool isTransposed;       // to decide row-major and column major
    
    uint64_t flatIndex(uint32_t row, uint32_t col) const {
        uint32_t rows = dimensions[0];
        uint32_t cols = dimensions[1];

        if (!isTransposed) {
            // Apply Row major formula to calculate flatindex
            return row * cols + col;
        } else {
            // Apply Column major formula to calculate flatindex
            return col * rows + row;
        }
    } 
  public:
    NDArray(uint32_t rows, uint32_t cols)
        : dimensions{rows, cols},collection(vector<int>(rows * cols)),isTransposed(false) {}

    // Set value
    void set(uint32_t row, uint32_t col, int value) {
        collection[flatIndex(row, col)] = value;
    }

    // Get value
    int get(uint32_t row, uint32_t col) const {
        return collection[flatIndex(row, col)];
    }
    
     // Transpose function 
    NDArray transpose() const {
        NDArray t = *this;  
        swap(t.dimensions[0], t.dimensions[1]);
        t.isTransposed = !isTransposed;
        return t;
    }
    
    
    // print 
        void print() const {
        for (uint32_t i = 0; i < dimensions[0]; ++i) {
            for (uint32_t j = 0; j < dimensions[1]; ++j) {
                cout << get(i, j) << " ";
            }
            cout << "\n";
        }
    }
    
  };
int main() 
{
  
    NDArray m(2, 3);
    m.set(0, 0, 1);
    m.set(0, 1, 2);
    m.set(0, 2, 3);
    m.set(1, 0, 4);
    m.set(1, 1, 5);
    m.set(1, 2, 6);

    cout << "Original:\n";
    m.print();

    NDArray t = m.transpose();  // nayi copy bn rahi he collection ki bhi .
    cout << "Transposed:\n";
    t.print();
	
    // change the value in using t
    t.set(0,1, 6);
    cout << "Transposed after value change :\n";
    t.print();

    std::cout << "Original after value change:\n";
    m.print();
    return 0;
}