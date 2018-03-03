call clean
..\..\codegen ds.def ..\..\lib gen
cl /EHsc library.cpp c:\ppf\factory.cpp
library c > res1
..\diff res1 out1
library o > res2
..\diff res2 out2
