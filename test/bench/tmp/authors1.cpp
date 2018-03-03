
// --------------------------------------------------------
class Library;
class Author;

PTR(Author) const authors_LinkedList2::tail(PTR(Library) p){return p->ZZds.ZZauthors.tail;}

PTR(Author) const authors_LinkedList2::head(PTR(Library) p){
    if(!(p->ZZds.ZZauthors.tail) || !(p->ZZds.ZZauthors.tail->ZZds.ZZauthors.next))return NULL;
    else return p->ZZds.ZZauthors.tail->ZZds.ZZauthors.next;
}

void authors_LinkedList2::addHead(PTR(Library) p, PTR(Author) c){
    p->ZZds.ZZauthors.tail=authors_Ring2::addHead(p->ZZds.ZZauthors.tail,c);
}

void authors_LinkedList2::addTail(PTR(Library) p, PTR(Author) c){
    p->ZZds.ZZauthors.tail=authors_Ring2::addTail(p->ZZds.ZZauthors.tail,c);
}

void authors_LinkedList2::append(PTR(Library) p,PTR(Author) c1, PTR(Author) c2){
    p->ZZds.ZZauthors.tail=authors_Ring2::append(p->ZZds.ZZauthors.tail,c1,c2);
}

void authors_LinkedList2::insert(PTR(Author) c1, PTR(Author) c2){authors_Ring2::insert(c1,c2);}

void authors_LinkedList2::sort(authors_sortFun cmpFun, PTR(Library) p){
    p->ZZds.ZZauthors.tail=authors_Ring2::sort(cmpFun,p->ZZds.ZZauthors.tail);
}

void authors_LinkedList2::remove(PTR(Library) p, PTR(Author) c){
    p->ZZds.ZZauthors.tail=authors_Ring2::remove(p->ZZds.ZZauthors.tail,c);
}

PTR(Author) const authors_LinkedList2::next(PTR(Library) p,PTR(Author) c){ 
                               return authors_Ring2::next(p->ZZds.ZZauthors.tail,c);}

PTR(Author) const authors_LinkedList2::prev(PTR(Library) p,PTR(Author) c){ 
                               return authors_Ring2::prev(p->ZZds.ZZauthors.tail,c);}

PTR(Author) const authors_LinkedList2::nextRing(PTR(Author) c){ return authors_Ring2::nextRing(c);}

PTR(Author) const authors_LinkedList2::prevRing(PTR(Author) c){ return authors_Ring2::prevRing(c);}

// ---------------------------------------------------------------
// This function either merges two lists (if s and t are on different lists)
// or splits a list (if s and t are on the same list).
//
// When merging lists, p must be a parent of t, and will be set p->tail=NULL.
// When splitting a list, p must be an empty list holder (p->tail==NULL)
// and will be set p->tail=t.
// ---------------------------------------------------------------
void authors_LinkedList2::merge(PTR(Author) s,PTR(Author) t,PTR(Library) p){
    PTR(Author) pp,tail; int merge;
 
    tail=p->ZZds.ZZauthors.tail;
    if(tail==NULL)merge=0; else merge=1; // 0=splitting
    if(merge){
        // check that t is child of p
        for(pp=t->ZZds.ZZauthors.next; pp; pp=pp->ZZds.ZZauthors.next){
            if(pp==tail)break;
            if(pp==t)pp=NULL;
        }
        if(!pp){
            printf("authors error in merge(): merging, inconsistent input\n    ");
            printf("p is not the parent of t\n");
        }
    }
    else {
        // check that s and t are on the same list
        for(pp=s->ZZds.ZZauthors.next; pp; pp=pp->ZZds.ZZauthors.next){
            if(pp==t)break;
            if(pp==s)pp=NULL;
        }
        if(!pp){
            printf("authors error in merge(): splitting, inconsistent input\n    ");
            printf("p has not children but s and t are not in the same list\n");
        }
    }
    
    authors_Ring2::merge(s,t); 
    if(merge)p->ZZds.ZZauthors.tail=NULL; else p->ZZds.ZZauthors.tail=t;
}

PTR(Author) authors_LinkedList2::child(PTR(Library) p){
    PTR(Author) t;
    t=p->ZZds.ZZauthors.tail; if(t)return t->ZZds.ZZauthors.next; return NULL;
}

// ------------------------------------------------------------------
// check=1 checks for consistency,
// check=0 is fast but a mistake can cause a hard-to-find error
// ------------------------------------------------------------------
void authors_LinkedList2::setTail(PTR(Library) p,PTR(Author) c,int check){
    PTR(Author) pp,tail;

    tail=p->ZZds.ZZauthors.tail; 
    if(check && tail){
        for(pp=tail->ZZds.ZZauthors.next; pp; pp=pp->ZZds.ZZauthors.next){
            if(pp==c || pp==tail)break;
        }
        if(pp!=c){
            printf("authors warning: setTail() for a wrong set of children,");
            printf(" nothing done\n");
            return;
        }
    }
    p->ZZds.ZZauthors.tail=c;
}

void authors_LinkedList2Iterator::start(const PTR(Library) p){
    authors_Ring2Iterator::start(p->ZZds.ZZauthors.tail);
}

PTR(Author) authors_LinkedList2Iterator::fromHead(PTR(Library) p){
    return authors_Ring2Iterator::fromHead(p->ZZds.ZZauthors.tail); 
}

PTR(Author) authors_LinkedList2Iterator::fromTail(PTR(Library) p){
    return authors_Ring2Iterator::fromTail(p->ZZds.ZZauthors.tail); 
}
