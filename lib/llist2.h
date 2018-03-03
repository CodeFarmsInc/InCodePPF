
// -------- data structure DOUBLY LINKED LIST ---------------
// EQUIVALENT OF:
// template <class Parent,class Child> class $$_LinkedList
// ----------------------------------------------------------
#ifndef ZZ_$$_LINKED_LIST2_INCLUDED
#define ZZ_$$_LINKED_LIST2_INCLUDED

class $1;
class $2;

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class $$_LinkedList2Parent {
public:
    PTR($2) tail;
    $$_LinkedList2Parent(){ tail=NULL; }
};

class $$_LinkedList2Child : public $$_Ring2Element {
public:
    $$_LinkedList2Child() : $$_Ring2Element(){ }
};

// the following class is used when Parent==Child
class $$_LinkedList2ParentLinkedList2Child : public $$_Ring2Element {
public:
    PTR($2) tail;
    $$_LinkedList2ParentLinkedList2Child() : $$_Ring2Element(){ tail=NULL; }
};
// ----------------------------------------------------------

class $$_LinkedList2 : $$_Ring2 {

public:
    static PTR($2) const tail(PTR($1) p);
    static PTR($2) const head(PTR($1) p);
    static void addHead(PTR($1) p, PTR($2) c);
    static void addTail(PTR($1) p, PTR($2) c);
    static void append(PTR($1) p,PTR($2) c1, PTR($2) c2);
    static void insert(PTR($2) c1, PTR($2) c2);
    static void remove(PTR($1) p, PTR($2) c);
    static PTR($2) const next(PTR($1) p, PTR($2) c);
    static PTR($2) const prev(PTR($1) p, PTR($2) c);
    static PTR($2) const nextRing(PTR($2) c);
    static PTR($2) const prevRing(PTR($2) c);
    static void sort($$_sortFun cmpFun, PTR($1) p);
    static void merge(PTR($2) s,PTR($2) t,PTR($1) p);
    static void setTail(PTR($1) p,PTR($2) c,int check);

    // historical DOL compatible interface
    static void del(PTR($1) p, PTR($2) c) { remove(p,c); }
    static void add(PTR($1) p, PTR($2) c) { addHead(p,c);}
    static void ins(PTR($2) c1, PTR($2) c2) { insert(c1,c2); }
    static PTR($2) child(PTR($1) p);
    static void set(PTR($1) p,PTR($2) c){ setTail(p,c,0);}
    static PTR($2) const fwd(PTR($2) c){return nextRing(c);}
    static PTR($2) const bwd(PTR($2) c){return prevRing(c);}
};

class $$_LinkedList2Iterator : public $$_Ring2Iterator {
public:
    $$_LinkedList2Iterator() : $$_Ring2Iterator(){}
    $$_LinkedList2Iterator(const PTR($1) p) : $$_Ring2Iterator(){ start(p); }
    void start(const PTR($1) p);
    PTR($2) fromHead(PTR($1) p);
    PTR($2) fromTail(PTR($1) p);
};
    
#endif // ZZ_$$_LINKED_LIST2_INCLUDED
