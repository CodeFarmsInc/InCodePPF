
// -------- data structure DOUBLY LINKED RING ---------------
// EQUIVALENT OF:
// template <class Node> class booksToAuthors_Ring2
// ----------------------------------------------------------
// Ring2 could be derived from Ring2 through inheritance, but
//  (a) The complex functions (sort, remove, merge) are different anyway,
//  (b) we want to minimize the number of classes which will be code generated.
// For these two reasons, Ring2 is coded as a base class.
// ----------------------------------------------------------
#ifndef ZZ_booksToAuthors_RING2_INCLUDED
#define ZZ_booksToAuthors_RING2_INCLUDED

#include "environ.h"
#include PPF_PATH

class BookToAuthor;
typedef int (*booksToAuthors_sortFun)(const PTR(BookToAuthor), const PTR(BookToAuthor));

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class booksToAuthors_Ring2Element {
public:
    PTR(BookToAuthor) next;
    PTR(BookToAuthor) prev;
    booksToAuthors_Ring2Element(){ next=prev=NULL; }
};

class booksToAuthors_Ring2 {

    static int debugFun(PTR(BookToAuthor) tail);
public:
    // standard interface
    static PTR(BookToAuthor) addHead(PTR(BookToAuthor) tail, PTR(BookToAuthor) c); // returns new tail
    static PTR(BookToAuthor) addTail(PTR(BookToAuthor) tail, PTR(BookToAuthor) c); // returns new tail
    static PTR(BookToAuthor) append(PTR(BookToAuthor) tail,PTR(BookToAuthor) c1, PTR(BookToAuthor) c2); // returns new tail
    static void insert(PTR(BookToAuthor) c1, PTR(BookToAuthor) c2); // insert c1 before c2
    static PTR(BookToAuthor) remove(PTR(BookToAuthor) tail, PTR(BookToAuthor) c);  // returns new tail, NULL when empty
    static PTR(BookToAuthor) const next(PTR(BookToAuthor) p, PTR(BookToAuthor) c); // returns NULL when p is the tail
    static PTR(BookToAuthor) const prev(PTR(BookToAuthor) p, PTR(BookToAuthor) c); // returns NULL when p is the head
    static PTR(BookToAuthor) const nextRing(PTR(BookToAuthor) c); // next in the ring
    static PTR(BookToAuthor) const prevRing(PTR(BookToAuthor) c); // previous in the ring
    static PTR(BookToAuthor) sort(booksToAuthors_sortFun cmpFun, PTR(BookToAuthor) tail); // returns the new tail
    static void merge(PTR(BookToAuthor) s,PTR(BookToAuthor) t);

    // historical DOL compatible interface
    static PTR(BookToAuthor) del(PTR(BookToAuthor) tail, PTR(BookToAuthor) c) { return remove(tail,c); }
    static PTR(BookToAuthor) add(PTR(BookToAuthor) tail, PTR(BookToAuthor) c) { return addHead(tail,c); }
    static void ins(PTR(BookToAuthor) c1, PTR(BookToAuthor) c2) { insert(c1,c2); }
    static PTR(BookToAuthor) const fwd(PTR(BookToAuthor) c){return nextRing(c);}
    static PTR(BookToAuthor) const bwd(PTR(BookToAuthor) c){return prevRing(c);}
    
};

class booksToAuthors_Ring2Iterator {
    PTR(BookToAuthor) tail; // NULL when loop finished
    PTR(BookToAuthor) nxt;  // NULL when starting a new loop
    int dir;  // 0=from head, 1=from tail
public:
    // standard interface:          for(p=it.fromHead(x); p; p=it.next()){...}
    //                              for(p=it.fromTail(x); p; p=it.next()){...}
    booksToAuthors_Ring2Iterator(){tail=nxt=NULL; dir=0;}
    PTR(BookToAuthor) fromHead(PTR(BookToAuthor) p);
    virtual PTR(BookToAuthor) fromTail(PTR(BookToAuthor) p);
    virtual PTR(BookToAuthor) const next();

    // historical DOL interface:    it.start(x); for(p= ++it; p; p= ++it){...}
    //                              it.start(x); for(p= --it; p; p= --it){...}
    void start(PTR(BookToAuthor) p);
    PTR(BookToAuthor) const operator++();
    PTR(BookToAuthor) const operator--();
    booksToAuthors_Ring2Iterator(PTR(BookToAuthor) p){start(p);}
};
    
#endif // ZZ_booksToAuthors_RING2_INCLUDED
