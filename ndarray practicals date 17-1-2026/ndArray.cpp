#include<vector>
#include<iostream>
#include<stdint.h> //for uint32_t
#include<numeric> // for std::accumulate
#include<unordered_set> // for std::unordered_set
#include<sstream>       // for std::ostringstream and std::istringstream and std::getline and std::ws
using namespace std;
#define T1 double //data type of the ndArray, yaha ; nahi lgta he
// compile k liye
// g++ -std=c++17 ndArray.cpp -o ndArray.out coz we are using c++17 features like fold expressions fold expressions
class NDArrayException : public std::exception {
    private:
        std::string message;
    public:
        // explicit NDArrayException(const std::string&);
        NDArrayException(const std::string&);
        const char* what() const noexcept override; // Override the what() method
};
NDArrayException::NDArrayException(const std::string& _msg) : message(_msg) {}
const char* NDArrayException::what() const noexcept {
    return message.c_str();
}


class NDArrayNode;
class NDArray {

    private:
        std::vector<T1> collection;
        std::vector<uint32_t> dimensions;
        std::unordered_set<NDArrayNode*> ndArrayNodes; // to keep track of all nodes created from this NDArray
        // void _set(uint32_t index, const T1& value);
        void _set(uint32_t index, T1 value);
        T1 _get(uint32_t index);

        // NDArray ki get index.
        uint32_t getIndex(vector<uint32_t> indexes)
        {
            // cout<<"    Debug: in getIndex function called"<<endl;
            if(indexes.size() != this->dimensions.size())
            {
                ostringstream oss;
                oss<<"Index out of bounds 1";
                for(auto j : indexes)
                {
                    oss<<" ["<<j<<"] ";
                }
                oss<<"size of array is ";
                for(auto j : this->dimensions)
                {
                    oss<<" ["<<j<<"] ";
                }
                throw NDArrayException(oss.str());
            }
            uint64_t idx;
            int x;
            uint32_t multiplier = 1;
            idx=0;
            for(x=indexes.size()-1;x>=0;--x)
            {
                // indexex  k x index ka vo elememt dimension k  us index k element k dayre me rehna chahie
                if(indexes[x]>=this->dimensions[x])
                {
                    cout<<"    Debug: in getIndex function: - "<<indexes[x]<<" index vs dimension"<<this->dimensions[x]<<endl;
                    ostringstream oss;
                    oss<<"Index out of bounds 2";
                    for(auto j : indexes)
                    {
                        oss<<" ["<<j<<"] ";
                    }
                    oss<<"size of array is ";
                    for(auto j : this->dimensions)
                    {
                        oss<<" ["<<j<<"] ";
                    }
                    throw NDArrayException(oss.str());
                }
                idx += indexes[x]*multiplier;
                multiplier *= this->dimensions[x];
            }

            //  yaha rule he ki kese hota he index calculation
            // 2d  k case me row major lete he and esa calculate krte he r*numofcols + c
            // column major liya to  c*numofrows + r
            // abhi humne row major hi liya he jisme r*numofcolumn + c
            // where r an c are indexes rowindex colindex  and numofcolumn is dimension 
            // isko offset bolte he: 2D offset = r*numofcolumn + c
            // similarly 3d k case me 
            /*
            3D offset = i × (Rows × Columns)
                        + j × Columns
                        + k

            */

             /*
            4D offset = i × (Depth x Rows × Columns)
                        + j × (Rows x Columns)
                        + k x Columns
                        + l

            */

            /*
            to jo logic bna he 
            suppose:
                indexes = [4,5,6,6]  = [i,j,k,l]
                dimensions = [5,6,8,9] [outerdepth, Depth,Rows,Columns]

            humara result bnta he:
               idx=       i × (Depth x Rows × Columns)
                        + j × (Rows x Columns)
                        + k x Columns
                        + l

            to humne loop 3 se 0 tk chlana he 
            l k liye multiplier 1
            k  k liye multiplier next 9
            j k liye 9*8 = 72
            i k liye 72* 6 = somethin
            ans so on 

            mtlb huumk loop chlana he n-1 se zero 
            indexes se nth wali value utha k  multipier ki current value se multiply kr k result ko idx me add krna he
            dimension se nth wali value nikl k multiplier k sath multiply krna he

            means 
                  idx =idx + indexes[nth]*multiplier  // last wale k liye multiplier 1 rahega
                  multiplier = multiplier * dimensions[nth]   //ab ye value next iteration me kaam aa jayegi.
            
                bs har iteration me ye 2 opeartion krna he and loop khtm hone k baad idx reurn krna he
                to ye flat index hoga.

            uint32_t multiplier = 1;
            idx=0;
            for(x=indexes.size()-1;x>=0;--x)  sarre indexes pr loop lagega
            {
                x will go for 4-1 to 0 means 3 to 0;

                if(indexes[x]>=this->dimensions[x]){
                check for outof bound 
                }
                iteration 1:
                    x=3;
                    multiplier= 1
                    indexes[3] = 6
                    dimensions[3] =9
               
                idx += indexes[x]*multiplier = 0+= 6*1; = 6
                multiplier *= this->dimensions[x]; *= 9 =9
                 after first iteration complete


            }
            */
            return idx;

            
        }
    public:
        NDArray(const std::vector<uint32_t>& dimensions);
        NDArray(const NDArray& other); // Copy constructor
        NDArray(NDArray&& other); // Move constructor
        NDArray& operator=(const NDArray& other); // Copy assignment
        NDArray& operator=(NDArray&& other); // Move assignment
        ~NDArray();

        template<typename... TT>
        void set(TT ...);

        template<typename... TT>
        T1 get(TT ...);

