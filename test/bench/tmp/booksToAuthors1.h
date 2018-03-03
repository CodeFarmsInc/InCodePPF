
// -------- data structure DOUBLY LINKED LIST ---------------
// EQUIVALENT OF:
// template <class Parent,class Child> class booksToAuthors_LinkedList
// ----------------------------------------------------------
#ifndef ZZ_booksToAuthors_LINKED_LIST2_INCLUDED
#define ZZ_booksToAuthors_LINKED_LIST2_INCLUDED

class Book;
class BookToAuthor;

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class booksToAuthors_LinkedList2Parent {
public:
    PTR(BookToAuthor) tail;
    booksToAuthors_LinkedList2Parent(){ tail=NULL; }
};

class booksToAuthors_LinkedList2Child : public booksToAuthors_Ring2Element {
public:
    booksToAuthors_LinkedList2Child() : booksToAuthors_Ring2Element(){ }
};

// the following class is used when Parent==Child
class booksToAuthors_LinkedList2ParentLinkedList2Child : public booksToAuthors_Ring2Element {
public:
    PTR(BookToAuthor) tail;
    booksToAuthors_LinkedList2ParentLinkedList2Child() : booksToAuthors_Ring2Element(){ tail=NULL; }
};
// ----------------------------------------------------------

class booksToAuthors_LinkedList2 : booksToAuthors_Ring2 {

public:
    static PTR(BookToAuthor) const tail(PTR(Book) p);
    static PTR(BookToAuthor) const head(PTR(Book) p);
    static void addHead(PTR(Book) p, PTR(BookToAuthor) c);
    static void addTail(PTR(Book) p, PTR(BookToAuthor) c);
    static void append(PTR(Book) p,PTR(BookToAuthor) c1, PTR(BookToAuthor) c2);
    static void insert(PTR(BookToAuthor) c1, PTR(BookToAuthor) c2);
    static void remove(PTR(Book) p, PTR(BookToAuthor) c);
    static PTR(BookToAuthor) const next(PTR(Book) p, PTR(BookToAuthor) c);
    static PTR(BookToAuthor) const prev(PTR(Book) p, PTR(BookToAuthor) c);
    static PTR(BookToAuthor) const nextRing(PTR(BookToAuthor) c);
    static PTR(BookToAuthor) const prevRing(PTR(BookToAuthor) c);
    static void sort(booksToAuthors_sortFun cmpFun, PTR(Book) p);
    static void merge(PTR(BookToAuthor) s,PTR(BookToAuthor) t,PTR(Book) p);
    static void setTail(PTR(Book) p,PTR(BookToAuthor) c,int check);

    // historical DOL compatible interface
    static void del(PTR(Book) p, PTR(BookToAuthor) c) { remove(p,c); }
    static void add(PTR(Book) p, PTR(BookToAuthor) c) { addHead(p,c);}
    static void ins(PTR(BookToAuthor) c1, PTR(BookToAuthor) c2) { insert(c1,c2); }
    static PTR(BookToAuthor) child(PTR(Book) p);
    static void set(PTR(Book) p,PTR(BookToAuthor) c){ setTail(p,c,0);}
    static PTR(BookToAuthor) const fwd(PTR(BookToAuthor) c){return nextRing(c);}
    static PTR(BookToAuthor) const bwd(PTR(BookToAuthor) c){return prevRing(c);}
};

class booksToAuthors_LinkedList2Iterator : public booksToAuthors_Ring2Iterator {
public:
    booksToAuthors_LinkedList2Iterator() : booksToAuthors_Ring2Iterator(){}
    booksToAuthors_LinkedList2Iterator(const PTR(Book) p) : booksToAuthors_Ring2Iterator(){ start(p); }
    void start(const PTR(Book) p);
    PTR(BookToAuthor) fromHead(PTR(Book) p);
    PTR(BookToAuthor) fromTail(PTR(Book) p);
};
    
#endif // ZZ_booksToAuthors_LINKED_LIST2_INCLUDED
