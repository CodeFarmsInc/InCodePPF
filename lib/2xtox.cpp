// THIS FILE HAS NOT BEEN CONVERTED AND MAY NOT BE NEEDED
// If we do need it, than it should be renamed mton.cpp
// -------------------------------------------------------------
class $1; // relation
class $2; // entity1
class $3; // entity2
                          
void $$_2XtoX::add(PTR($1) r,PTR($2) e1,PTR($3) e2){ // equivalent of addTail()
    PTR($1) head;

    $$_1XtoX::add(r,e1);

    if(r->$0.next2){
        printf("$$.add() error: element=%d already in $$\n",r);
        return;
    }
    if(e2->$0.tail2){
        head=e2->$0.tail2->$0.next2;
        r->$0.next2=head; e2->$0.tail2->$0.next2=r;
        r->$0.prev2=e2->$0.tail2; head->$0.prev2=r;
    }
    else {e2->$0.tail2=r->$0.next2=r->$0.prev2=r;}

    e2->$0.tail2=r;
    r->$0.parent2=e2;
}

void $$_2XtoX::remove(PTR($1) r){
    PTR($1) prv,nxt; PTR($3) par;

    $$_1XtoX::remove(r);

    nxt=r->$0.next2;
    prv=r->$0.prev2;
    par=r->$0.parent2;
    if(nxt==NULL || prv==NULL || par==NULL){
        printf("$$:remove() error: node=%d not on the list\n",r); return;
    }

    if(r==nxt)par->$0.tail2=NULL;
    else {
        if(r==par->$0.tail2)par->$0.tail2=prv;
        prv->$0.next2=nxt;
        nxt->$0.prev2=prv;
    }
    r->$0.next2=r->$0.prev2=NULL;
    r->$0.parent2=NULL;
}

PTR($3) const $$_2XtoX::entity2(PTR($1) r){return r->$0.parent2;}

PTR($1) const $$_2XtoX::next2(PTR($1) r){   // returns NULL when s is the tail
    PTR($3) p2;
    p2=r->$0.parent2;
    if(!r || !p2)return NULL;
    if(r==p2->$0.tail2)return NULL;
    return r->$0.next2;
}
    

PTR($1) const $$_2XtoX::prev2(PTR($1) r){   // returns NULL when s is the head
    PTR($3) p2; PTR($1) prv;
    p2=r->$0.parent2;
    if(!r || !p2)return NULL;
    prv=r->$0.prev2;
    if(prv==p2->$0.tail2)return NULL;
    return prv;
}

PTR($1) $$_2XtoXIterator::from2(PTR($3) e){
    PTR($1) ret;

    tail2=e->$0.tail2;
    if(!tail2)return NULL;
    ret=tail2->$0.next2;
    if(ret==tail2)nxt2=tail2=NULL; 
    else nxt2=ret->$0.next2;
    return ret;
}

PTR($1) const $$_2XtoXIterator::next2(){
    PTR($1) r;

    r=nxt2;
    if(r==tail2)nxt2=tail2=NULL; else nxt2=r->$0.next2;
    return r;
}
