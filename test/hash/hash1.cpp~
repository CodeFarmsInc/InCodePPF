// -------------------------------------------------------------------------
// This is a test of class Hash.
// We have Company with Employees which we want to access quickly
// either by name or by the ID. We keep the employee records in two
// independent hash tables: byName and byID.
// For eazy checking, we also store all employees in a LinkedList eList.
// This allows us to check that all objects in either table are in the list,
// and all objects in the list are in the table.
//
// We form one tiny and one reasonably large table, load them with
// test data, and call the checking function.
// Then we resize the small table to a large one and the large one to a small
// one, and call the checking function again.
// Then we remove every second employee on the list from all records,
// and call the checking function.
//
// This test does not generate any output data except for a message that
// all the automatic checking passed correctly.
//
// This test runs with the standard interface (not using the historical DOL
// interface).
// --------------------------------------------------------------------------

#include <stdio.h>
#include "environ.h"
#include "gen.h"
#include "employee.h"
#include "company.h"


int byName::hash(PTR(Employee) e,int hashSz){
    char *s;
    s=eName::get(e).getPtr();
    return byName::hashString(s,hashSz);
}

int byID::hash(PTR(Employee) e,int hashSz){
    return byName::hashInt(e->getID(),hashSz);
}

int byName::cmp(PTR(Employee) e1,PTR(Employee) e2){
    char *s1,*s2;
    s1=eName::get(e1).getPtr();
    s2=eName::get(e2).getPtr();
    return strcmp(s1,s2);
}

int byID::cmp(PTR(Employee) e1,PTR(Employee) e2){
    int i1,i2;
    i1=e1->getID();
    i2=e2->getID();
    return i1 - i2;
}

// ------------------------------------------------------------
int main(int argc,char **argv){
    PTR(Company) cp; int i,k; PTR(Employee) ep;
    char seed[20]; strcpy(seed,"A_name_z"); int seedNum=1234;
    int last=strlen(seed)-1;
    int run; // 1 for the first run, 2 for the second run
    eList_Iterator it;

    if(argc!=2)run=0;
    else if(!strcmp("1",argv[1]))run=1;
    else if(!strcmp("2",argv[1]))run=2;
    else run=0;

    if(run==0){
        cout<<"SYNTAX:\n"; 
	cout<<"   hash1 1 ... creates data, tests it, and stores it on disk\n";
	cout<<"   hash1 2 ... reads data from disk and checks it\n";
        return 2;
    }
    

    if(run==1){
        system("del *.ppf");
    }

    Company::startPager(24,2,100,0);
    Employee::startPager(32,200,9000,0);
    PersistString::startPager(256,50,1000,0);
    PersistStart;
    // NOTE: pageSz for PersistString must be at least Company::txtBufSz


    if(run==1){
        cp=new Company(120); // text buffer 120B for query interface
        cp.setRoot();
        byName::form(cp,3);
        byID::form(cp,20);

        // generate 20 employees with unique names and ID's
        // and load them into the two hash tables and into the linked list
        for(i=k=0; i<20; i++){
            if(k>=0)k++;
            k= -k;
        (seed[0])++; (seed[last])--;
	    ep=Employee::newEmployee(seed,seedNum+k);
            eList::addHead(cp,ep);
            byName::add(cp,ep);
            byID::add(cp,ep);
        }
    }
    else { // run==2
	cp.getRoot();
	if(cp==NULL){
	    cout<<"WARNING: the root is NULL, no disk data\n";
	    return 1;
	}
    }
    
    if(run==1){
        if(cp->checkAll("after creating data",cp))return 1;
    
        byName::resize(cp,40);
        byID::resize(cp,4);
        if(cp->checkAll("after re-sizing hash tables",cp))return 1;
        Company::prtList(cp);
    
        // remove all records for every second employee on the list
        for(ep=it.fromHead(cp), k=0; ep; ep=it.next(), k=1-k){
            if(k)continue;
            eList::remove(cp,ep);
            byName::remove(cp,ep);
            byID::remove(cp,ep);
            ep.delObj();
        }
        if(cp->checkAll("after removing every second employee",cp))return 1;
    }
    else { // run==2
        i=byName::size(cp,&k);
        if(cp->checkAll("checking data from the disk",cp))return 1;
        printf("final check: count=%d - everything OK\n",k);
    }

    // print the data
    Company::prtList(cp);
    
    Company::closePager();
    Employee::closePager();
    PersistString::closePager();
    PersistClose;

    return 0;
}

PersistImplement(Company);
PersistImplement(Employee);

#include "gen.cpp"
#include "..\..\ppf\pointer.cpp"

