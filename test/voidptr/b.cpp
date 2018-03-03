#include <iostream>
using namespace std;
#include "environ.h"
#include "gen.h"
#include "c:\ppf\factory.h"

class B;
class C;
#include "b.h"
#include "c.h"

void B::prtList(PTR(B) bp){
    BtoC_Iterator itBC; PTR(C) cp; int ind;
    cout << "  B=" << bb << "\n";
    itBC.start(bp);
    ITERATE(itBC,cp){
	cp->prt();
	ind=cp.getIndex();
	cout << " index=" << ind << "\n";
    }
}

// ---------------------------------------------------------------------
// Allocate a large array and fill only the beginning with the references
// to the C-objects from the list, first in ascending then descending order.
// ---------------------------------------------------------------------
void B::setArray(PTR(B) bp){
    PTR(C) cp; BtoC_Iterator itBC; int i;

    // find how many objects/references
    arrUsed=0;
    itBC.start(bp);
    ITERATE(itBC,cp){
	arrUsed++;
    }
    arrUsed=arrUsed*2; //ascending and descending;
    bp->arr.newArr(arrUsed);
    
    i=0;
    itBC.start(bp);
    ITERATE(itBC,cp){
	arr[i]=to_void(cp);
	arr[arrUsed-1-i]=arr[i];
	i++;
    }
}

// Print the objects from the void array
//  ------------------------------------
void B::prtArray(){
    int i,ind; PTR(C) cp;
    printf("print from array:\n");
    for(i=0; i<arrUsed; i++){
	cp=from_void(C,arr[i]);
	// cp= *(PTR(C)*)(&(arr[i]));
	cp->prt();
	ind=cp.getIndex();
	cout << " index=" << ind << "\n";
    }
}
