call clean
..\..\codegen -ppf ..\..\ppf ds.def ..\..\lib gen
cl /EHsc hash1.cpp ..\..\ppf\factory.cpp company.cpp employee.cpp
hash1 1 > res1
..\diff res1 out1
hash1 2 > res2
..\diff res2 out2
