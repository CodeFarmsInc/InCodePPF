call clean
..\..\codegen ds.def ..\..\lib gen
cl /EHsc testABC.cpp B.cpp c:\ppf\factory.cpp
