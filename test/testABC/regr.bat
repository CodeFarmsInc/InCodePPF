call clean
..\..\codegen -ppf ..\..\ppf ds.def ..\..\lib gen
cl /EHsc testABC.cpp B.cpp ..\..\ppf\factory.cpp
testABC 1 > res1
..\diff res1 out1
testABC 2 > res2
..\diff res2 out2
testABC 3 > res3
..\diff res3 out1
