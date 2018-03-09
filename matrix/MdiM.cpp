#include <iostream>
#include "Matrix.hpp"

void Test4Dimensions()
{
	Matrix<int,4,-1> a;

	a[0][0][0][0]=0;
	a[0][0][0][5]=1;
	a[0][0][0][6]=2;
	a[0][0][0][7]=3;
	//Dummies
	a[0][0][0][1];
	a[0][0][0][2];
	a[0][0][0][3];
	a[0][0][0][4];


	a[0][0][1][0];
	a[0][0][1][1];
	a[0][0][1][2];
	a[0][0][2][0];
	a[0][0][3][0];
	a[0][0][4][0];
	//-----------
	a[0][0][5][0]=10;
	a[0][0][5][1]=11;
	a[0][0][5][2]=12;
	a[0][0][5][3]=13;
	a[1][0][0][0]=1000;
	a[1][0][0][1]=1001;
	a[1][0][0][2]=1002;
	a[1][0][0][3]=1003;
	a[1][0][1][0]=10010;
	a[1][0][1][1]=10011;
	a[1][0][1][2]=10012;
	a[1][0][1][3]=10013;

	std::cout<<"Stored elements:"<<a.size()<<std::endl;

	for(auto e: a)
	{
		int x,y,z,w,v;
		std::tie(x,y,z,w,v)=e;
		std::cout<<"["<<x<<"]["<<y<<"]["<<z<<"]["<<w<<"]="<<v<<std::endl;
	}
}

int main() {


	Matrix<int,2,0> matrix;

	for(int i =0 ;i<10;++i)
	{
		matrix[i][i]=i;
		matrix[i][9-i]=9-i;
	}

	for(int i =1; i<9;++i)
	{
		for(int j=1;j<9;++j)
		{
			int value = matrix[i][j];
			std::cout<<value<<" ";
		}
		std::cout<<std::endl;
	}

	std::cout<<std::endl;

	std::cout<<"Stored elements:"<<matrix.size()<<std::endl;

	for(auto a: matrix)
	{
		int x,y,v;
		std::tie(x,y,v)=a;
		std::cout<<"matrix["<<x<<"]["<<y<<"]="<<v<<std::endl;
	}

	return 0;
}
