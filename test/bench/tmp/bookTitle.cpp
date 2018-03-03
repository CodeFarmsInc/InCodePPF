#include <string.h>

class Book;

void bookTitle_Name::add(PTR(Book) p, STR c){
    STR s=p->ZZds.ZZbookTitle.name;
    if(s!=NULL){
        printf("bookTitle:add() error: object=%d has already a name\n",p); return;
    }
    p->ZZds.ZZbookTitle.name=c;
}

void bookTitle_Name::addNew(PTR(Book) p, char *c){
    if(c==NULL){
        printf("bookTitle:addNew() given a NULL name\n"); return;
    }
    p->ZZds.ZZbookTitle.name=new PersistString(c);
}

void bookTitle_Name::remove(PTR(Book) p1){
    if(p1->ZZds.ZZbookTitle.name==NULL)return; (p1->ZZds.ZZbookTitle.name).delString(); p1->ZZds.ZZbookTitle.name=NULL; }

STR bookTitle_Name::get(PTR(Book) p){ return p->ZZds.ZZbookTitle.name;}

int bookTitle_Name::compare(PTR(Book) p1, PTR(Book) p2){
    return strcmp((p1->ZZds.ZZbookTitle.name).getPtr(), (p2->ZZds.ZZbookTitle.name).getPtr());
}

