
// --------------------------------------------------------
class $1;
class $2;

PTR($2) const $$_LinkedList2::tail(PTR($1) p){return p->$0.tail;}

PTR($2) const $$_LinkedList2::head(PTR($1) p){
    if(!(p->$0.tail) || !(p->$0.tail->$0.next))return NULL;
    else return p->$0.tail->$0.next;
}

void $$_LinkedList2::addHead(PTR($1) p, PTR($2) c){
    p->$0.tail=$$_Ring2::addHead(p->$0.tail,c);
}

void $$_LinkedList2::addTail(PTR($1) p, PTR($2) c){
    p->$0.tail=$$_Ring2::addTail(p->$0.tail,c);
}

void $$_LinkedList2::append(PTR($1) p,PTR($2) c1, PTR($2) c2){
    p->$0.tail=$$_Ring2::append(p->$0.tail,c1,c2);
}

void $$_LinkedList2::insert(PTR($2) c1, PTR($2) c2){$$_Ring2::insert(c1,c2);}

void $$_LinkedList2::sort($$_sortFun cmpFun, PTR($1) p){
    p->$0.tail=$$_Ring2::sort(cmpFun,p->$0.tail);
}

void $$_LinkedList2::remove(PTR($1) p, PTR($2) c){
    p->$0.tail=$$_Ring2::remove(p->$0.tail,c);
}

PTR($2) const $$_LinkedList2::next(PTR($1) p,PTR($2) c){ 
                               return $$_Ring2::next(p->$0.tail,c);}

PTR($2) const $$_LinkedList2::prev(PTR($1) p,PTR($2) c){ 
                               return $$_Ring2::prev(p->$0.tail,c);}

PTR($2) const $$_LinkedList2::nextRing(PTR($2) c){ return $$_Ring2::nextRing(c);}

PTR($2) const $$_LinkedList2::prevRing(PTR($2) c){ return $$_Ring2::prevRing(c);}

// ---------------------------------------------------------------
// This function either merges two lists (if s and t are on different lists)
// or splits a list (if s and t are on the same list).
//
// When merging lists, p must be a parent of t, and will be set p->tail=NULL.
// When splitting a list, p must be an empty list holder (p->tail==NULL)
// and will be set p->tail=t.
// ---------------------------------------------------------------
void $$_LinkedList2::merge(PTR($2) s,PTR($2) t,PTR($1) p){
    PTR($2) pp,tail; int merge;
 
    tail=p->$0.tail;
    if(tail==NULL)merge=0; else merge=1; // 0=splitting
    if(merge){
        // check that t is child of p
        for(pp=t->$0.next; pp; pp=pp->$0.next){
            if(pp==tail)break;
            if(pp==t)pp=NULL;
        }
        if(!pp){
            printf("$$ error in merge(): merging, inconsistent input\n    ");
            printf("p is not the parent of t\n");
        }
    }
    else {
        // check that s and t are on the same list
        for(pp=s->$0.next; pp; pp=pp->$0.next){
            if(pp==t)break;
            if(pp==s)pp=NULL;
        }
        if(!pp){
            printf("$$ error in merge(): splitting, inconsistent input\n    ");
            printf("p has not children but s and t are not in the same list\n");
        }
    }
    
    $$_Ring2::merge(s,t); 
    if(merge)p->$0.tail=NULL; else p->$0.tail=t;
}

PTR($2) $$_LinkedList2::child(PTR($1) p){
    PTR($2) t;
    t=p->$0.tail; if(t)return t->$0.next; return NULL;
}

// ------------------------------------------------------------------
// check=1 checks for consistency,
// check=0 is fast but a mistake can cause a hard-to-find error
// ------------------------------------------------------------------
void $$_LinkedList2::setTail(PTR($1) p,PTR($2) c,int check){
    PTR($2) pp,tail;

    tail=p->$0.tail; 
    if(check && tail){
        for(pp=tail->$0.next; pp; pp=pp->$0.next){
            if(pp==c || pp==tail)break;
        }
        if(pp!=c){
            printf("$$ warning: setTail() for a wrong set of children,");
            printf(" nothing done\n");
            return;
        }
    }
    p->$0.tail=c;
}

void $$_LinkedList2Iterator::start(const PTR($1) p){
    $$_Ring2Iterator::start(p->$0.tail);
}

PTR($2) $$_LinkedList2Iterator::fromHead(PTR($1) p){
    return $$_Ring2Iterator::fromHead(p->$0.tail); 
}

PTR($2) $$_LinkedList2Iterator::fromTail(PTR($1) p){
    return $$_Ring2Iterator::fromTail(p->$0.tail); 
}
