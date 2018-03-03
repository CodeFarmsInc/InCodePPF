call clean
..\..\codegen -ppf ..\..\ppf ds.def ..\..\lib gen
cl /EHsc hash1.cpp c:\ppf\factory.cpp company.cpp employee.cpp
