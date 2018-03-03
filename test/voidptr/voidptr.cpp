// Test the persistency and use of class PersistVoid which replaces
// regular pointer (void*)
// --------------------------------------------------------------------------
// Syntax:  testABC 1 ... generates the data, tests and saves it 
//          testABC 2 ... reload data from disk and tests it is the same
//
#include <iostream>
using namespace std;
#include "environ.h"
#include "gen.h"
#include "c:\ppf\factory.h"


class B;
class C;
#include "b.h"
#include "c.h"

int cmpF(PTR(C) const c1,PTR(C) const c2){
    return c1->val() - c2->val();
}

int main(int argc,char **argv){
    int i,k,createNew;
    // iterator is automatically provided
    PTR(B) bp; PTR(C) cp;

    if(argc<2 || argc>2)createNew= -1;
    else if(!strcmp("1",argv[1])){createNew=1; }
    else if(!strcmp("2",argv[1])){createNew=0; }
    else                         createNew= -1;
    if(createNew==(-1)){
  cout<<"SYNTAX: test n // n=1 creates data, n=2 Rmode,n=3 RWmode\n";
        return 2;
    }
    

    if(createNew){
        system("del *.ppf");
    }

    // start the three pagers, last parameter 0=read or write
    B::startPager(24,2,100000,0);
    C::startPager(64,10,100000,0);
    PersistStart;
    
    if(createNew){
        // create test data one B, with 10 C's attached
        bp=new B(0);
	bp.setRoot();
        for(k=1;k<=10;k++){
            cp=new C(k);
            BtoC::addTail(bp,cp);
        }

        // test whether PersistVoid is active
        printf("PersistVoid pager, active=%d\n",PersistVoid::isActive());

        // set the array of PersistVoid on bp
        bp->setArray(bp);
    }
    else { // get the data from the disk
	bp.getRoot();
	if(bp==NULL)printf("WARNING: the root is NULL\n");
    }

    // print the data
    bp->prtList(bp);
    bp->prtArray();


    B::closePager();
    C::closePager();
    PersistClose;
    return(0);
}

PersistImplement(B);
PersistImplement(C);

#include "gen.cpp"
#include "c:\ppf\pointer.cpp"
