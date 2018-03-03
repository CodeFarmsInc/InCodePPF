call clean
..\..\codegen -ppf ..\..\ppf -flex ds.def ..\..\lib gen
cl /EHsc mton.cpp ..\..\ppf\factory.cpp
mton 1 > res1
..\diff res1 out
mton 2 > res2
..\diff res2 out
