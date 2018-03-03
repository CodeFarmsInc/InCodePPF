
// -------- data structure SINGLY LINKED LIST ---------------
// EQUIVALENT OF:
// template <class Parent,class Child> class $$_LinkedList
// ----------------------------------------------------------
#ifndef ZZ_$$_LINKED_LIST1_INCLUDED
#define ZZ_$$_LINKED_LIST1_INCLUDED

class $1;
class $2;

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class $$_LinkedList1Parent {
public:
    PTR($2) tail;
    $$_LinkedList1Parent(){ tail=NULL; }
};

class $$_LinkedList1Child : public $$_Ring1Element {
public:
    $$_LinkedList1Child() : $$_Ring1Element(){ }
};

// the following class is used when Parent==Child
class $$_LinkedList1ParentLinkedList1Child : public $$_Ring1Element {
public:
    PTR($2) tail;
    $$_LinkedList1ParentLinkedList1Child() : $$_Ring1Element(){ tail=NULL; }
};
// ----------------------------------------------------------

class $$_LinkedList1 : $$_Ring1 {

public:
    static PTR($2) tail(PTR($1) p);
    static PTR($2) head(PTR($1) p);
    static void addHead(PTR($1) p, PTR($2) c);
    static void addTail(PTR($1) p, PTR($2) c);
    static void append(PTR($1) p,PTR($2) c1, PTR($2) c2);
    static void remove(PTR($1) p, PTR($2) c);
    static PTR($2) const next(PTR($1) p,PTR($2) c);
    static PTR($2) const nextRing(PTR($2) c);
    static void sort($$_sortFun cmpFun, PTR($1) p);
    static void merge(PTR($2) s,PTR($2) t,PTR($1) p);
    static void setTail(PTR($1) p,PTR($2) c,int check);

    // historical DOL compatible interface
    static void del(PTR($1) p, PTR($2) c){ remove(p,c);}
    static void add(PTR($1) p, PTR($2) c){ addHead(p,c);}
    static PTR($2) child(PTR($1) p);
    static void set(PTR($1) p,PTR($2) c){ setTail(p,c,0);}
    static PTR($2) const fwd(PTR($2) c){ return nextRing(c);}
};

class $$_LinkedList1Iterator : public $$_Ring1Iterator {
public:
    $$_LinkedList1Iterator() : $$_Ring1Iterator(){}
    $$_LinkedList1Iterator(const PTR($1) p) : $$_Ring1Iterator(){start(p);}
    void start(const PTR($1) p);
    PTR($2) fromHead(PTR($1) p);
};
    
#endif // ZZ_$$_LINKED_LIST1_INCLUDED
