
// -------- data structure DOUBLY LINKED RING ---------------
// EQUIVALENT OF:
// template <class Node> class authors_Ring2
// ----------------------------------------------------------
// Ring2 could be derived from Ring2 through inheritance, but
//  (a) The complex functions (sort, remove, merge) are different anyway,
//  (b) we want to minimize the number of classes which will be code generated.
// For these two reasons, Ring2 is coded as a base class.
// ----------------------------------------------------------
#ifndef ZZ_authors_RING2_INCLUDED
#define ZZ_authors_RING2_INCLUDED

#include "environ.h"
#include PPF_PATH

class Author;
typedef int (*authors_sortFun)(const PTR(Author), const PTR(Author));

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class authors_Ring2Element {
public:
    PTR(Author) next;
    PTR(Author) prev;
    authors_Ring2Element(){ next=prev=NULL; }
};

class authors_Ring2 {

    static int debugFun(PTR(Author) tail);
public:
    // standard interface
    static PTR(Author) addHead(PTR(Author) tail, PTR(Author) c); // returns new tail
    static PTR(Author) addTail(PTR(Author) tail, PTR(Author) c); // returns new tail
    static PTR(Author) append(PTR(Author) tail,PTR(Author) c1, PTR(Author) c2); // returns new tail
    static void insert(PTR(Author) c1, PTR(Author) c2); // insert c1 before c2
    static PTR(Author) remove(PTR(Author) tail, PTR(Author) c);  // returns new tail, NULL when empty
    static PTR(Author) const next(PTR(Author) p, PTR(Author) c); // returns NULL when p is the tail
    static PTR(Author) const prev(PTR(Author) p, PTR(Author) c); // returns NULL when p is the head
    static PTR(Author) const nextRing(PTR(Author) c); // next in the ring
    static PTR(Author) const prevRing(PTR(Author) c); // previous in the ring
    static PTR(Author) sort(authors_sortFun cmpFun, PTR(Author) tail); // returns the new tail
    static void merge(PTR(Author) s,PTR(Author) t);

    // historical DOL compatible interface
    static PTR(Author) del(PTR(Author) tail, PTR(Author) c) { return remove(tail,c); }
    static PTR(Author) add(PTR(Author) tail, PTR(Author) c) { return addHead(tail,c); }
    static void ins(PTR(Author) c1, PTR(Author) c2) { insert(c1,c2); }
    static PTR(Author) const fwd(PTR(Author) c){return nextRing(c);}
    static PTR(Author) const bwd(PTR(Author) c){return prevRing(c);}
    
};

class authors_Ring2Iterator {
    PTR(Author) tail; // NULL when loop finished
    PTR(Author) nxt;  // NULL when starting a new loop
    int dir;  // 0=from head, 1=from tail
public:
    // standard interface:          for(p=it.fromHead(x); p; p=it.next()){...}
    //                              for(p=it.fromTail(x); p; p=it.next()){...}
    authors_Ring2Iterator(){tail=nxt=NULL; dir=0;}
    PTR(Author) fromHead(PTR(Author) p);
    virtual PTR(Author) fromTail(PTR(Author) p);
    virtual PTR(Author) const next();

    // historical DOL interface:    it.start(x); for(p= ++it; p; p= ++it){...}
    //                              it.start(x); for(p= --it; p; p= --it){...}
    void start(PTR(Author) p);
    PTR(Author) const operator++();
    PTR(Author) const operator--();
    authors_Ring2Iterator(PTR(Author) p){start(p);}
};
    
#endif // ZZ_authors_RING2_INCLUDED