        NDArrayNode operator[](uint32_t index);
        friend class NDArrayNode; // because NDArrayNode needs access to private members of NDArray like collection and dimensions

        void set(const std::vector<uint32_t>& indexes, T1 value);
        T1 get(const std::vector<uint32_t>& indexes);
        NDArrayNode* getNode(const std::vector<uint32_t>& indexes);
        void printDimensions();
        std::vector<uint32_t> getDimensions() const; // const isliye kiya ki contract ki iske sath koi modification nahi hoga.




        void just_do_it(vector<int> &v,vector<int> from,vector<int> to)
        {
            int i,k;
            int last_index=from.size()-1;
            vector<int> tmp_indices= from;
            while(1)
            {
                // process current combination
                while(tmp_indices[last_index]<=to[last_index])
                {
                    // do whatever you want with current combination stored in tmp_indices
                    for(auto x: tmp_indices)
                    {
                        cout<<x<<" ";
                    }
                    cout<<endl;
                    tmp_indices[last_index]++; // meansiing move to next combination
                }
                // move to next combination
                k=last_index-1; // start from the second last index
                while(k>=0 && tmp_indices[k]>to[k]) k--; // find the rightmost index which can be incremented
                if(k<0) break; // all combinations are processed
                tmp_indices[k]++; // increment this index
                k++;
                while(k<=last_index)
                {
                    tmp_indices[k]=from[k]; // reset all indices to the right
                    k++;
                }
            }
        }
        void print(){
            // iska likna he.
            int i,k;
            // vector<uint32_t> from(dimensions.size(), 0);
            vector<uint32_t> from;
            vector<uint32_t> to;
            // for(i=0;i<dimensions.size();i++)
            // {
            //     from.push_back(0);
            //     to.push_back(dimensions[i]-1);
            // }
             for(i=0;i<this->dimensions.size();i++)
            {
                from.push_back(0);
                to.push_back(this->dimensions[i]-1);
            }
            // above loop se from and to vector ban gya
            int last_index=from.size()-1;
            vector<uint32_t> tmp_indices= from;
            while (1){
                // process current combination
                while(tmp_indices[last_index]<=to[last_index])
                {
                    // do whatever you want with current combination stored in tmp_indices
                    // ndArray.set(tmp_indices, value);
                    for(auto x: tmp_indices)
                    {
                        cout<<"["<<x<<" ]";
                    }
                        // come here 
                    // uint32_t idx = getIndex( tmp_indices);
                    cout<<"="<<this->collection[this->getIndex(tmp_indices)]<<endl;
                    tmp_indices[last_index]++; // meansiing move to next combination
                }
                k=last_index-1; // start from the second last index
                // while(k>=0 && tmp_indices[k]>to[k]) k--; // find the rightmost index which can be incremented
                //  upar wale loop  se index out of bound ho rha tha isliye neeche wale loop se kr rhe he
                while(k>=0 && tmp_indices[k]==to[k]) k--; // find the rightmost index which can be incremented
                if(k<0) break; // all combinations are processed
                tmp_indices[k]++; // increment this index
                k++;
                while(k<=last_index)
                {
                    tmp_indices[k]=from[k]; // reset all indices to the right
                    k++;
                }
            }
            
        }
        //  fill krna , means konse index se konse index tk fill krna he , and kya value se fill krna he
        static void fill(NDArray &ndArray, vector<uint32_t> from, vector<uint32_t> to, T1 value)
        {
            int i,k;
            int last_index=from.size()-1;
            vector<uint32_t> tmp_indices= from;
            while(1)
            {
                // process current combination
                while(tmp_indices[last_index]<=to[last_index])
                {
                    // do whatever you want with current combination stored in tmp_indices
                    // ndArray.set(tmp_indices, value);
                    ndArray.collection[ndArray.getIndex(tmp_indices)] = value;
                    tmp_indices[last_index]++; // meansiing move to next combination
                }
                // move to next combination
                k=last_index-1; // start from the second last index
                while(k>=0 && tmp_indices[k]>to[k]) k--; // find the rightmost index which can be incremented
                if(k<0) break; // all combinations are processed
                tmp_indices[k]++; // increment this index
                k++;
                while(k<=last_index)
                {
                    tmp_indices[k]=from[k]; // reset all indices to the right
                    k++;
                }
            }
        }

