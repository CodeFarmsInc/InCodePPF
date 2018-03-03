#include <string.h>

class Book;

void bookAbstract_Name::add(PTR(Book) p, STR c){
    STR s=p->ZZds.ZZbookAbstract.name;
    if(s!=NULL){
        printf("bookAbstract:add() error: object=%d has already a name\n",p); return;
    }
    p->ZZds.ZZbookAbstract.name=c;
}

void bookAbstract_Name::addNew(PTR(Book) p, char *c){
    if(c==NULL){
        printf("bookAbstract:addNew() given a NULL name\n"); return;
    }
    p->ZZds.ZZbookAbstract.name=new PersistString(c);
}

void bookAbstract_Name::remove(PTR(Book) p1){
    if(p1->ZZds.ZZbookAbstract.name==NULL)return; (p1->ZZds.ZZbookAbstract.name).delString(); p1->ZZds.ZZbookAbstract.name=NULL; }

STR bookAbstract_Name::get(PTR(Book) p){ return p->ZZds.ZZbookAbstract.name;}

int bookAbstract_Name::compare(PTR(Book) p1, PTR(Book) p2){
    return strcmp((p1->ZZds.ZZbookAbstract.name).getPtr(), (p2->ZZds.ZZbookAbstract.name).getPtr());
}

