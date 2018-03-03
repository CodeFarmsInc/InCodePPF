// ----------------------------------------------------------
//                    2XtoX<Relation,Entity1,Entity2>
//
// This is the most frequently used Many-To-Many relation between
// two entity types. For each recorded relation there is on Relation object.
// ----------------------------------------------------------
// By repeating this code with minor modifications, it is easy
// to derive higher order many-to-many relations:
//    from 2XtoX to derive 2XtoX (this code)
//    from 2XtoX to derive 3XtoX
//    from 3XtoX to derive 4XtoX
//    ... and so on ...
//                                    Jiri Soukup, July 14, 2005
// ----------------------------------------------------------

// ----------------------------------------------------------
#ifndef ZZ_booksToAuthors_2XTOX_INCLUDED
#define ZZ_booksToAuthors_2XTOX_INCLUDED

class BookToAuthor;
class Book;
class Author;

// description of the cooperating classes
class booksToAuthors_2XtoXRelation : public booksToAuthors_1XtoXRelation {
public:
    PTR(BookToAuthor) next2;
    PTR(BookToAuthor) prev2;
    PTR(Author) parent2;
    booksToAuthors_2XtoXRelation() : booksToAuthors_1XtoXRelation(){ next2=prev2=NULL; parent2=NULL;}
};

class booksToAuthors_2XtoXEntity1 : public booksToAuthors_1XtoXEntity1 {
public:
    booksToAuthors_2XtoXEntity1() : booksToAuthors_1XtoXEntity1(){ }
};

class booksToAuthors_2XtoXEntity2 {
public:
    PTR(BookToAuthor) tail2;
    booksToAuthors_2XtoXEntity2(){tail2=NULL;}
};


// ----------------------------------------------------------

class booksToAuthors_2XtoX : public booksToAuthors_1XtoX {

public:
    static void add(PTR(BookToAuthor) r, PTR(Book) e1,PTR(Author) e2); // equivalent of addTail()
    static void remove(PTR(BookToAuthor) r);

    static PTR(Book) const entity1(PTR(BookToAuthor) r){return booksToAuthors_1XtoX::entity1(r);}
    static PTR(BookToAuthor) const next1(PTR(BookToAuthor) r)  {return booksToAuthors_1XtoX::next1(r);}
    static PTR(BookToAuthor) const prev1(PTR(BookToAuthor) r)  {return booksToAuthors_1XtoX::prev1(r);}

    static PTR(Author) const entity2(PTR(BookToAuthor) r);
    static PTR(BookToAuthor) const next2(PTR(BookToAuthor) r);   // returns NULL when s is the tail
    static PTR(BookToAuthor) const prev2(PTR(BookToAuthor) r);   // returns NULL when s is the head
};

class booksToAuthors_2XtoXIterator : public booksToAuthors_1XtoXIterator {
    // standard interface:          for(r=it.from1(e1); r; r=it.next1()){...}
    // standard interface:          for(r=it.from2(e2); r; r=it.next2()){...}
    PTR(BookToAuthor) tail2;
    PTR(BookToAuthor) nxt2;
public:
    PTR(BookToAuthor) from1(PTR(Book) e) {return booksToAuthors_1XtoXIterator::from1(e);}
    PTR(BookToAuthor) const next1(){return booksToAuthors_1XtoXIterator::next1();}

    PTR(BookToAuthor) from2(PTR(Author) e);
    PTR(BookToAuthor) const next2();
};

#endif // ZZ_booksToAuthors_2XTOX_INCLUDED