        static void copyWithOffsetError(NDArray &target,vector<uint32_t> target_from
                         ,NDArray &source,vector<uint32_t> source_from,vector<uint32_t> source_to)
        {

            // validation not doing right now and not computing target_to right now
            int i,k;
            T1 value;
            int last_index=source_from.size()-1;
            vector<uint32_t> source_tmp_indeces= source_from;
            vector<uint32_t> target_tmp_indices= target_from;
            while(1)
            {
                // process current combination
                while(source_tmp_indeces[last_index]<=source_to[last_index])
                {
                    // do whatever you want with current combination stored in tmp_indices
                    // ndArray.set(tmp_indices, value);
                   value = source.collection[source.getIndex(source_tmp_indeces)];
                   target.collection[target.getIndex(target_tmp_indices)] = value;
                    target_tmp_indices[last_index]++; // meansiing move to next combination
                    source_tmp_indeces[last_index]++;  // meansiing move to next combination but why? because we are copying from source to target
                }
                // move to next combination
                k=last_index-1; // start from the second last index
                while(k>=0 && source_tmp_indeces[k]>source_to[k]) k--; // find the rightmost index which can be incremented
                if(k<0) break; // all combinations are processed
                target_tmp_indices[k]++; // increment this index
                source_tmp_indeces[k]++;
                k++;
                while(k<=last_index)
                {
                    target_tmp_indices[k]=target_from[k]; // reset all indices to the right
                    source_tmp_indeces[k]=source_from[k];
                    k++;
                }
            }
        }
        // ye upar wale copy fucntion me offfset ka issue tha jo ab solve krege.
        // ye problem thi
        // NDArray::copy(ndArray4D,{2,0,2,3},ndArray,{1,1},{4,2});
            //  upar wale line me index out of bound rha he uska reason
            // copy ka code  chla ,last index calculate ki baat hui
            // lastindex source k from index wale vector se calculate kiya , source k from vector ka size 2 he to last index 1 aaya
            // agar target k from se last index calculate krte to 3 aata kyuki target k from vector ka size 4 he
            // ab for current soultion lastindex 1 he to niche wale operation se naya index bnega
            //  source_tmp_indeces[last_index]++; [1,1] become [1,2] 
            // target_tmp_indices[last_index]++; [2,0,2,3] become [2,1,2,3] jabki [2,0,2,4] hona tha means expected tha.
            // iska solutino: sourcce k from vector and targer k from vector ki size ka jo differnece he usko offset maan k us offset ka use kr k dono index ko 
            // same index pr point krwana he.
            // to abhi pura print() method chagne hoga.
        // ab iske solution k liye 3 conditions bnegei
        // 1. source_from.size() == target_from.size()  -> ye case upar wale jesa he chlega
        // 2. source_from.size() < target_from.size()  -> target_from ka size jyada he to hume offset calculate krna he
        // 3. source_from.size() > target_from.size()  -> source_from ka size jyada he to hume offset calculate krna he
        // is offset use kese krna he?
        // maan lo source ka size 2 he and target ka size 4 he to offset 2 aayega
        // ab jab last_index calculate krenge to source ka size lenge
        // last_index=source_from.size()-1;
        // ab jab bhi source_tmp_indeces or target_tmp_indices ko access krenge to  
        // source_tmp_indeces[i]  and target_tmp_indices[i + offset]  aise krenge
        // same cheez jab source ka size jyada he to krenge 
        // offset negative hoga to target_tmp_indices[i - offset]  aise krenge
        // ye karne k baad dono index same element pr point krenge


        // chlo krte he
        // final copy function after fixing offset issue
        // ye function ab 3 case handle kr lega
        // offset logic is working well
        // copyWorkingfor2D4D and vice versa bhi kaam krega ye kr k dekh liya he


        /*
        Note:
            lastindex=3
            offset -2
            soure  dimension  4
            target dimension  2

            from 2023
            to   2046

            left offset right offset lene ki jarurat nahi he vo
            mathematically kaam kr jayega
            workout kr k dekhna he
        */
        static void copy(NDArray &target,vector<uint32_t> target_from
                         ,NDArray &source,vector<uint32_t> source_from,vector<uint32_t> source_to)
        {
            int i,k;
            T1 value;
            int source_size = source_from.size();
            int target_size = target_from.size();
            int last_index = source_size - 1;
            vector<uint32_t> source_tmp_indeces = source_from;
            vector<uint32_t> target_tmp_indices = target_from;

            int offset = target_size - source_size; // Calculate offset

            // what to do when offset is negetive and when offset is positive?
            //  source offset and target offset calculate krege
            // seee
            // 

            while (1) {
                // process current combination
                while (source_tmp_indeces[last_index] <= source_to[last_index]) {
                    // do whatever you want with current combination stored in tmp_indices
                    value = source.collection[source.getIndex(source_tmp_indeces)];
                    target.collection[target.getIndex(target_tmp_indices)] = value;
                    target_tmp_indices[last_index + offset]++; // Move to next combination in target
                    source_tmp_indeces[last_index]++;  // Move to next combination in source
                }
                // move to next combination
                k = last_index - 1; // start from the second last index
                while (k >= 0 && source_tmp_indeces[k] == source_to[k]) k--; // find the rightmost index which can be incremented
                if (k < 0) break; // all combinations are processed
                target_tmp_indices[k + offset]++; // increment this index in target
                source_tmp_indeces[k]++;
                k++;
                while (k <= last_index) {
                    target_tmp_indices[k + offset] = target_from[k + offset]; // reset all indices to the right in target
                    source_tmp_indeces[k] = source_from[k];
                    k++;
                }
            }
        }



        //  multiply 3Darray
        // [2,2,3] [2,3,2]
        // note: “Only 2D matrices have a defined multiplication in linear algebra.
        // 3D arrays are tensors, and their multiplication depends on the chosen tensor operation.
        // Normal matrix math = only 2D, strict rules
        // “3D matrix multiplication” = many 2D multiplications or tensor math, defined by libraries

        
        static void multiplication3D(NDArray  &array1, NDArray  &array2, NDArray *arrayResult)
        {
         
              // print dimensions of left and right operand
                // for(auto d:dimensions_m1) cout<<d<<" ";
                // cout<<endl;
                // for(auto d:dimensions_m2) cout<< d<<" ";
                // cout<<endl;
                // for(auto d:dimensions_m3) cout<< d<<" ";
                // cout<<endl;

            std::vector<uint32_t> dimensions_m1 = array1.getDimensions();
            std::vector<uint32_t> dimensions_m2 = array2.getDimensions();
            std::vector<uint32_t> dimensions_m3 = (*arrayResult).getDimensions();
            // validation to be applied
            // if array1[0] != array2[0] then issue
            // if array1[2] != array2[1] then issue
            // if(arrayResult[1]!= array1[1] && arrayResult[2] != array2[2]) // constainer size isse


            uint32_t depth = dimensions_m1[0];
            uint32_t rows = dimensions_m1[1];
            uint32_t cols = dimensions_m2[2];
            uint32_t common = dimensions_m2[1];
            uint32_t i,r,c,j;
            double sum=0.0;
            for(i=0;i<depth;i++)
            {
                    for(r=0;r<rows;r++)
                    {
                        for(c=0;c<cols;c++)
                        {
                            sum=0.0;
                            for(j=0;j<common;j++)
                            {
                                sum+=array1.collection[array1.getIndex({i,r,j})]* array2.collection[array2.getIndex({i,j,c})];
                            } 
                            (*arrayResult).collection[(*arrayResult).getIndex({i,r,c})] = sum;
                        }
                    }
            }
        }


