======================================================================
WARNING: 
Before running the two tests rr1.bat and rr2.bat, remove manually any
previously stored data., otherwise the test may crash.
We have to investigate whether this is an error of the library, or
of how the test (library.cpp) is coded.
                                      Jiri Soukup, Feb.5, 2014
======================================================================

libraryLast.cpp is the last version I had in this directory
       before re-visiting the problem
libraryBK.cpp is the version with which Petr ran the benchmark for the book
       (over 300 sec spent in DELETE operation).

Copy any verion into library.cpp and compile with tt.bat
rr1.bat creates the data and saves it (phase 1).
rr2.bat opens the data, tests it, and saves it again (phase2)

WARNING: number of books in tt1.bat and rr2.bat must agree.

Jiri, March 1, 2013
