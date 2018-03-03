// A simple test of basic array features
// --------------------------------------------------------------------------
// Syntax:  testABC 1 ... generates the tests data and saves it 
//          testABC 2 ... loads the data tests that it is still there
//
#include <iostream>
using namespace std;
#include "environ.h"
#include "gen.h"

class A;
class B;
#include "a.h"
#include "b.h"

int cmpF(A const a1,A const a2){return a1.aval-a2.aval;}


int main(int argc,char **argv){
    int i,sz,step,match;
    // iterators are not needed for Array
    PTR(A) ap; PTR(B) bp; 
    A at,ar; // temporary, not persistent objects

    if(!strcmp("1",argv[1])){step=1;}
    else if(!strcmp("2",argv[1])){step=2;} 
    else if(!strcmp("3",argv[1])){step=3;} 
    else if(!strcmp("4",argv[1])){step=4;} // just retrieves and prints
    else {
        cout<<"SYNTAX: test n // n=1 creates data, n=2 Rmode,n=3 RWmode\n";
        return 2;
    }
    
    if(step==1){
        system("del *.ppf");
    }

    // start the two pagers, last parameter 0=read or write
    A::startPager(24,40,2000,0);
    B::startPager(24,2,100,0);
    PersistStart;
    
    if(step==1){
        // create one B object with an array of A objects
        bp=new B;
	bp.setRoot();
	arr::form(bp,15,-2);
	for(i=0; i<10; i++){ // set aval on some entries
	    at=arr::get(bp,i);
	    at.aval=1000+i;
	    arr::set(bp,i,at);
        }
    }

    else {
	bp.getRoot(); // get the data from disk
	if(bp==NULL) {
            cout << "WARNING: the root is NULL\n";
	    return 3;
	}

	if(step==2){
	    // add a new section to the array, with a gap
	    for(i=20; i<25; i++){ // set aval on some entries
	        at=arr::get(bp,i);
	        at.aval=1000+i;
	        arr::set(bp,i,at);
            }
        }
    }

    if(step==3){
	at=arr::get(bp,29);
	at.aval=99;
	arr::set(bp,29,at);

	for(i=10; i<20; i++){ // set aval on some entries
	    at=arr::get(bp,i);
	    at.aval=1000-i;
	    arr::set(bp,i,at);
        }

	// sort the array between 1 and 24
	arr::sort(cmpF,bp); // leaves out the last, isolated value
    }

    if(step==4){
        // test binary search for 4 cases, and try both versions of binSearch()
        at.aval= -10;   // before beginning
        i=arr::binSearch(cmpF,bp,at,&match); 
        ar=arr::get(bp,i);
        printf("search entire array for %d, found match=%d i=%d val=%d\n",at.aval,match,i,ar.aval);

        at.aval=995;  // between values
        i=arr::binSearch(cmpF,bp,at,&match,0,24); 
        ar=arr::get(bp,i);
        printf("search between[0,24] for %d, found match=%d i=%d val=%d\n",at.aval,match,i,ar.aval);

        at.aval=1007; // exact hit
        i=arr::binSearch(cmpF,bp,at,&match,0,24); 
        ar=arr::get(bp,i);
        printf("search between[0,24] for %d, found match=%d i=%d val=%d\n",at.aval,match,i,ar.aval);

        at.aval=1025; // beyond the end
        i=arr::binSearch(cmpF,bp,at,&match); 
        printf("search entire array for %d, found match=%d i=%d\n",at.aval,match,i);
    }


    // all steps print print this at the end
    printf("\nRESULT BEFORE SAVING TO DISK:\n");
    bp->prt();
    sz=arr::size(bp);
    for(i=0; i<sz; i++){
	printf("%d ",i);
	at=arr::get(bp,i);
	at.prt();
    }

    A::closePager();
    B::closePager();
    return(0);
}

PersistImplement(A);
PersistImplement(B);

#include "gen.cpp"
#include "c:\ppf\pointer.cpp"
