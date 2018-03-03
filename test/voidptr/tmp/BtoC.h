

// -------- data structure SINGLY LINKED RING ---------------
// EQUIVALENT OF:
// template <class Node> class BtoC_Ring1
// ----------------------------------------------------------
#ifndef ZZ_BtoC_RING1_INCLUDED
#define ZZ_BtoC_RING1_INCLUDED

#include "environ.h"
#include "c:\ppf\factory.h"

class C;
typedef int (*BtoC_sortFun)(const PTR(C), const PTR(C));

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class BtoC_Ring1Element {
public:
    PTR(C) next;
    BtoC_Ring1Element(){ next=NULL; }
};

class BtoC_Ring1 {

public:
    // standard interface
    static PTR(C) addHead(PTR(C) tail, PTR(C) c); // returns new tail
    static PTR(C) addTail(PTR(C) tail, PTR(C) c); // returns new tail
    static PTR(C) append(PTR(C) tail,PTR(C) c1, PTR(C) c2); // returns new tail
    static PTR(C) remove(PTR(C) tail, PTR(C) c);  // returns new tail, NULL when empty
    static PTR(C) const next(PTR(C) p, PTR(C) c); // returns NULL when p is tail
    static PTR(C) const nextRing(PTR(C) c); // unprotected raw ring pointer
    static PTR(C) sort(BtoC_sortFun cmpFun, PTR(C) tail); // returns the new tail
    static void merge(PTR(C) s,PTR(C) t);

    // historical DOL compatible interface
    static PTR(C) del(PTR(C) tail, PTR(C) c) { return remove(tail,c); }
    static PTR(C) add(PTR(C) tail, PTR(C) c) { return addHead(tail,c); }
    static PTR(C) const fwd(PTR(C) c){ return nextRing(c);}
};

class BtoC_Ring1Iterator {
    PTR(C) tail; // NULL when loop finished
    PTR(C) nxt;  // NULL when starting a new loop
public:
    // standard interface:          for(p=it.fromHead(x); p; p=it.next()){...}
    BtoC_Ring1Iterator(){tail=nxt=NULL;}
    PTR(C) fromHead(PTR(C) p);
    PTR(C) const next();

    // historical DOL interface:    it.start(x); for(p= ++it; p; p= ++it){...}
    void start(PTR(C) p);
    PTR(C) const operator++();
    BtoC_Ring1Iterator(C *p){start(p);}
};
    
#endif // ZZ_BtoC_RING1_INCLUDED
