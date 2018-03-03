#include "environ.h"
#include "c:\ppf\factory.h"

class B {
PersistClass(B);
    int bb;
    PTR(PersistVoid) arr;
    int arrUsed;
public:
    ZZ_B ZZds;
    B(){bb=0; arr=NULL;}
    B(int i){bb=i; arr=NULL;}
    void prtList(PTR(B) bp); // change from the one-file version
    void setArray(PTR(B) bp);
    void prtArray();
};
