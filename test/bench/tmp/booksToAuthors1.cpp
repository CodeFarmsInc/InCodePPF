
// --------------------------------------------------------
class Book;
class BookToAuthor;

PTR(BookToAuthor) const booksToAuthors_LinkedList2::tail(PTR(Book) p){return p->ZZds.ZZbooksToAuthors.tail;}

PTR(BookToAuthor) const booksToAuthors_LinkedList2::head(PTR(Book) p){
    if(!(p->ZZds.ZZbooksToAuthors.tail) || !(p->ZZds.ZZbooksToAuthors.tail->ZZds.ZZbooksToAuthors.next))return NULL;
    else return p->ZZds.ZZbooksToAuthors.tail->ZZds.ZZbooksToAuthors.next;
}

void booksToAuthors_LinkedList2::addHead(PTR(Book) p, PTR(BookToAuthor) c){
    p->ZZds.ZZbooksToAuthors.tail=booksToAuthors_Ring2::addHead(p->ZZds.ZZbooksToAuthors.tail,c);
}

void booksToAuthors_LinkedList2::addTail(PTR(Book) p, PTR(BookToAuthor) c){
    p->ZZds.ZZbooksToAuthors.tail=booksToAuthors_Ring2::addTail(p->ZZds.ZZbooksToAuthors.tail,c);
}

void booksToAuthors_LinkedList2::append(PTR(Book) p,PTR(BookToAuthor) c1, PTR(BookToAuthor) c2){
    p->ZZds.ZZbooksToAuthors.tail=booksToAuthors_Ring2::append(p->ZZds.ZZbooksToAuthors.tail,c1,c2);
}

void booksToAuthors_LinkedList2::insert(PTR(BookToAuthor) c1, PTR(BookToAuthor) c2){booksToAuthors_Ring2::insert(c1,c2);}

void booksToAuthors_LinkedList2::sort(booksToAuthors_sortFun cmpFun, PTR(Book) p){
    p->ZZds.ZZbooksToAuthors.tail=booksToAuthors_Ring2::sort(cmpFun,p->ZZds.ZZbooksToAuthors.tail);
}

void booksToAuthors_LinkedList2::remove(PTR(Book) p, PTR(BookToAuthor) c){
    p->ZZds.ZZbooksToAuthors.tail=booksToAuthors_Ring2::remove(p->ZZds.ZZbooksToAuthors.tail,c);
}

PTR(BookToAuthor) const booksToAuthors_LinkedList2::next(PTR(Book) p,PTR(BookToAuthor) c){ 
                               return booksToAuthors_Ring2::next(p->ZZds.ZZbooksToAuthors.tail,c);}

PTR(BookToAuthor) const booksToAuthors_LinkedList2::prev(PTR(Book) p,PTR(BookToAuthor) c){ 
                               return booksToAuthors_Ring2::prev(p->ZZds.ZZbooksToAuthors.tail,c);}

PTR(BookToAuthor) const booksToAuthors_LinkedList2::nextRing(PTR(BookToAuthor) c){ return booksToAuthors_Ring2::nextRing(c);}

PTR(BookToAuthor) const booksToAuthors_LinkedList2::prevRing(PTR(BookToAuthor) c){ return booksToAuthors_Ring2::prevRing(c);}

// ---------------------------------------------------------------
// This function either merges two lists (if s and t are on different lists)
// or splits a list (if s and t are on the same list).
//
// When merging lists, p must be a parent of t, and will be set p->tail=NULL.
// When splitting a list, p must be an empty list holder (p->tail==NULL)
// and will be set p->tail=t.
// ---------------------------------------------------------------
void booksToAuthors_LinkedList2::merge(PTR(BookToAuthor) s,PTR(BookToAuthor) t,PTR(Book) p){
    PTR(BookToAuthor) pp,tail; int merge;
 
    tail=p->ZZds.ZZbooksToAuthors.tail;
    if(tail==NULL)merge=0; else merge=1; // 0=splitting
    if(merge){
        // check that t is child of p
        for(pp=t->ZZds.ZZbooksToAuthors.next; pp; pp=pp->ZZds.ZZbooksToAuthors.next){
            if(pp==tail)break;
            if(pp==t)pp=NULL;
        }
        if(!pp){
            printf("booksToAuthors error in merge(): merging, inconsistent input\n    ");
            printf("p is not the parent of t\n");
        }
    }
    else {
        // check that s and t are on the same list
        for(pp=s->ZZds.ZZbooksToAuthors.next; pp; pp=pp->ZZds.ZZbooksToAuthors.next){
            if(pp==t)break;
            if(pp==s)pp=NULL;
        }
        if(!pp){
            printf("booksToAuthors error in merge(): splitting, inconsistent input\n    ");
            printf("p has not children but s and t are not in the same list\n");
        }
    }
    
    booksToAuthors_Ring2::merge(s,t); 
    if(merge)p->ZZds.ZZbooksToAuthors.tail=NULL; else p->ZZds.ZZbooksToAuthors.tail=t;
}

PTR(BookToAuthor) booksToAuthors_LinkedList2::child(PTR(Book) p){
    PTR(BookToAuthor) t;
    t=p->ZZds.ZZbooksToAuthors.tail; if(t)return t->ZZds.ZZbooksToAuthors.next; return NULL;
}

// ------------------------------------------------------------------
// check=1 checks for consistency,
// check=0 is fast but a mistake can cause a hard-to-find error
// ------------------------------------------------------------------
void booksToAuthors_LinkedList2::setTail(PTR(Book) p,PTR(BookToAuthor) c,int check){
    PTR(BookToAuthor) pp,tail;

    tail=p->ZZds.ZZbooksToAuthors.tail; 
    if(check && tail){
        for(pp=tail->ZZds.ZZbooksToAuthors.next; pp; pp=pp->ZZds.ZZbooksToAuthors.next){
            if(pp==c || pp==tail)break;
        }
        if(pp!=c){
            printf("booksToAuthors warning: setTail() for a wrong set of children,");
            printf(" nothing done\n");
            return;
        }
    }
    p->ZZds.ZZbooksToAuthors.tail=c;
}

void booksToAuthors_LinkedList2Iterator::start(const PTR(Book) p){
    booksToAuthors_Ring2Iterator::start(p->ZZds.ZZbooksToAuthors.tail);
}

PTR(BookToAuthor) booksToAuthors_LinkedList2Iterator::fromHead(PTR(Book) p){
    return booksToAuthors_Ring2Iterator::fromHead(p->ZZds.ZZbooksToAuthors.tail); 
}

PTR(BookToAuthor) booksToAuthors_LinkedList2Iterator::fromTail(PTR(Book) p){
    return booksToAuthors_Ring2Iterator::fromTail(p->ZZds.ZZbooksToAuthors.tail); 
}
