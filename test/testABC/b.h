
#include "environ.h"
#include "c:\ppf\factory.h"

class B {
PersistClass(B);
    int bb;
public:
    ZZ_B ZZds;
    B(){bb=0;}
    B(int i){bb=i;}
    void prt(void); // change from the one-file version
};
