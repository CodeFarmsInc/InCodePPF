The library assumes that the PPF library (files factory.h, factory.cpp, and 
pointer.cpp) is in subdirectory ppf.

Subdirectory alib is not the full code of the C++ InCode library.
It is only its part which allows to recompile codegen.exe if you
need it on a different platform than Windows 32.
alib is not persistent by itself.

Code generator is the same for plain C++, PPF or Objective-C version
of InCode. Recompiling codegen is a bit tricky, because it needs itself 
and the InCode library to recompile.

Check file setup.bat copies alib\codegen.exe into this directory.

The description of the data organization is always assumed to be in file ds.def
(data structure definitions). As with all Code Farms libraries, this file can
be used as input for the Layout program, which generates the UML class diagram
of your data organization.

Program test\diff.cpp must be compiled in order to run the tests which
are under directory 'test'. This is also done in setup.bat

Note that each directory in which you run the test needs a little file environ.h
which specifies whether the light, 4-byte persistent pointers can be used 
(PPF_LIGHT defined) or whether full, 8-byte persistent pointers are needed
(PPF_LIGHT not defined). The light version can be used if your data organization
does not include inheritance.
