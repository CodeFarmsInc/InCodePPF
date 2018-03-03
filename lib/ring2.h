
// -------- data structure DOUBLY LINKED RING ---------------
// EQUIVALENT OF:
// template <class Node> class $$_Ring2
// ----------------------------------------------------------
// Ring2 could be derived from Ring2 through inheritance, but
//  (a) The complex functions (sort, remove, merge) are different anyway,
//  (b) we want to minimize the number of classes which will be code generated.
// For these two reasons, Ring2 is coded as a base class.
// ----------------------------------------------------------
#ifndef ZZ_$$_RING2_INCLUDED
#define ZZ_$$_RING2_INCLUDED

#include "environ.h"
#include PPF_PATH

class $1;
typedef int (*$$_sortFun)(const PTR($1), const PTR($1));

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class $$_Ring2Element {
public:
    PTR($1) next;
    PTR($1) prev;
    $$_Ring2Element(){ next=prev=NULL; }
};

class $$_Ring2 {

    static int debugFun(PTR($1) tail);
public:
    // standard interface
    static PTR($1) addHead(PTR($1) tail, PTR($1) c); // returns new tail
    static PTR($1) addTail(PTR($1) tail, PTR($1) c); // returns new tail
    static PTR($1) append(PTR($1) tail,PTR($1) c1, PTR($1) c2); // returns new tail
    static void insert(PTR($1) c1, PTR($1) c2); // insert c1 before c2
    static PTR($1) remove(PTR($1) tail, PTR($1) c);  // returns new tail, NULL when empty
    static PTR($1) const next(PTR($1) p, PTR($1) c); // returns NULL when p is the tail
    static PTR($1) const prev(PTR($1) p, PTR($1) c); // returns NULL when p is the head
    static PTR($1) const nextRing(PTR($1) c); // next in the ring
    static PTR($1) const prevRing(PTR($1) c); // previous in the ring
    static PTR($1) sort($$_sortFun cmpFun, PTR($1) tail); // returns the new tail
    static void merge(PTR($1) s,PTR($1) t);

    // historical DOL compatible interface
    static PTR($1) del(PTR($1) tail, PTR($1) c) { return remove(tail,c); }
    static PTR($1) add(PTR($1) tail, PTR($1) c) { return addHead(tail,c); }
    static void ins(PTR($1) c1, PTR($1) c2) { insert(c1,c2); }
    static PTR($1) const fwd(PTR($1) c){return nextRing(c);}
    static PTR($1) const bwd(PTR($1) c){return prevRing(c);}
    
};

class $$_Ring2Iterator {
    PTR($1) tail; // NULL when loop finished
    PTR($1) nxt;  // NULL when starting a new loop
    int dir;  // 0=from head, 1=from tail
public:
    // standard interface:          for(p=it.fromHead(x); p; p=it.next()){...}
    //                              for(p=it.fromTail(x); p; p=it.next()){...}
    $$_Ring2Iterator(){tail=nxt=NULL; dir=0;}
    PTR($1) fromHead(PTR($1) p);
    virtual PTR($1) fromTail(PTR($1) p);
    virtual PTR($1) const next();

    // historical DOL interface:    it.start(x); for(p= ++it; p; p= ++it){...}
    //                              it.start(x); for(p= --it; p; p= --it){...}
    void start(PTR($1) p);
    PTR($1) const operator++();
    PTR($1) const operator--();
    $$_Ring2Iterator(PTR($1) p){start(p);}
};
    
#endif // ZZ_$$_RING2_INCLUDED