        static void multiplication3DOptimizeWithcomment(NDArray  &array1, NDArray  &array2, NDArray *arrayResult)
        {
         
            std::vector<uint32_t> dimensions_m1 = array1.getDimensions();
            std::vector<uint32_t> dimensions_m2 = array2.getDimensions();
            std::vector<uint32_t> dimensions_m3 = (*arrayResult).getDimensions();
            // validation to be applied
            // if array1[0] != array2[0] then issue
            // if array1[2] != array2[1] then issue
            // if(arrayResult[1]!= array1[1] && arrayResult[2] != array2[2]) // constainer size isse


            uint32_t depth = dimensions_m1[0];
            uint32_t matrix1_rows = dimensions_m1[1];
            uint32_t matrix2_rows = dimensions_m2[1];
            uint32_t matrix2_columns = dimensions_m1[2];
            uint32_t i,r,c,j;

            //  populate matrix with 0 , since we need to accumulate product
            for(i=0;i<depth;i++)
            {
                    for(r=0;r<matrix1_rows;r++)
                    {
                        for(c=0;c<matrix2_columns;c++)
                                (*arrayResult).collection[(*arrayResult).getIndex({i,r,c})]=0;
                            
                    }
            }

            // do multiplication
             for(i=0;i<depth;i++)
            {
                    // for(r=0;r<rows;r++)
                    // {
                    //     for(c=0;c<common;c++)
                    //     {
                    //         for(j=0;j<cols;j++)
                    //         {
                    //             // (*arrayResult).collection[(*arrayResult).getIndex({i,r,c})]+=array1.collection[array1.getIndex({i,r,j})]* array2.collection[array2.getIndex({i,j,c})];
                    //             //  abhi kya hua ki j columns k upar chl rhaa he and c common k upar to upar wala fomula me j and c ka replcement hoga.
                    //             (*arrayResult).collection[(*arrayResult).getIndex({i,r,j})]+=array1.collection[array1.getIndex({i,r,c})]* array2.collection[array2.getIndex({i,c,j})];
                    //         } 
                    //     }
                    // }


                    //  agar address calculation wala formula wahi rkha he irc = irj * ijc
                    //  yahi rkhna he to legscy me common and cols wale loop ko exchage kr do.
                     for(r=0;r<matrix1_rows;r++)
                    {
                        for(j=0;j<matrix2_rows;j++)
                        {

                            // ye wala loop isliye ki don rows ka multiplication saare columns k liye
                            for(c=0;c<matrix2_columns;c++)
                            {
                                // (*arrayResult).collection[(*arrayResult).getIndex({i,r,c})]+=array1.collection[array1.getIndex({i,r,j})]* array2.collection[array2.getIndex({i,j,c})];
                                //  abhi kya hua ki j columns k upar chl rhaa he and c common k upar to upar wala fomula me j and c ka replcement hoga.
                                (*arrayResult).collection[(*arrayResult).getIndex({i,r,c})]+=array1.collection[array1.getIndex({i,r,j})]* array2.collection[array2.getIndex({i,j,c})];
                            } 
                        }
                    }
            // hum 3 loops kyu lagate he
                /*
                1. for(r=0;r<rows;r++) // left wale ki saari rows 
                2. for(c=0;c<cols;c++) // right wale k saare columns
                 
                in se to result ki addressing ho jayegi , means r and c se result matrix ki cell ka pata lgane kaa kaam ho jayega
                but left and right wale ki cells ko kese access kre?
                kyuki left wale ka column index missing and right wale k row index missins

                to iske liye ek or loop lgta he vo common index k upar wale k upar
                3. for(j=0;j<common;j++)

                isme j ko r k sath lekr left wale k cells ko fetch kiya jaata he 
                and  j ko c sath lekr right wale k cells ko fetch kiya jaata he.

                jb j wale loop k har iteration se r ka combo bnaya jata he to left wale ki puri row mil jaati he
                jb j wale loop k har iteraiotn se c ka combo bnaya jaata he to right wale ka pura column mil jaata he

                konsi row and konsa column milta he?
                ans: rth row and cth column

                humara paar row bhi aa gayi column bhi aa gaya ab respective cell ka multiply kr k result sum me
                and vo sum r,c se reuslt ka index pta kr k usme daal diya


                but optimize way me:

                humara focus left ki row ka right ki row se multiply and result ki row me accumulate
                to
                1. for(r=0;r<rows;r++)   // left wale ki puro row 
                2. for(c=0;c<rightrows;c++) // right wale ki puri row ya left wale k columns bhi bol skte he

                ab r and c se left ki cells to bn jayegi 
                but resultwale ki and right wale ki cell bnane k liye column chahiye
                result wale ki row indexing r se ho jayegi 
                right wale ki row indexing c se ho jayegi 
                
                but dono ki column indexing to right k columns pr loop laga k hogi 

                to suppose vo loop j pr lagate he 
                3. for(j=0;j<rightcols;j++)
                to teeno matrix ki cells ese bnegi
                left: r*c     
                right: c*j
                result r*j
                ab inke columns nikalo and sui se indexing bnegi

                left k columns: leftcols , index r*c to formuls     r*leftcols  +c
                right k columns : rightcols  index c*j to formula   c*rightcols +j
                result k columns: resultcols  index r*j to formula  r*resultcols + j
              
                ND array ki get index ko pass:
                result (i,r,j)
                left   (i,r,c)
                right: (i,c,j)
                (*arrayResult).collection[(*arrayResult).getIndex({i,r,j})]+=
                                array1.collection[array1.getIndex({i,r,c})]* 
                                array2.collection[array2.getIndex({i,c,j})];
                */
            }
        }
        


