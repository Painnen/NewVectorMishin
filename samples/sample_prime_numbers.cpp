
#include <iomanip>
#include "Vector.h"
#include "TMatrix.h"

int main()
{
	TMatrix<int> b(2, 2);
	TMatrix<int> a(2, 2);
	a.Fill(1);
	b.Fill(1);
	TMatrix<int> c = a * b;
	auto res = c.CountOccurrenses(2);
	return 0;
}
