#ifndef ZZ_authorName_NAME_INCLUDED
#define ZZ_authorName_NAME_INCLUDED
#include PPF_PATH

// -------- data structure SINGLY LINKED LIST ---------------
// EQUIVALENT OF:
// template <class Parent> class authorName_Name {
// ----------------------------------------------------------

class Author;

// description of the cooperating classes
class authorName_NameParent {
public:
    PersistString name;
    authorName_NameParent(){ name=NULL; }
};

// ----------------------------------------------------------

class authorName_Name {

public:
    static void add(PTR(Author) p, STR c);     // add link to
    static void addNew(PTR(Author) p, char *c);  // allocate string, add link
    static STR  get(PTR(Author) p);
    static void  remove(PTR(Author) p);
    static int compare(PTR(Author) p1,PTR(Author) p2);
    // ...
    // historical DOL interface
    static STR  del(PTR(Author) p){STR  s=get(p); remove(p); return s;}
    static STR  fwd(PTR(Author) p){return get(p);}
};
    
#endif // ZZ_authorName_NAME_INCLUDED
