#include "environ.h"
#include "..\..\ppf\factory.h"

class Company {
    PersistClass(Company);
public:
    ZZ_Company ZZds;
    Company(){}
    Company(int bufSz);
    int checkAll(char *label,PTR(Company) cp);
    void static debPrtName(PTR(Company) cp);
    void static debPrtID(PTR(Company) cp);
    void static debPrtIteratorName(PTR(Company) cp);
    void static debPrtIteratorID(PTR(Company) cp);
    void static prtList(PTR(Company) cp);
private:
    PTR(Employee) eObj; // object to be used in queries even in subsequent runs
    int txtBufSz; // size of the text buffer used eName queries
};
