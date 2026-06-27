#pragma once
template<typename T11,typename T22>
class InfixExpression
{
			public:
			T11 left;
			T22 right;
			char oper;
			InfixExpression(T11 left,char oper,T22 right)
			{
				this->left=left;
				this->right=right;
				this->oper=oper;
			}
};

