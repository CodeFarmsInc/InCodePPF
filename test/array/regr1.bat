call clean
..\..\codegen -flex ds.def ..\..\lib gen -ppf c:\dropbox\ppf
cl /EHsc array1.cpp c:\ppf\factory.cpp
array1 1 > res1
array1 2 > res2
array1 3 > res3
array1 4 > res4
..\diff res4 out4
