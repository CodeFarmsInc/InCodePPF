call clean
..\..\codegen ds.def ..\..\lib gen
cl /EHsc voidptr.cpp B.cpp c:\ppf\factory.cpp
voidptr 1 > res1
..\diff res1 out
voidptr 2 > res2
..\diff res2 out
