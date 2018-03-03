
// -------- data structure SINGLY LINKED LIST ---------------
// EQUIVALENT OF:
// template <class Parent,class Child> class BtoC_LinkedList
// ----------------------------------------------------------
#ifndef ZZ_BtoC_LINKED_LIST1_INCLUDED
#define ZZ_BtoC_LINKED_LIST1_INCLUDED

class B;
class C;

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class BtoC_LinkedList1Parent {
public:
    PTR(C) tail;
    BtoC_LinkedList1Parent(){ tail=NULL; }
};

class BtoC_LinkedList1Child : public BtoC_Ring1Element {
public:
    BtoC_LinkedList1Child() : BtoC_Ring1Element(){ }
};

// the following class is used when Parent==Child
class BtoC_LinkedList1ParentLinkedList1Child : public BtoC_Ring1Element {
public:
    PTR(C) tail;
    BtoC_LinkedList1ParentLinkedList1Child() : BtoC_Ring1Element(){ tail=NULL; }
};
// ----------------------------------------------------------

class BtoC_LinkedList1 : BtoC_Ring1 {

public:
    static PTR(C) tail(PTR(B) p);
    static PTR(C) head(PTR(B) p);
    static void addHead(PTR(B) p, PTR(C) c);
    static void addTail(PTR(B) p, PTR(C) c);
    static void append(PTR(B) p,PTR(C) c1, PTR(C) c2);
    static void remove(PTR(B) p, PTR(C) c);
    static PTR(C) const next(PTR(B) p,PTR(C) c);
    static PTR(C) const nextRing(PTR(C) c);
    static void sort(BtoC_sortFun cmpFun, PTR(B) p);
    static void merge(PTR(C) s,PTR(C) t,PTR(B) p);
    static void setTail(PTR(B) p,PTR(C) c,int check);

    // historical DOL compatible interface
    static void del(PTR(B) p, PTR(C) c){ remove(p,c);}
    static void add(PTR(B) p, PTR(C) c){ addHead(p,c);}
    static PTR(C) child(PTR(B) p);
    static void set(PTR(B) p,PTR(C) c){ setTail(p,c,0);}
    static PTR(C) const fwd(PTR(C) c){ return nextRing(c);}
};

class BtoC_LinkedList1Iterator : public BtoC_Ring1Iterator {
public:
    BtoC_LinkedList1Iterator() : BtoC_Ring1Iterator(){}
    BtoC_LinkedList1Iterator(const PTR(B) p) : BtoC_Ring1Iterator(){start(p);}
    void start(const PTR(B) p);
    PTR(C) fromHead(PTR(B) p);
};
    
#endif // ZZ_BtoC_LINKED_LIST1_INCLUDED
