#include "environ.h"
#include "c:\ppf\factory.h"

class A {
PersistClass(A);
    int aa;
public:
    ZZ_A ZZds; 
    A(){aa=0;}
    A(int i){aa=i;}
    void prt(void){cout << "A=" << aa << "\n";}
};
