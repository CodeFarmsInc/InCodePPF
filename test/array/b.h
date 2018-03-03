#include "..\..\ppf\factory.h"

class B {
PersistClass(B);
public:
    ZZ_B ZZds; 
    int bval;
    B(){bval=0;}
    void prt(void){cout << "B=" << bval << "\n";}
};
