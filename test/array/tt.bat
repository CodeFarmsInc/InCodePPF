call clean
..\..\codegen -ppf ..\..\ppf -flex ds.def ..\..\lib gen
cl /EHsc array1.cpp ..\..\ppf\factory.cpp
