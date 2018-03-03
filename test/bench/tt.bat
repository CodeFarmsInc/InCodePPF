..\..\codegen -ppf ..\..\ppf -flex ds.def ..\..\lib gen
cl /EHsc library.cpp ..\..\ppf\factory.cpp
