#include<NDArrayTransposedView.h>
NDArrayTransposedView::NDArrayTransposedView(NDArray *ndArray,bool is_read_only)
	:ndArray(ndArray),is_read_only(is_read_only)
{
}
InfixExpression<NDArrayTransposedView,NDArray> NDArrayTransposedView::operator*(NDArray &right)
{
	return InfixExpression<NDArrayTransposedView,NDArray>(this,'*',&right);
}

vector<uint32_t> NDArrayTransposedView::get_dimensions()
{
	vector<uint32_t> dimensions;
	if(this->ndArray==nullptr) return dimensions;
	dimensions=this->ndArray->get_dimensions();
	size_t sz=dimensions.size();
	if(sz==1) return dimensions;
	uint32_t r=dimensions[sz-2];
	uint32_t c=dimensions[sz-1];
	dimensions[sz-2]=c;
	dimensions[sz-1]=r;
	return dimensions;
}



