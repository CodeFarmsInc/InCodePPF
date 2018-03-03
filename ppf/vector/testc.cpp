#include "stdlib.h"
#include "string.h"
#include "..\factory.h"
#include "vector.h"
#include "testD.h"
#include "testC.h"

void C::prtVect(char *label){
    int i,n; 
    cout << label << "\n";
    n=vec.size();
    for(i=0; i<n; i++)get(i).prt(i);
    cout.flush();
};

PersistImplement(C); // use just once, typically in B.cpp
PersistImplement(D); // there is no testd.cpp, so it is used here
