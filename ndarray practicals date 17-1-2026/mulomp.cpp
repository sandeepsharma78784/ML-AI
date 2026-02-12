// jp kaam threading se kiya vo without thread.

#include<omp.h>
#include<iostream>
#include<vector>
#include<chrono>
using namespace std;
int main()
{
    int matrix_1_rows= 4096;
    int matrix_1_columns = 4096;
    int matrix_2_rows= 4096;
    int matrix_2_columns = 4500;

    vector<double> matrix_1(matrix_1_rows*matrix_1_columns);
    vector<double> matrix_2(matrix_2_rows*matrix_2_columns);
    vector<double> matrix_3(matrix_1_rows*matrix_2_columns);

    double value=1.0;
    // populate matrix 1
    for(int i=0;i<matrix_1.size();i++) matrix_1[i]=value++;
    value=10.0;
    // populate matrix 2
    for(int i=0;i<matrix_2.size();i++) matrix_2[i]=value++;
   


     int r,c,c1;
    auto start_time= chrono::high_resolution_clock::now();
     // populate matrix 3
    for(int i=0;i<matrix_3.size();i++) matrix_3[i]=0.0; //accumulate krege.

    // #pragma omp parallel for private(c,c1)
    #pragma omp parallel num_threads(8) for private(c,c1)
    for(r=0;r<matrix_1_rows;++r)
    {
        for(c=0;c<matrix_1_columns;++c)
        {
            for(c1=0;c1<matrix_2_columns;++c1)
            {
                matrix_3[r*matrix_2_columns +c1]+=matrix_1[r*matrix_1_columns + c]*matrix_2[c*matrix_2_columns +c1];
            }
            
        }
    }
    // implicit barrier
    auto end_time= chrono::high_resolution_clock::now();
    chrono::duration<double> tt= end_time-start_time;

    cout<<"Duration: "<<tt.count()<<"seconds"<<endl;

    // print product matrix
    // for(r=0;r<matrix_1_rows;r++)
    // {
    //     for(c=0;c<matrix_2_columns;c++)
    //     {
    //         cout<<matrix_3[r*matrix_2_columns + c]<<" ";
    //     }
    //     cout<<endl;
    // }




}