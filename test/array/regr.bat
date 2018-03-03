call clean
..\..\codegen -ppf ..\..\ppf ds.def ..\..\lib gen
cl /EHsc array1.cpp ..\..\ppf\factory.cpp
array1 1 > res1
array1 2 > res2
array1 3 > res3
array1 4 > res4
..\diff res4 out4
