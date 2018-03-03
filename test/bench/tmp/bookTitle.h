#ifndef ZZ_bookTitle_NAME_INCLUDED
#define ZZ_bookTitle_NAME_INCLUDED
#include PPF_PATH

// -------- data structure SINGLY LINKED LIST ---------------
// EQUIVALENT OF:
// template <class Parent> class bookTitle_Name {
// ----------------------------------------------------------

class Book;

// description of the cooperating classes
class bookTitle_NameParent {
public:
    PersistString name;
    bookTitle_NameParent(){ name=NULL; }
};

// ----------------------------------------------------------

class bookTitle_Name {

public:
    static void add(PTR(Book) p, STR c);     // add link to
    static void addNew(PTR(Book) p, char *c);  // allocate string, add link
    static STR  get(PTR(Book) p);
    static void  remove(PTR(Book) p);
    static int compare(PTR(Book) p1,PTR(Book) p2);
    // ...
    // historical DOL interface
    static STR  del(PTR(Book) p){STR  s=get(p); remove(p); return s;}
    static STR  fwd(PTR(Book) p){return get(p);}
};
    
#endif // ZZ_bookTitle_NAME_INCLUDED