         static void multiplication3DOptimize(NDArray  &array1, NDArray  &array2, NDArray *arrayResult)
        {
         
            std::vector<uint32_t> dimensions_m1 = array1.getDimensions();
            std::vector<uint32_t> dimensions_m2 = array2.getDimensions();
            std::vector<uint32_t> dimensions_m3 = (*arrayResult).getDimensions();
            // validation to be applied
            uint32_t depth = dimensions_m1[0];
            uint32_t matrix1_rows = dimensions_m1[1];
            uint32_t matrix2_rows = dimensions_m2[1];
            uint32_t matrix2_columns = dimensions_m2[2];
            uint32_t i,r,c,j;
            //  populate matrix with 0 , since we need to accumulate product
            for(i=0;i<depth;i++)
            {
                    for(r=0;r<matrix1_rows;r++)
                    {
                        for(c=0;c<matrix2_columns;c++)
                                (*arrayResult).collection[(*arrayResult).getIndex({i,r,c})]=0;
                            
                    }
            }

            // multiplication
             for(i=0;i<depth;i++)
            {
                     for(r=0;r<matrix1_rows;r++)
                    {
                        for(j=0;j<matrix2_rows;j++)
                        {
                            for(c=0;c<matrix2_columns;c++)
                                (*arrayResult).collection[(*arrayResult).getIndex({i,r,c})]+=array1.collection[array1.getIndex({i,r,j})]* array2.collection[array2.getIndex({i,j,c})];
                        }
                    }
            }
        }
        
        static void copyBasic(NDArray &target,vector<uint32_t> target_from
                         ,NDArray &source,vector<uint32_t> source_from,vector<uint32_t> source_to)
        {

            // validation not doing right now and not computing target_to right now
            int i,k;
            T1 value;
            int last_index=source_from.size()-1;
            vector<uint32_t> source_tmp_indeces= source_from;
            vector<uint32_t> target_tmp_indices= target_from;
            while(1)
            {
                // process current combination
                while(source_tmp_indeces[last_index]<=source_to[last_index])
                {
                    // do whatever you want with current combination stored in tmp_indices
                    // ndArray.set(tmp_indices, value);
                   value = source.collection[source.getIndex(source_tmp_indeces)];
                   target.collection[target.getIndex(target_tmp_indices)] = value;
                    target_tmp_indices[last_index]++; // meansiing move to next combination
                    source_tmp_indeces[last_index]++;  // meansiing move to next combination but why? because we are copying from source to target
                }
                // move to next combination
                k=last_index-1; // start from the second last index
                while(k>=0 && source_tmp_indeces[k]>source_to[k]) k--; // find the rightmost index which can be incremented
                if(k<0) break; // all combinations are processed
                target_tmp_indices[k]++; // increment this index
                source_tmp_indeces[k]++;
                k++;
                while(k<=last_index)
                {
                    target_tmp_indices[k]=target_from[k]; // reset all indices to the right
                    source_tmp_indeces[k]=source_from[k];
                    k++;
                }
            }
        }
};

class NDArrayNode {
    private:
        NDArray* ndArray;
        vector<uint32_t> indexes;
        NDArrayNode(NDArray* _ndArray, const vector<uint32_t>& _indexes);
        uint32_t getIndex() const; // get the linear index in the NDArray's collection corresponding to this node's indexes
    public:
        // NDArrayNode operator[](uint32_t index);
        NDArrayNode& operator[](uint32_t index);
        // void set(T1 value);
        // T1 get() const;
        operator T1(); // Conversion operator to T1
        T1 operator=(T1 value); // Assignment operator
        ~NDArrayNode();


