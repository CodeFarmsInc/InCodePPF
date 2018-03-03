
// -------- data structure DOUBLY LINKED LIST ---------------
// EQUIVALENT OF:
// template <class Parent,class Child> class authors_LinkedList
// ----------------------------------------------------------
#ifndef ZZ_authors_LINKED_LIST2_INCLUDED
#define ZZ_authors_LINKED_LIST2_INCLUDED

class Library;
class Author;

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class authors_LinkedList2Parent {
public:
    PTR(Author) tail;
    authors_LinkedList2Parent(){ tail=NULL; }
};

class authors_LinkedList2Child : public authors_Ring2Element {
public:
    authors_LinkedList2Child() : authors_Ring2Element(){ }
};

// the following class is used when Parent==Child
class authors_LinkedList2ParentLinkedList2Child : public authors_Ring2Element {
public:
    PTR(Author) tail;
    authors_LinkedList2ParentLinkedList2Child() : authors_Ring2Element(){ tail=NULL; }
};
// ----------------------------------------------------------

class authors_LinkedList2 : authors_Ring2 {

public:
    static PTR(Author) const tail(PTR(Library) p);
    static PTR(Author) const head(PTR(Library) p);
    static void addHead(PTR(Library) p, PTR(Author) c);
    static void addTail(PTR(Library) p, PTR(Author) c);
    static void append(PTR(Library) p,PTR(Author) c1, PTR(Author) c2);
    static void insert(PTR(Author) c1, PTR(Author) c2);
    static void remove(PTR(Library) p, PTR(Author) c);
    static PTR(Author) const next(PTR(Library) p, PTR(Author) c);
    static PTR(Author) const prev(PTR(Library) p, PTR(Author) c);
    static PTR(Author) const nextRing(PTR(Author) c);
    static PTR(Author) const prevRing(PTR(Author) c);
    static void sort(authors_sortFun cmpFun, PTR(Library) p);
    static void merge(PTR(Author) s,PTR(Author) t,PTR(Library) p);
    static void setTail(PTR(Library) p,PTR(Author) c,int check);

    // historical DOL compatible interface
    static void del(PTR(Library) p, PTR(Author) c) { remove(p,c); }
    static void add(PTR(Library) p, PTR(Author) c) { addHead(p,c);}
    static void ins(PTR(Author) c1, PTR(Author) c2) { insert(c1,c2); }
    static PTR(Author) child(PTR(Library) p);
    static void set(PTR(Library) p,PTR(Author) c){ setTail(p,c,0);}
    static PTR(Author) const fwd(PTR(Author) c){return nextRing(c);}
    static PTR(Author) const bwd(PTR(Author) c){return prevRing(c);}
};

class authors_LinkedList2Iterator : public authors_Ring2Iterator {
public:
    authors_LinkedList2Iterator() : authors_Ring2Iterator(){}
    authors_LinkedList2Iterator(const PTR(Library) p) : authors_Ring2Iterator(){ start(p); }
    void start(const PTR(Library) p);
    PTR(Author) fromHead(PTR(Library) p);
    PTR(Author) fromTail(PTR(Library) p);
};
    
#endif // ZZ_authors_LINKED_LIST2_INCLUDED
