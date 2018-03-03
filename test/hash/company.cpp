#include "environ.h"
#include "gen.h"
#include "employee.h"
#include "company.h"
#include <string.h>

// ------------------------------------------------------------
// For each Company, we use one Employee object needed for query
// interface. This object is associated with a large enough eName
// buffer so that we can reuse it without worrying about the length.
// ------------------------------------------------------------
Company::Company(int bufSz){
    STR s;
    s=new PersistString(bufSz+1); // add one for the end of string
    txtBufSz=bufSz;
    eObj=new Employee();
    eName::add(eObj,s);
}

// ------------------------------------------------------------
// Use the Employee::flg when comparing records
// Return  0 when everything correct.
// ------------------------------------------------------------
int Company::checkAll(char *label,PTR(Company) cp){
    PTR(Employee) ep,p; 
    int ret,id,k,count_name,count_id,sz;
    char name[120]; // temporary storage for names
    eList_Iterator eIter;
    byName_Iterator nIter;
    byID_Iterator iIter;

    printf("---calling checkAll(): %s\n",label);
    ret=0;
    for(ep=eIter.fromHead(cp); ep; ep=eIter.next()){
        ep->flg=0;
	strcpy(name,(eName::get(ep)).getPtr());    id=ep->getID();

	strcpy((eName::get(eObj)).getPtr(),name); 
       	eObj->setID(id);
        p=byName::get(cp,eObj);
        if(p!=ep){
            printf("byName mixup, different names=%s, %s\n",
                                         eName::get(ep),eName::get(eObj));
            ret=1;
        }
        p=byID::get(cp,eObj);
        if(p!=ep){
            printf("byID mixup, different ID=%d, %d\n",ep->getID(),eObj->getID());
            ret=1;
        }
    }
    for(ep=nIter.first(cp,-1); ep; ep=nIter.next()){
        (ep->flg)++;
    }

    for(ep=iIter.first(cp,-1); ep; ep=iIter.next()){
        (ep->flg)++;
    }

    sz=byName::size(cp,&count_name);
    sz=byID::size(cp,&count_id);
    if(count_name != count_id){
        printf("error: count_name=%d count_id=%d\n",count_name,count_id);
    }
       
    for(ep=eIter.fromHead(cp), k=0; ep; ep=eIter.next(), k++){
        if(ep->flg != 2){
            printf("error: %s %d is not in both hash tables, flg=%d\n",
                           eName::get(ep),ep->getID(),ep->flg);
            ret=1;
        }
    }

    if(k!=count_name){
        printf("error in counts: list=%d byName=%d byID=%d\n",
                                                  k,count_name,count_id);
        ret=1;
    }
    
    if(ret==0) printf("all results correct\n");
    return ret;
}
        
// ------------------------------------------------------------
// Debugging print of hash table 'byName'
// ------------------------------------------------------------
void Company::debPrtName(PTR(Company) cp){
    PTR(Employee) ep; int i,sz,count,c;
    byName_Iterator eIter;

    sz=byName::size(cp,&count);
    c=0;
    printf("\nDEBUGGING PRINT OF: byName\n");
    for(i=0; i<sz; i++){
        for(ep=eIter.first(cp,i); ep; ep=eIter.next()){
            printf("   slot=%d ",i); Employee::prt(ep);
            c++;
        }
    }
    printf("  count=%d c=%d\n\n",count,c);
}

// ------------------------------------------------------------
// Debugging print of hash table 'byID'
// ------------------------------------------------------------
void Company::debPrtID(PTR(Company) cp){
    PTR(Employee) ep; int i,sz,count,c;
    byID_Iterator eIter;

    sz=byID::size(cp,&count);
    c=0;
    printf("\nDEBUGGING PRINT OF: byID\n");
    for(i=0; i<sz; i++){
        for(ep=eIter.first(cp,i); ep; ep=eIter.next()){
            printf("   slot=%d ",i); Employee::prt(ep);
            c++;
        }
    }
    printf("  count=%d c=%d\n\n",count,c);
}

// ------------------------------------------------------------
// Debugging print of hash table 'byName' using iterator option -1
// ------------------------------------------------------------
void Company::debPrtIteratorName(PTR(Company) cp){
    PTR(Employee) ep; int count,c,sz;
    byName_Iterator nIter;

    sz=byName::size(cp,&count);
    c=0;
    printf("\nDEBUGGING PRINT OF: byName OPTION: -1\n");
    for(ep=nIter.first(cp,-1); ep; ep=nIter.next()){
	Employee::prt(ep);
        c++;
    }
    printf("  count=%d c=%d\n\n",count,c);
}

// ------------------------------------------------------------
// Debugging print of hash table 'byID' using iterator option -1
// ------------------------------------------------------------
void Company::debPrtIteratorID(PTR(Company) cp){
    PTR(Employee) ep; int count,c,sz;
    byID_Iterator iIter;

    sz=byID::size(cp,&count);
    c=0;
    printf("\nDEBUGGING PRINT OF: byID OPTION: -1\n");
    for(ep=iIter.first(cp,-1); ep; ep=iIter.next()){
	Employee::prt(ep);
        c++;
    }
    printf("  count=%d c=%d\n\n",count,c);
}

// ------------------------------------------------------------
// Debugging print of the Employees, using 'eList'
// ------------------------------------------------------------
void Company::prtList(PTR(Company) cp){
    PTR(Employee) ep; 
    eList_Iterator eIter;

    printf("\nThe list of Employees:\n");
    for(ep=eIter.fromHead(cp); ep; ep=eIter.next()){
        Employee::prt(ep);
    }
}