        NDArrayNode(const NDArrayNode& other); // Copy constructor
        NDArrayNode(NDArrayNode&& other); // Move constructor
        NDArrayNode & operator=(const NDArrayNode& other); // Copy assignment operator
        NDArrayNode & operator=(NDArrayNode&& other); // Move assignment operator
        friend class NDArray;
    };



    // NDArray method definitions

    NDArray::NDArray(const std::vector<uint32_t>& _dimensions){
       if(_dimensions.size() == 0)
       {
           throw NDArrayException("Dimensions cannot be zero");  // refactor the message later
       }
       int product= std::accumulate(_dimensions.begin(), _dimensions.end(), 1, std::multiplies<uint32_t>());
       if(product==0){
        ostringstream oss;
        oss<<"One of the dimensions is zero: "; // refactor the message later
        for(uint32_t x: _dimensions){
            oss<<"["<<x<<"]";
        }
        throw NDArrayException(oss.str());
       }
       this->dimensions = _dimensions;
        this->collection.resize(product); // flat array ka size set kr diya
    }
    //  copuy constructor
    NDArray::NDArray(const NDArray& other) : collection(other.collection), dimensions(other.dimensions) {
        // Copy constructor
        // ya esa bhi likh skte he
        // this->collection = other.collection;
        // this->dimensions = other.dimensions;
    }
    // move constructor
    NDArray::NDArray(NDArray&& other)  {

       // Move constructor
       this->collection = std::move(other.collection);
       this->dimensions = std::move(other.dimensions);
       for(NDArrayNode* node : other.ndArrayNodes) {
           node->ndArray = NULL; // Update the ndArray pointer in each node
           node->indexes.resize(0); // Transfer ownership of nodes
       }
       other.ndArrayNodes.clear(); // Clear the nodes in the moved-from object
    }

    //destructor
    NDArray::~NDArray() {
        // Destructor to clean up NDArrayNodes
        for (NDArrayNode* node : this->ndArrayNodes) {
            node->ndArray = NULL; // Update the ndArray pointer in each node
           node->indexes.resize(0); // Clear indexes to avoid dangling references
        }
    }

    // copy assignment
    NDArray& NDArray::operator=(const NDArray& other) {
        // if (this != &other) {
        //     this->collection = other.collection;
        //     this->dimensions = other.dimensions;
        //     // Note: NDArrayNodes are not copied; they should be created anew if needed
        // }
         for (NDArrayNode* node : this->ndArrayNodes) {
            node->ndArray = NULL; // Update the ndArray pointer in each node
           node->indexes.resize(0); // Clear indexes to avoid dangling references
        }
        ndArrayNodes.clear(); // Clear existing nodes
        this->collection = other.collection;
        this->dimensions = other.dimensions;
        return *this;
    }

    // move assignment
    NDArray& NDArray::operator=(NDArray&& other) {  
        for (NDArrayNode* node : this->ndArrayNodes) {
            node->ndArray = NULL; // Update the ndArray pointer in each node
           node->indexes.resize(0); // Clear indexes to avoid dangling references
        }
        ndArrayNodes.clear(); // Clear existing nodes
        this->collection = std::move(other.collection);
        this->dimensions = std::move(other.dimensions);
        for(NDArrayNode* node : other.ndArrayNodes) {
            node->ndArray = NULL; // Update the ndArray pointer in each node
            node->indexes.resize(0); // Transfer ownership of nodes
        }
        other.ndArrayNodes.clear(); // Clear the nodes in the moved-from object
        return *this;
    }

    void NDArray::_set(uint32_t index, T1 value) {
        if(index >= this->collection.size()) {
            throw NDArrayException("Index out of bounds in _set");
        }
        this->collection[index] = value;
    }
    T1 NDArray::_get(uint32_t index) {
        if(index >= this->collection.size()) {
            throw NDArrayException("Index out of bounds in _get");
        }
        // sir k ss me ye check nahi kiya uska reason he ki ye function private he and sirf valid index ke sath hi call hoga
        return this->collection[index];
    }


    template<typename... TT>
    void NDArray::set(TT ... arguments) {
        // std::vector<uint32_t> indexes = {static_cast<uint32_t>(arguments)...};
        // T1 value = indexes.back();
        vector<uint32_t> indexes;
        double value;
        // (indexes.push_back(static_cast<uint32_t>(arguments)), ...);
        (indexes.push_back((uint32_t)arguments), ...);  // pack expansion to populate indexes
        ((value=arguments), ...); // last argument will be assigned to value
        indexes.pop_back();  // remove the last element which is the value

        if(indexes.size() != this->dimensions.size()) {
            throw NDArrayException("Number of indexes does not match number of dimensions in set");
            // part 14 page 36 se message ko refactor krna he
        }
        

        uint64_t idx;
        int x;
        uint32_t multiplier = 1;
        idx=0;
        for(x=indexes.size()-1;x>=0;--x)
        {
            if(indexes[x]>=this->dimensions[x])
            {
                ostringstream oss;
                oss<<"Index out of bounds 3";
                for(auto j : indexes)
                {
                    oss<<" ["<<j<<"] ";
                }
                oss<<"size of array is ";
                for(auto j : this->dimensions)
                {
                    oss<<" ["<<j<<"] ";
                }
                throw NDArrayException(oss.str());
            }
            idx += indexes[x]*multiplier;
            multiplier *= dimensions[x];
        }
        // _set(idx, value);
        this->collection[idx] = value;
    }

    template<typename... TT>
    T1 NDArray::get(TT ... arguments) {
        // std::vector<uint32_t> indexes = {static_cast<uint32_t>(arguments)...};
        vector<uint32_t> indexes;
        (indexes.push_back((uint32_t)arguments), ...);  // pack expansion to populate indexes

        if(indexes.size() != this->dimensions.size()) {
            throw NDArrayException("Number of indexes does not match number of dimensions in get");
            // part 14 page 36 se message ko refactor krna he
        }

        uint64_t idx;
        int x;
        uint32_t multiplier = 1;
        idx=0;
        for(x=indexes.size()-1;x>=0;--x)
        {
            if(indexes[x]>=this->dimensions[x])
            {
                ostringstream oss;
                oss<<"Index out of bounds 4";
                for(auto j : indexes)
                {
                    oss<<" ["<<j<<"] ";
                }
                oss<<"size of array is ";
                for(auto j : this->dimensions)
                {
                    oss<<" ["<<j<<"] ";
                }
                throw NDArrayException(oss.str());
            }
            idx += indexes[x]*multiplier;
            multiplier *= dimensions[x];
        }
        // return _get(idx);
        return this->collection[idx];
    }


    NDArrayNode  NDArray::operator[](uint32_t index) {
        // vector<uint32_t> idx_vector = {index};
        // return NDArrayNode(this, idx_vector);
        NDArrayNode node(this, {index});
        return  node;
    }

    std::vector<uint32_t> NDArray::getDimensions() const{
        // yah pr copy jayegi ye shallow copy ye dekhte he baad me
        return dimensions;
    }

