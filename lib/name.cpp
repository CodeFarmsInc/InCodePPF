#include <string.h>

class $1;

void $$_Name::add(PTR($1) p, STR c){
    STR s=p->$0.name;
    if(s!=NULL){
        printf("$$:add() error: object=%d has already a name\n",p); return;
    }
    p->$0.name=c;
}

void $$_Name::addNew(PTR($1) p, char *c){
    if(c==NULL){
        printf("$$:addNew() given a NULL name\n"); return;
    }
    p->$0.name=new PersistString(c);
}

void $$_Name::remove(PTR($1) p1){
    if(p1->$0.name==NULL)return; (p1->$0.name).delString(); p1->$0.name=NULL; }

STR $$_Name::get(PTR($1) p){ return p->$0.name;}

int $$_Name::compare(PTR($1) p1, PTR($1) p2){
    return strcmp((p1->$0.name).getPtr(), (p2->$0.name).getPtr());
}

