#include<iostream>
#include<vector>
#include<chrono>
using namespace std;
int main()
{
    int matrix_1_rows= 2;
    int matrix_1_columns = 3;
    int matrix_2_rows= 3;
    int matrix_2_columns = 2;

    vector<double> matrix_1(matrix_1_rows*matrix_1_columns);
    vector<double> matrix_2(matrix_2_rows*matrix_2_columns);
    vector<double> matrix_3(matrix_1_rows*matrix_2_columns);

    double value=1.0;
    // populate matrix 1
    for(int i=0;i<matrix_1.size();i++) matrix_1[i]=value++;
    value=10.0;
    // populate matrix 2
    for(int i=0;i<matrix_2.size();i++) matrix_2[i]=value++;

    int r,c,j;
    double sum;
    auto start_time= chrono::high_resolution_clock::now();
    for(r=0;r<matrix_1_rows;r++)
    {
        for(c=0;c<matrix_2_columns;c++)
        {
            sum=0.0;
            for(j=0;j<matrix_1_columns;j++)
            {
                sum+=matrix_1[r*matrix_1_columns + j]*matrix_2[j*matrix_2_columns +c];
            }
            matrix_3[r*matrix_2_columns +c]=sum;
        }
    }
    auto end_time= chrono::high_resolution_clock::now();
    chrono::duration<double> tt= end_time-start_time;

    cout<<"Duration: "<<tt.count()<<"seconds"<<endl;

    // print product matrix
    for(r=0;r<matrix_1_rows;r++)
    {
        for(c=0;c<matrix_2_columns;c++)
        {
            cout<<matrix_3[r*matrix_2_columns + c]<<" ";
        }
        cout<<endl;
    }




}