// ----------------------------------------------------------
//                    1XtoX<Relation,Entity>
// ----------------------------------------------------------
// This data organization is not intended for direct use. It is the root
// organization of ManyToMany for 2,3 or more participating classes.
// Note that 1XtoX is derived from Aggregate2<>, but the 
// parameters are reversed.
//                                    Jiri Soukup, July 14, 2005
// ----------------------------------------------------------

// ----------------------------------------------------------
#ifndef ZZ_$$_1XTOX_INCLUDED
#define ZZ_$$_1XTOX_INCLUDED

class $1;
class $2;

// description of the cooperating classes
class $$_1XtoXRelation : public $$_Aggregate2Child {
public:
    $$_1XtoXRelation() : $$_Aggregate2Child(){ }
};

class $$_1XtoXEntity1 : public $$_Aggregate2Parent {
public:
    $$_1XtoXEntity1() : $$_Aggregate2Parent(){ }
};

// ----------------------------------------------------------

class $$_1XtoX : private $$_Aggregate2 {

public:
    static void add(PTR($1) r, PTR($2) e){ $$_Aggregate2::addTail(e,r);}
    static void remove(PTR($1) r){ $$_Aggregate2::remove(r); }
    static PTR($2) const entity1(PTR($1) r){ return $$_Aggregate2::parent(r); }
    static PTR($1) const next1(PTR($1) r){ // returns NULL when s is the tail
        return $$_Aggregate2::next(r);
    }
    static PTR($1) const prev1(PTR($1) r){ // returns NULL when s is the head
        return $$_Aggregate2::prev(r);
    }
};

class $$_1XtoXIterator : private $$_Aggregate2Iterator {
    // standard interface:          for(r=it.from1(e); r; r=it.next1()){...}
public:
    PTR($1) from1(PTR($2) e){return $$_Aggregate2Iterator::fromHead(e);}
    PTR($1) const next1(){return $$_Aggregate2Iterator::next();}
};

#endif // ZZ_$$_1XTOX_INCLUDED
