#include <string.h>

class Author;

void authorName_Name::add(PTR(Author) p, STR c){
    STR s=p->ZZds.ZZauthorName.name;
    if(s!=NULL){
        printf("authorName:add() error: object=%d has already a name\n",p); return;
    }
    p->ZZds.ZZauthorName.name=c;
}

void authorName_Name::addNew(PTR(Author) p, char *c){
    if(c==NULL){
        printf("authorName:addNew() given a NULL name\n"); return;
    }
    p->ZZds.ZZauthorName.name=new PersistString(c);
}

void authorName_Name::remove(PTR(Author) p1){
    if(p1->ZZds.ZZauthorName.name==NULL)return; (p1->ZZds.ZZauthorName.name).delString(); p1->ZZds.ZZauthorName.name=NULL; }

STR authorName_Name::get(PTR(Author) p){ return p->ZZds.ZZauthorName.name;}

int authorName_Name::compare(PTR(Author) p1, PTR(Author) p2){
    return strcmp((p1->ZZds.ZZauthorName.name).getPtr(), (p2->ZZds.ZZauthorName.name).getPtr());
}

