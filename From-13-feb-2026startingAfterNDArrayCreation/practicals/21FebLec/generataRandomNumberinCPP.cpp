#include<iostream>
#include<cstdlib> // for rand() and srand()
#include<ctime>   // for time(), which is used to seed the random number generator

using namespace std;
// int main() {
//     srand(1);
//     cout<<"Random number with seed 1: " << rand() << endl;
//     cout<<"Random number with seed 1: " << rand() << endl;
//     cout<<"Random number with seed 1: " << rand() << endl;
//     cout<<"Random number with seed 1: " << rand() << endl;
// }

int main() {
    srand(time(0)); // Seed the random number generator with the current time
    cout<<"Random number with seed 1: " << rand() << endl;
    cout<<"Random number with seed 1: " << rand() << endl;
    cout<<"Random number with seed 1: " << rand() << endl;
    cout<<"Random number with seed 1: " << rand() << endl;
}
