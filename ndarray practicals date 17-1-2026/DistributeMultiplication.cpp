// sir k according mul1 me normal multiplication dekha
// mul 2 me optimize wala without cache miss
// humne mul1 and mul2 ka 3D version bna rkha he.

// mul3 me thread me distribute
// mul4 me pragma directive se.

// ye wala exmaple mul3 ko represent krega, for distribute work between threads.

#include<iostream>
#include<vector>
#include<chrono>
#include<thread>

using namespace std;

// wrapper kyu bnaya uska reason pdna he raw note se.
struct wrapper
{
    vector<double> *matrix_1;
    vector<double> *matrix_2;
    vector<double> *matrix_3;
    int matrix_1_rows;
    int matrix_1_columns;
    int matrix_2_rows;
    int matrix_2_columns;
    int start_at;
    int end_at;

};

void multiplier(struct wrapper *w){
     vector<double> *matrix_1=w->matrix_1;
    vector<double> *matrix_2=w->matrix_2;
    vector<double> *matrix_3=w->matrix_3;
    int matrix_1_rows= w->matrix_1_rows;
    int matrix_1_columns =w->matrix_1_columns;
    int matrix_2_rows= w->matrix_2_rows;
    int matrix_2_columns = w->matrix_2_columns;
    int start_at= w->start_at;
    int end_at = w->end_at;
    // cout<<" call "<<endl;
    // cout<<start_at<< " "<< end_at<<endl;
    int r,c,c1;
        // note for(r=start_at;r<end_at;++r) jbki humko endat tk jana tha. iskliye <= use kiya.
        for(r=start_at;r<=end_at;++r)
            {
                // cout<<"loop me ghusaa"<<endl;
                     for(c=0;c<matrix_1_columns;++c)
                    {
                        for(c1=0;c1<matrix_2_columns;++c1)
                        {
                        
                            (*matrix_3)[r*matrix_2_columns+c1]+=(*matrix_1)[r*matrix_1_columns+c] * (*matrix_2)[c*matrix_2_columns+c1];
                            
                            // cout<<val<<" valuess ";
                        }
                    }
            }

}

int main()
{
    // int matrix_1_rows= 2;
    // int matrix_1_columns = 3;
    // int matrix_2_rows= 3;
    // int matrix_2_columns = 2;
    
    // int matrix_1_rows= 2000;
    // int matrix_1_columns = 300;
    // int matrix_2_rows= 300;
    // int matrix_2_columns = 2000;
    // iska 9 sec

    // Duration: 14.4095seconds  1,1,1,1
    // Duration: 50.2408seconds  2,1,1,1
    // Duration: 100.502seconds  2,2,2,2  // without multithread Duration: 481.296seconds
    // Duration: 174.798seconds  2,2,2,2  with omp
    // Duration: 332.474seconds  3,3,3,3
    // Duration: 880.516seconds  4,4,4,4  // Duration: 1257.81seconds with openmp


    // humko open mp ka use kr k ye pta krna he ki kitne threads bn rahe he,
    // pta chle jo logical processor he 4 vo sahi bhi he ya bewakoof bnnae wali baat he.

    int matrix_1_rows= 4096;
    int matrix_1_columns = 4096;
    int matrix_2_rows= 4096 ;
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

    // populate matrix 3
    for(int i=0;i<matrix_3.size();i++) matrix_3[i]=0.0; //accumulate krege.

    // create thread 
    auto cores = thread::hardware_concurrency();
    int number_of_threads=cores;
    // cout<< number_of_threads<<" total core";

    // decide how much work to deploy on thread.
    int batch_size= matrix_1_rows/number_of_threads;
    if(matrix_1_rows%number_of_threads!=0) batch_size++;


    vector<thread> threads;
    vector<struct wrapper> w(number_of_threads); // wector ki size btai.
    int start_at, end_at;
    start_at=0;
    // cout<< "batch size "<<batch_size<<" numbe theres"<<number_of_threads<<" mat1rows"<<matrix_1_rows<<endl;

    // part 16 me page 15 se pending.

    for(int ii=0;ii<number_of_threads;++ii){
        end_at=start_at+batch_size-1;  // batch size means ek thread pr kitni rows daalna he
        if(end_at>=matrix_1_rows) end_at=matrix_1_rows-1;
        w[ii].matrix_1=&matrix_1;
        w[ii].matrix_2=&matrix_2;
        w[ii].matrix_3=&matrix_3;

        w[ii].matrix_1_rows=matrix_1_rows;
        w[ii].matrix_2_rows=matrix_2_rows;
        w[ii].matrix_1_columns=matrix_1_columns;
        w[ii].matrix_2_columns=matrix_2_columns;
        w[ii].start_at=start_at;
        w[ii].end_at=end_at;
        start_at=end_at+1;
    }

    auto start_time= chrono::high_resolution_clock::now();
    for(int ii=0;ii<number_of_threads;ii++){
        threads.emplace_back(multiplier, &w[ii]);
    }
    for(auto &tt:threads) tt.join();
    auto end_time= chrono::high_resolution_clock::now();
    chrono::duration<double> tt= end_time-start_time;

    cout<<"Duration: "<<tt.count()<<"seconds"<<endl;

//  for(int i=0;i<matrix_3.size();i++) cout<<matrix_3[i]<<" "; //accumulate krege.


}