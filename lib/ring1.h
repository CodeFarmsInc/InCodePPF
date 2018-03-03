

// -------- data structure SINGLY LINKED RING ---------------
// EQUIVALENT OF:
// template <class Node> class $$_Ring1
// ----------------------------------------------------------
#ifndef ZZ_$$_RING1_INCLUDED
#define ZZ_$$_RING1_INCLUDED

#include "environ.h"
#include PPF_PATH

class $1;
typedef int (*$$_sortFun)(const PTR($1), const PTR($1));

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class $$_Ring1Element {
public:
    PTR($1) next;
    $$_Ring1Element(){ next=NULL; }
};

class $$_Ring1 {

public:
    // standard interface
    static PTR($1) addHead(PTR($1) tail, PTR($1) c); // returns new tail
    static PTR($1) addTail(PTR($1) tail, PTR($1) c); // returns new tail
    static PTR($1) append(PTR($1) tail,PTR($1) c1, PTR($1) c2); // returns new tail
    static PTR($1) remove(PTR($1) tail, PTR($1) c);  // returns new tail, NULL when empty
    static PTR($1) const next(PTR($1) p, PTR($1) c); // returns NULL when p is tail
    static PTR($1) const nextRing(PTR($1) c); // unprotected raw ring pointer
    static PTR($1) sort($$_sortFun cmpFun, PTR($1) tail); // returns the new tail
    static void merge(PTR($1) s,PTR($1) t);

    // historical DOL compatible interface
    static PTR($1) del(PTR($1) tail, PTR($1) c) { return remove(tail,c); }
    static PTR($1) add(PTR($1) tail, PTR($1) c) { return addHead(tail,c); }
    static PTR($1) const fwd(PTR($1) c){ return nextRing(c);}
};

class $$_Ring1Iterator {
    PTR($1) tail; // NULL when loop finished
    PTR($1) nxt;  // NULL when starting a new loop
public:
    // standard interface:          for(p=it.fromHead(x); p; p=it.next()){...}
    $$_Ring1Iterator(){tail=nxt=NULL;}
    PTR($1) fromHead(PTR($1) p);
    PTR($1) const next();

    // historical DOL interface:    it.start(x); for(p= ++it; p; p= ++it){...}
    void start(PTR($1) p);
    PTR($1) const operator++();
    $$_Ring1Iterator($1 *p){start(p);}
};
    
#endif // ZZ_$$_RING1_INCLUDED