// NDArrayNode method definitions
// constructor
NDArrayNode::NDArrayNode(NDArray* _ndArray, const vector<uint32_t>& _indexes) {
    this->ndArray = _ndArray;
    this->indexes = _indexes;
    // Register this node with the NDArray
    if(this->ndArray != NULL) {
        this->ndArray->ndArrayNodes.insert(this);
    }
}
//copy constructor
NDArrayNode::NDArrayNode(const NDArrayNode& other) {
    this->ndArray = other.ndArray;
    this->indexes = other.indexes;
    // Register this node with the NDArray
    if(this->ndArray != NULL) {
        this->ndArray->ndArrayNodes.insert(this);
    }
}
// move constructor
NDArrayNode::NDArrayNode(NDArrayNode&& other) {
    this->ndArray = other.ndArray;
    // this->indexes = std::move(other.indexes);
    this->indexes = other.indexes;
    if(this->ndArray != NULL) {
        this->ndArray->ndArrayNodes.insert(this);
    }
    // Update the ndArrayNodes set
}


// copy assignment operator
NDArrayNode& NDArrayNode::operator=(const NDArrayNode& other) {
    // if (this != &other) {
    //     // Deregister this node from the current NDArray
    //     if(this->ndArray != NULL) {
    //         this->ndArray->ndArrayNodes.erase(this);
    //     }
    //     this->ndArray = other.ndArray;
    //     this->indexes = other.indexes;
    //     // Register this node with the new NDArray
    //     if(this->ndArray != NULL) {
    //         this->ndArray->ndArrayNodes.insert(this);
    //     }
    // }
    if(this->ndArray==NULL) throw NDArrayException("NDArrayNode is not associated with any NDArray in copy assignment");
    if(other.ndArray==NULL) throw NDArrayException("Cannot assign from an NDArrayNode not associated with any NDArray in copy assignment");
    this->ndArray->collection[this->getIndex()] = other.ndArray->collection[other.getIndex()];

    return *this;
}
// move assignment operator
NDArrayNode& NDArrayNode::operator=(NDArrayNode&& other) {
     if(this->ndArray==NULL) throw NDArrayException("NDArrayNode is not associated with any NDArray in copy assignment");
    if(other.ndArray==NULL) throw NDArrayException("Cannot assign from an NDArrayNode not associated with any NDArray in copy assignment");
    this->ndArray->collection[this->getIndex()] = other.ndArray->collection[other.getIndex()];

    return *this;
}

// destructor
NDArrayNode::~NDArrayNode() {
    if(this->ndArray){
        this->ndArray->ndArrayNodes.erase(this);
    }
}

//operator[]
// NDArrayNode NDArrayNode::operator[](uint32_t index) {
//     if(this->ndArray == NULL) {
//         throw NDArrayException("NDArrayNode is not associated with any NDArray in operator[]");
//     }
//     uint32_t verify_using_idx = this->indexes.size();
//     vector<uint32_t> tmpvector= this->indexes;
//     tmpvector.push_back(index);
//     NDArrayNode   node(this->ndArray, tmpvector);
//     return node;
// }
// In NDArrayNode class, change operator[] to return reference:
NDArrayNode& NDArrayNode::operator[](uint32_t index) {
    if(this->ndArray == NULL) {
        throw NDArrayException("NDArrayNode is not associated with any NDArray in operator[]");
    }
    vector<uint32_t> tmpvector= this->indexes;
    tmpvector.push_back(index);
    // Create a new node and return reference - but this won't work with temporaries
    // Better solution: don't use this pattern
    NDArrayNode* node = new NDArrayNode(this->ndArray, tmpvector);
    return *node;
}

NDArrayNode::operator T1() {
    if(this->ndArray == NULL) {
        throw NDArrayException("NDArrayNode is not associated with any NDArray in conversion to T1");
    }
    return this->ndArray->collection[this->getIndex()];
}

// assignnmet operatior
T1 NDArrayNode::operator=(T1 value) {
    if(this->ndArray == NULL) {
        throw NDArrayException("NDArrayNode is not associated with any NDArray in assignment");
    }
    this->ndArray->collection[this->getIndex()] = value;
    return value; // because assignment operator should return the assigned value

}

uint32_t NDArrayNode::getIndex() const {

    if(this->indexes.size() != this->ndArray->dimensions.size()) {
        throw NDArrayException("Number of indexes does not match number of dimensions in NDArrayNode getIndex");
    }
    uint64_t idx;
    int x;
    uint32_t multiplier = 1;
    idx=0;
    for(x=this->indexes.size()-1;x>=0;--x){
        if(this->indexes[x]>=this->ndArray->dimensions[x]){
            ostringstream oss;
            oss<<"Index out of bounds in NDArrayNode getIndex";
            for(auto j : this->indexes){
                oss<<" ["<<j<<"] ";
            }
            oss<<"size of array is ";
            for(auto j : this->ndArray->dimensions){
                oss<<" ["<<j<<"] ";
            }
            throw NDArrayException(oss.str());
        }
        idx += this->indexes[x]*multiplier;
        multiplier *= this->ndArray->dimensions[x];
    }
    return idx; 
}


//  multiply 3D array




