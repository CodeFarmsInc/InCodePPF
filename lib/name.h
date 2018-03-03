#ifndef ZZ_$$_NAME_INCLUDED
#define ZZ_$$_NAME_INCLUDED
#include PPF_PATH

// -------- data structure SINGLY LINKED LIST ---------------
// EQUIVALENT OF:
// template <class Parent> class $$_Name {
// ----------------------------------------------------------

class $1;

// description of the cooperating classes
class $$_NameParent {
public:
    PersistString name;
    $$_NameParent(){ name=NULL; }
};

// ----------------------------------------------------------

class $$_Name {

public:
    static void add(PTR($1) p, STR c);     // add link to
    static void addNew(PTR($1) p, char *c);  // allocate string, add link
    static STR  get(PTR($1) p);
    static void  remove(PTR($1) p);
    static int compare(PTR($1) p1,PTR($1) p2);
    // ...
    // historical DOL interface
    static STR  del(PTR($1) p){STR  s=get(p); remove(p); return s;}
    static STR  fwd(PTR($1) p){return get(p);}
};
    
#endif // ZZ_$$_NAME_INCLUDED
