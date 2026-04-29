#include<NDArray.h>
#include<NDArrayView.h>
int main()
{
	NDArray n({5,5,5});
	NDArrayView nv1=n.get_view({0,0,0},{4,4,2},false);
	auto d1=nv1.get_dimensions();
	for(auto k:d1) cout<<k<<",";
	cout<<endl;
	NDArrayView nv2=n.get_view({0,0,2},{0,2,2},false);
	auto d2=nv2.get_dimensions();
	for(auto k:d2) cout<<k<<",";
	cout<<endl;

	return 0;
}
