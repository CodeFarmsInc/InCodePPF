
// --------------------------------------------------------
class B;
class C;

PTR(C) BtoC_LinkedList1::tail(PTR(B) p){return p->ZZds.ZZBtoC.tail;}

PTR(C) BtoC_LinkedList1::head(PTR(B) p){
    if(!(p->ZZds.ZZBtoC.tail) || !(p->ZZds.ZZBtoC.tail->ZZds.ZZBtoC.next))return NULL;
    else return p->ZZds.ZZBtoC.tail->ZZds.ZZBtoC.next;
}

void BtoC_LinkedList1::addHead(PTR(B) p, PTR(C) c){
    p->ZZds.ZZBtoC.tail=BtoC_Ring1::addHead(p->ZZds.ZZBtoC.tail,c);
}

void BtoC_LinkedList1::addTail(PTR(B) p, PTR(C) c){
    p->ZZds.ZZBtoC.tail=BtoC_Ring1::addTail(p->ZZds.ZZBtoC.tail,c);
}

void BtoC_LinkedList1::append(PTR(B) p,PTR(C) c1, PTR(C) c2){
    p->ZZds.ZZBtoC.tail=BtoC_Ring1::append(p->ZZds.ZZBtoC.tail,c1,c2);
}

void BtoC_LinkedList1::remove(PTR(B) p, PTR(C) c){
    p->ZZds.ZZBtoC.tail=BtoC_Ring1::remove(p->ZZds.ZZBtoC.tail,c);
}

PTR(C) const BtoC_LinkedList1::next(PTR(B) p,PTR(C) c){
    return BtoC_Ring1::next(p->ZZds.ZZBtoC.tail,c);
}

PTR(C) const BtoC_LinkedList1::nextRing(PTR(C) c){ return BtoC_Ring1::nextRing(c);}

void BtoC_LinkedList1::sort(BtoC_sortFun cmpFun, PTR(B) p){
    p->ZZds.ZZBtoC.tail=BtoC_Ring1::sort(cmpFun,p->ZZds.ZZBtoC.tail);
}

// ---------------------------------------------------------------
// This function either merges two lists (if s and t are on different lists)
// or splits a list (if s and t are on the same list).
//
// When merging lists, p must be a parent of t, and will be set p->tail=NULL.
// When splitting a list, p must be an empty list holder (p->tail==NULL)
// and will be set p->tail=t.
// ---------------------------------------------------------------
void BtoC_LinkedList1::merge(PTR(C) s,PTR(C) t,PTR(B) p){
    PTR(C) pp,tail; int merge;
 
    tail=p->ZZds.ZZBtoC.tail;
    if(tail==NULL)merge=0; else merge=1; // 0=splitting
    if(merge){
        // check that t is child of p
        for(pp=t->ZZds.ZZBtoC.next; pp; pp=pp->ZZds.ZZBtoC.next){
            if(pp==tail)break;
            if(pp==t)pp=NULL;
        }
        if(!pp){
            printf("BtoC error in merge(): merging, inconsistent input\n    ");
            printf("p is not the parent of t\n");
        }
    }
    else {
        // check that s and t are on the same list
        for(pp=s->ZZds.ZZBtoC.next; pp; pp=pp->ZZds.ZZBtoC.next){
            if(pp==t)break;
            if(pp==s)pp=NULL;
        }
        if(!pp){
            printf("BtoC error in merge(): splitting, inconsistent input\n    ");
            printf("p has not children but s and t are not in the same list\n");
        }
    }
    
    BtoC_Ring1::merge(s,t); 
    if(merge)p->ZZds.ZZBtoC.tail=NULL; else p->ZZds.ZZBtoC.tail=t;
}

PTR(C) BtoC_LinkedList1::child(PTR(B) p){
    PTR(C) t;
    t=p->ZZds.ZZBtoC.tail; if(t)return t->ZZds.ZZBtoC.next; return NULL;
}

// ------------------------------------------------------------------
// check=1 checks for consistency,
// check=0 is fast but a mistake can cause a hard-to-find error
// ------------------------------------------------------------------
void BtoC_LinkedList1::setTail(PTR(B) p,PTR(C) c,int check){
    PTR(C) pp,tail;

    tail=p->ZZds.ZZBtoC.tail; 
    if(check && tail){
        for(pp=tail->ZZds.ZZBtoC.next; pp; pp=pp->ZZds.ZZBtoC.next){
            if(pp==c || pp==tail)break;
        }
        if(pp!=c){
            printf("BtoC warning: setTail() for a wrong set of children,");
            printf(" nothing done\n");
            return;
        }
    }
    p->ZZds.ZZBtoC.tail=c;
}

void BtoC_LinkedList1Iterator::start(const PTR(B) p){
    BtoC_Ring1Iterator::start(p->ZZds.ZZBtoC.tail); 
}

PTR(C) BtoC_LinkedList1Iterator::fromHead(PTR(B) p){
    return BtoC_Ring1Iterator::fromHead(p->ZZds.ZZBtoC.tail); 
}
