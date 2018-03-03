
// -------- data structure DOUBLY LINKED LIST ---------------
// EQUIVALENT OF:
// template <class Parent,class Child> class books_LinkedList
// ----------------------------------------------------------
#ifndef ZZ_books_LINKED_LIST2_INCLUDED
#define ZZ_books_LINKED_LIST2_INCLUDED

class Library;
class Book;

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class books_LinkedList2Parent {
public:
    PTR(Book) tail;
    books_LinkedList2Parent(){ tail=NULL; }
};

class books_LinkedList2Child : public books_Ring2Element {
public:
    books_LinkedList2Child() : books_Ring2Element(){ }
};

// the following class is used when Parent==Child
class books_LinkedList2ParentLinkedList2Child : public books_Ring2Element {
public:
    PTR(Book) tail;
    books_LinkedList2ParentLinkedList2Child() : books_Ring2Element(){ tail=NULL; }
};
// ----------------------------------------------------------

class books_LinkedList2 : books_Ring2 {

public:
    static PTR(Book) const tail(PTR(Library) p);
    static PTR(Book) const head(PTR(Library) p);
    static void addHead(PTR(Library) p, PTR(Book) c);
    static void addTail(PTR(Library) p, PTR(Book) c);
    static void append(PTR(Library) p,PTR(Book) c1, PTR(Book) c2);
    static void insert(PTR(Book) c1, PTR(Book) c2);
    static void remove(PTR(Library) p, PTR(Book) c);
    static PTR(Book) const next(PTR(Library) p, PTR(Book) c);
    static PTR(Book) const prev(PTR(Library) p, PTR(Book) c);
    static PTR(Book) const nextRing(PTR(Book) c);
    static PTR(Book) const prevRing(PTR(Book) c);
    static void sort(books_sortFun cmpFun, PTR(Library) p);
    static void merge(PTR(Book) s,PTR(Book) t,PTR(Library) p);
    static void setTail(PTR(Library) p,PTR(Book) c,int check);

    // historical DOL compatible interface
    static void del(PTR(Library) p, PTR(Book) c) { remove(p,c); }
    static void add(PTR(Library) p, PTR(Book) c) { addHead(p,c);}
    static void ins(PTR(Book) c1, PTR(Book) c2) { insert(c1,c2); }
    static PTR(Book) child(PTR(Library) p);
    static void set(PTR(Library) p,PTR(Book) c){ setTail(p,c,0);}
    static PTR(Book) const fwd(PTR(Book) c){return nextRing(c);}
    static PTR(Book) const bwd(PTR(Book) c){return prevRing(c);}
};

class books_LinkedList2Iterator : public books_Ring2Iterator {
public:
    books_LinkedList2Iterator() : books_Ring2Iterator(){}
    books_LinkedList2Iterator(const PTR(Library) p) : books_Ring2Iterator(){ start(p); }
    void start(const PTR(Library) p);
    PTR(Book) fromHead(PTR(Library) p);
    PTR(Book) fromTail(PTR(Library) p);
};
    
#endif // ZZ_books_LINKED_LIST2_INCLUDED
