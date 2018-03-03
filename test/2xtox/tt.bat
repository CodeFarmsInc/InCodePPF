call clean
..\..\codegen -ppf ..\..\ppf -flex ds.def ..\..\lib gen
cl /EHsc mton.cpp ..\..\ppf\factory.cpp
