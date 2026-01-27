#include <iostream>
#include <vector>
#include <type_traits>
using namespace std;

template<typename... T>
void sam(T... k)
{
    vector<int> indices;
    double value = 0;   // initialize

    auto process = [&](auto&& x) {
        using X = typename std::decay<decltype(x)>::type;

        if (std::is_same<X, int>::value) {
            indices.push_back(x);
        }
        else if (std::is_same<X, double>::value) {
            value = x;
        }
		else{

		}
        // else ignore other types (const char*, float, etc.)
    };

    int dummy[] = { (process(k), 0)... };
    (void)dummy; // avoid unused warning

    for (int x : indices)
        cout << x << ", ";

    cout << value << endl;
}

int main()
{
    //sam(2, 3, "Good");         // OK
    sam(5, 40, 50, 3.33, 3);   // OK
    sam();                     // Empty pack – OK
    return 0;
}
