
// -------- data structure DOUBLY LINKED RING ---------------
// EQUIVALENT OF:
// template <class Node> class books_Ring2
// ----------------------------------------------------------
// Ring2 could be derived from Ring2 through inheritance, but
//  (a) The complex functions (sort, remove, merge) are different anyway,
//  (b) we want to minimize the number of classes which will be code generated.
// For these two reasons, Ring2 is coded as a base class.
// ----------------------------------------------------------
#ifndef ZZ_books_RING2_INCLUDED
#define ZZ_books_RING2_INCLUDED

#include "environ.h"
#include PPF_PATH

class Book;
typedef int (*books_sortFun)(const PTR(Book), const PTR(Book));

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class books_Ring2Element {
public:
    PTR(Book) next;
    PTR(Book) prev;
    books_Ring2Element(){ next=prev=NULL; }
};

class books_Ring2 {

    static int debugFun(PTR(Book) tail);
public:
    // standard interface
    static PTR(Book) addHead(PTR(Book) tail, PTR(Book) c); // returns new tail
    static PTR(Book) addTail(PTR(Book) tail, PTR(Book) c); // returns new tail
    static PTR(Book) append(PTR(Book) tail,PTR(Book) c1, PTR(Book) c2); // returns new tail
    static void insert(PTR(Book) c1, PTR(Book) c2); // insert c1 before c2
    static PTR(Book) remove(PTR(Book) tail, PTR(Book) c);  // returns new tail, NULL when empty
    static PTR(Book) const next(PTR(Book) p, PTR(Book) c); // returns NULL when p is the tail
    static PTR(Book) const prev(PTR(Book) p, PTR(Book) c); // returns NULL when p is the head
    static PTR(Book) const nextRing(PTR(Book) c); // next in the ring
    static PTR(Book) const prevRing(PTR(Book) c); // previous in the ring
    static PTR(Book) sort(books_sortFun cmpFun, PTR(Book) tail); // returns the new tail
    static void merge(PTR(Book) s,PTR(Book) t);

    // historical DOL compatible interface
    static PTR(Book) del(PTR(Book) tail, PTR(Book) c) { return remove(tail,c); }
    static PTR(Book) add(PTR(Book) tail, PTR(Book) c) { return addHead(tail,c); }
    static void ins(PTR(Book) c1, PTR(Book) c2) { insert(c1,c2); }
    static PTR(Book) const fwd(PTR(Book) c){return nextRing(c);}
    static PTR(Book) const bwd(PTR(Book) c){return prevRing(c);}
    
};

class books_Ring2Iterator {
    PTR(Book) tail; // NULL when loop finished
    PTR(Book) nxt;  // NULL when starting a new loop
    int dir;  // 0=from head, 1=from tail
public:
    // standard interface:          for(p=it.fromHead(x); p; p=it.next()){...}
    //                              for(p=it.fromTail(x); p; p=it.next()){...}
    books_Ring2Iterator(){tail=nxt=NULL; dir=0;}
    PTR(Book) fromHead(PTR(Book) p);
    virtual PTR(Book) fromTail(PTR(Book) p);
    virtual PTR(Book) const next();

    // historical DOL interface:    it.start(x); for(p= ++it; p; p= ++it){...}
    //                              it.start(x); for(p= --it; p; p= --it){...}
    void start(PTR(Book) p);
    PTR(Book) const operator++();
    PTR(Book) const operator--();
    books_Ring2Iterator(PTR(Book) p){start(p);}
};
    
#endif // ZZ_books_RING2_INCLUDED
