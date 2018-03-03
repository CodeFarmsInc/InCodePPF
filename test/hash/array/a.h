#include "c:\ppf\factory.h"

class A {
PersistClass(A);
public:
    ZZ_A ZZds; 
    int aval;
    A(){aval=0;}
    void prt(void){cout << "A=" << aval << "\n";}
};
