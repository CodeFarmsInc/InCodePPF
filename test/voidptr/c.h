#include "c:\ppf\factory.h"

class C {
PersistClass(C);
    int cc;
public:
    ZZ_C ZZds;
    C(){cc=0;}
    C(int i){cc=i;}
    void prt(void){cout << "    C=" << cc;}
    int val(){return cc;}
};
