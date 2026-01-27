#include<iostream>
#include<cstddef>
#include<tuple>
#include<vector>
#include<algorithm>
#include<utility>

using namespace std;

template<typename... T>
void sam(T ...k)
{
constexpr size_t j=sizeof...(k);
//cout<<"number of arguments received: "<<j<<endl;


// assignment part extract data from pack 
auto list = make_tuple(k...);
vector<int> indices;
double value= get<j-1>(list);

for_each_n(index_sequence_for<T...>{},[&](auto i){
if(i<j-1)
	indices.push_back(get<i>(list));

});


for(int x:indices) {
cout<<x<<", ";
}
cout<<endl;
cout<<value;
}

int main()
{
	sam(2,3,"Good");
	sam(5,40,50,3.33,3);
	sam();
	return 0;
}
