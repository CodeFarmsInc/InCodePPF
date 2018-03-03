// The test problem quoted in Chap.4.9 of the book (The Software Revolution).
// --------------------------------------------------------------------------
// Syntax:  testABC 1 ... generates the tests data, tetst and saves it 
//          testABC 2 ... loads the data in R-only mode and modifies it
//          testABC 3 ... reads the data in the R/W mode and tests it
//
#include <iostream>
using namespace std;
#include "environ.h"
#include "gen.h"
#include "c:\ppf\factory.h"


class A;
class B;
class C;
#include "a.h"
#include "b.h"
#include "c.h"

int cmpF(PTR(C) const c1,PTR(C) const c2){
    return c1->val() - c2->val();
}

int main(int argc,char **argv){
    int i,k,n,rd,createNew;
    // iterators are automatically provided
    AtoB_Iterator itAB;
    BtoC_Iterator itBC;
    PTR(A) ap; PTR(B) bp; PTR(C) cp;

    if(argc<2 || argc>2)createNew= -1;
    else if(!strcmp("1",argv[1])){createNew=1; rd=0;}
    else if(!strcmp("2",argv[1])){createNew=0; rd=1;}
    else if(!strcmp("3",argv[1])){createNew=0; rd=0;}
    else                         createNew= -1;
    if(createNew==(-1)){
  cout<<"SYNTAX: test n // n=1 creates data, n=2 Rmode,n=3 RWmode\n";
        return 2;
    }
    

    if(createNew){
        system("del *.ppf");
    }

    // start the three pagers, last parameter 0=read or write
    A::startPager(20,2,1000,rd);
    B::startPager(50,8,100000,rd);
    C::startPager(50,8,100000,rd);
    PersistStart;
    
    if(createNew){
        // create test data one A, three B with ten C each.
        ap=new A(0);
	ap.setRoot();
        n=1; // for marking all objects
        for(i=1;i<=3;i++){
            bp=new B(n); n++;
            AtoB::addTail(ap,bp);
            for(k=1;k<=10;k++){
               cp=new C(n); n=n+7; if(n>30)n=n-30;
               BtoC::addTail(bp,cp);
            }
    
	    // sort the C data
	    BtoC::sort(cmpF,bp);
        }
    }
    else { // get the data from the disk
	ap.getRoot();
	if(ap==NULL)printf("WARNING: the root is NULL\n");
    }

    if(rd==1){ // make changes, remove the first C for every B
        itAB.start(ap);
        ITERATE(itAB,bp){
            cp=BtoC::head(bp);
	    BtoC::del(bp,cp);
            cp.delObj();
        }
    }

    // print the data
    ap->prt();
    itAB.start(ap);
    ITERATE(itAB,bp){
        bp->prt();
        itBC.start(bp);
        ITERATE(itBC,cp){
            cp->prt(); 
        }
    }

    A::closePager();
    B::closePager();
    C::closePager();
    PersistClose;
    return(0);
}

PersistImplement(A);
PersistImplement(B);
PersistImplement(C);

#include "gen.cpp"
#include "c:\ppf\pointer.cpp"
