call clean
..\..\codegen ds.def ..\..\lib gen
cl /EHsc voidptr.cpp B.cpp c:\ppf\factory.cpp
