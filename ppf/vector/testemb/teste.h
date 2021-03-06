#include "stdlib.h"
#include "string.h"
#include "..\..\factory.h"
#include "..\vector.h"

class E {
PersistClass(E);
    vector<PersistPtr<A>> vec;
public:
    E():vec(){}
    void set(int i,PersistPtr<A> *a){vec.set(i,a);}
    int append(PersistPtr<A> a,int i){return vec.append(a,i);}
    PersistPtr<A>& get(int i){return vec.get(i);}
    int size(){return vec.size();}
    void prtVect(char *label);
    
};

#include "..\..\pointer.cpp" // always use when PersistPtr or PersistVptr is used