int mainreal() {
    // Test your NDArray implementation here
cout<<"NDArray implementation started"<<endl;

try {

    NDArray ndArray({5,5});
    double value=1.0;
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
                // ndArray.set(i,j, value);
                // value+=1.0;
            //ndArray[i][j] = value++;
        }
    }
    //ndArray.print();

    //cout<<"Just for separating the outputs"<<endl;
                // now we are going to create 4d arrray and will copy 2d into that
    // NDArray ndArray4D({3,2,15,15});
    NDArray ndArray4D({3,2,5,5});
                // come here , abhi copy krna he
    //NDArray::copy(ndArray4D,{2,0,2,3},ndArray,{1,1},{4,2});
                //  upar wale line me index out of bound rha he uska reason
                // copy ka code  chla ,last index calculate ki baat hui
                // lastindex source k from index wale vector se calculate kiya , source k from vector ka size 2 he to last index 1 aaya
                // agar target k from se last index calculate krte to 3 aata kyuki target k from vector ka size 4 he
                // ab for current soultion lastindex 1 he to niche wale operation se naya index bnega
                //  source_tmp_indeces[last_index]++; [1,1] become [1,2] 
                // target_tmp_indices[last_index]++; [2,0,2,3] become [2,1,2,3] jabki [2,0,2,4] hona tha means expected tha.
                // iska solutino: sourcce k from vector and targer k from vector ki size ka jo differnece he usko offset maan k us offset ka use kr k dono index ko 
                // same index pr point krwana he.
                // to abhi pura print() method chagne hoga.
    //cout<<"After copying 2D array into 4D array"<<endl;
    //ndArray4D.print();





    // Vice versa
    //  abhi 4d array me data dalege and usko udr 2d me copy krege
    int f,t;   // f and t are fourth and third dimensions
    // NDArray ndArray4D({3,2,15,15});
    f=1;
    t=0;   
    // means 2nd 3D array k first 2d Array me data fill krege
    value=1.0;
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            // ndArray.set(i,j, value);
            // value+=1.0;
            ndArray4D[f][t][i][j] = value+10;
            value++;
        }
    }
        // abhi 2d array 15*15 ka he but humne 5*5 me data data he.
        // ab isi ko 5*5 k 2d me copy kr k print krte he
    // NDArray::copy(ndArray4D,{2,0,2,3},ndArray,{1,1},{4,2});
    //ndArray4D.print();
    //cout<<"After copying 4D array into 2D array"<<endl;
    // NDArray::copy(ndArray,{0,0},ndArray4D,{1,0,0,0},{1,0,3,3});
    NDArray::copy(ndArray,{0,0},ndArray4D,{1,0,0,0},{1,0,4,4});
    //ndArray.print();



    //  ndarray ka copy ka test kr k dekh liya he.
    // next: multiplication 3d
    //  NDArray ndArrayLeft({2,2,5});
    //  NDArray ndArrayRight({2,5,2});
    //  NDArray ndArrayResult({2,2,2});
      NDArray ndArrayLeft({2,2,3});
     NDArray ndArrayRight({2,3,2});
     NDArray ndArrayResult({2,2,2});
    // left ka first 2d
     ndArrayLeft[0][0][0]=1;
     ndArrayLeft[0][0][1]=2;
     ndArrayLeft[0][0][2]=3;
     ndArrayLeft[0][1][0]=4;
     ndArrayLeft[0][1][1]=5;
     ndArrayLeft[0][1][2]=6;
    // left ka second 2d
     ndArrayLeft[1][0][0]=7;
     ndArrayLeft[1][0][1]=8;
     ndArrayLeft[1][0][2]=9;
     ndArrayLeft[1][1][0]=1;
     ndArrayLeft[1][1][1]=2;
     ndArrayLeft[1][1][2]=3;

    // right first
      ndArrayRight[0][0][0]=1;
      ndArrayRight[0][0][1]=0;
      ndArrayRight[0][1][0]=0;
      ndArrayRight[0][1][1]=1;
      ndArrayRight[0][2][0]=1;
      ndArrayRight[0][2][1]=1;

    // right second
      ndArrayRight[1][0][0]=1;
      ndArrayRight[1][0][1]=1;
      ndArrayRight[1][1][0]=1;
      ndArrayRight[1][1][1]=0;
      ndArrayRight[1][2][0]=0;
      ndArrayRight[1][2][1]=1;



    // isko static isliye rkha kyuki ye kisi object pr nahi call hogi
    // ye utility he class name k reference se call hogi
     NDArray::multiplication3D(ndArrayLeft,ndArrayRight,&ndArrayResult);
    //  multiplication result
    cout<<endl<<"mmultiplication result :- "<<endl;
     (ndArrayResult).print();
    
}catch(const NDArrayException &e){


    cout<<"NDArrayException caught: "<<e.what()<<endl;
}
    return 0;
}




int main() {

try {


    NDArray ndArrayLeft({2,2,3});
     NDArray ndArrayRight({2,3,2});
     NDArray ndArrayResult({2,2,2});
    // left first 2d
     ndArrayLeft[0][0][0]=1;
     ndArrayLeft[0][0][1]=2;
     ndArrayLeft[0][0][2]=3;
     ndArrayLeft[0][1][0]=4;
     ndArrayLeft[0][1][1]=5;
     ndArrayLeft[0][1][2]=6;
    // left second 2d
     ndArrayLeft[1][0][0]=7;
     ndArrayLeft[1][0][1]=8;
     ndArrayLeft[1][0][2]=9;
     ndArrayLeft[1][1][0]=1;
     ndArrayLeft[1][1][1]=2;
     ndArrayLeft[1][1][2]=3;

    // right first
      ndArrayRight[0][0][0]=1;
      ndArrayRight[0][0][1]=0;
      ndArrayRight[0][1][0]=0;
      ndArrayRight[0][1][1]=1;
      ndArrayRight[0][2][0]=1;
      ndArrayRight[0][2][1]=1;

    // right second
      ndArrayRight[1][0][0]=1;
      ndArrayRight[1][0][1]=1;
      ndArrayRight[1][1][0]=1;
      ndArrayRight[1][1][1]=0;
      ndArrayRight[1][2][0]=0;
      ndArrayRight[1][2][1]=1;

    //  NDArray::multiplication3D(ndArrayLeft,ndArrayRight,&ndArrayResult);
     NDArray::multiplication3DOptimize(ndArrayLeft,ndArrayRight,&ndArrayResult);
     
    //  multiplication result
    cout<<endl<<"mmultiplication result :-- "<<endl;
     ndArrayResult.print();
    
}catch(const NDArrayException &e){


    cout<<"NDArrayException caught: "<<e.what()<<endl;
}
    return 0;
}
