
// ----------------- data structure Aggregate ---------------
// ----------------------------------------------------------
// EQUIVALENT OF:
//     template <class Parent, class Child> class $$_Aggregate :
//                                   public $$_LinkedList<Parent,Child>
// INHERITS FROM: LinkedList
// ----------------------------------------------------------
#ifndef ZZ_$$_AGGREGATE2_INCLUDED
#define ZZ_$$_AGGREGATE2_INCLUDED

class $1;
class $2;

// description of the cooperating classes
class $$_Aggregate2Parent : public $$_LinkedList2Parent {
public:
    $$_Aggregate2Parent() : $$_LinkedList2Parent(){ }
};

class $$_Aggregate2Child : public $$_LinkedList2Child {
public:
    PTR($1) parent;
    $$_Aggregate2Child() : $$_LinkedList2Child(){ parent=NULL; }
};

// the following class is used when Parent==Child
class $$_Aggregate2ParentAggregate2Child 
       : public $$_LinkedList2ParentLinkedList2Child {
public:
    PTR($1) parent;
    $$_Aggregate2ParentAggregate2Child()
       : $$_LinkedList2ParentLinkedList2Child(){ parent=NULL; }
};

// ----------------------------------------------------------

class $$_Aggregate2 : public $$_LinkedList2 {

public:
    static void addHead(PTR($1) p, PTR($2) c);
    static void addTail(PTR($1) p, PTR($2) c);
    static void append(PTR($2) c1, PTR($2) c2); // has a different syntax
    static void insert(PTR($2) c1, PTR($2) c2); 
    static void remove(PTR($2) c);             // has a different syntax
    static PTR($1) const parent(PTR($2) c);    // is new
    static PTR($2) const next(PTR($2) c){ // returns NULL when s is the tail
        return $$_LinkedList2::next(parent(c),c);
    }
    static PTR($2) const prev(PTR($2) c){ // returns NULL when s is the head
        return $$_LinkedList2::prev(parent(c),c);
    }
    // ...
};

class $$_Aggregate2Iterator : public $$_LinkedList2Iterator {
};

#endif // ZZ_$$_AGGREGATE2_INCLUDED
