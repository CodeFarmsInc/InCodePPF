// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// WARNING: ONLY THE MAIN FUNCTIONS HAVE BEEN CONVERTED TO PPF
//   ... see #ifdef NOT_CONVERTED_YET
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------- data structure DYNAMICALLY ADJUSTABLE ARRAY ---------------
// EQUIVALENT OF:
// template <class Holder,class Element> class $$_Array
// --------------------------------------------------------------------
// Implementation note:
// Internally, this class works with:
//    sz =presently allocated size
//    num=number of valid entries in the array (highest index used + 1)
// The external interface conforms to the commonly used parameters of Vectors:
//    capacity=allocated size (capacity=sz)
//    size    =size actually used (size=num+1)
// --------------------------------------------------------------------

#ifndef ZZ_$$_ARRAY_INCLUDED
#define ZZ_$$_ARRAY_INCLUDED

#include "environ.h"
#include PPF_PATH

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class $$_ArrayHolder {
public:
    unsigned int sz; // present allocated size (capacity)
    int num,incr; // number of valid entries,increment
    PTR($2) array;
    $$_ArrayHolder(){ sz=0; num=0; incr=0; array=NULL;}
};
// NOTE: incr=0 fixed size (attempt to change the size treated as error)
//       incr>0    az=sz+incr
//       incr<0    sz=sz*(-incr)

// ----------------------------------------------------------

class $$_Array {
protected:
    typedef int (*$$_sortFun)(const $2, const $2); // compare for sort,heap
    typedef void (*$$_callback)(void *, int); // callback function for heap operations
    static unsigned int maxCapacity;  // upper limit on any array, 0=disabled
    static int sizeChange(PTR($1) hp,const unsigned int newCap,const int exact);
             // exact=1 new size will be exactly newCap, mode=0 grow using incr
    static void split($$_sortFun cmp,PTR($1) hp,long i1,long i2);

public:
    static PTR($2) form(PTR($1) hp,unsigned int const sz,int const incr);
    static unsigned int size(PTR($1) hp);
    static $2 get(PTR($1) hp,const unsigned int k);  // a= &(array[k])
    static PTR($2) ind(PTR($1) hp,const unsigned int k);  // a=array[k]
    static void set(PTR($1) hp,const unsigned int k,$2 a); // array[k]=(*a)
    static void set(PTR($1) hp,const unsigned int k,PTR($2) a); // array[k]=(*a)
    static void free(PTR($1) hp);
    static unsigned int capacity(PTR($1) hp);
    static void sort($$_sortFun cmp,PTR($1) hp); // entire array
    static void sort($$_sortFun cmp,PTR($1) hp,long i1,long i2); // only section
    static int binSearch($$_sortFun cmpF,PTR($1) hp, $2 obj,int *match);
    static int binSearch($$_sortFun cmpF,PTR($1) hp,$2 obj,int *match,int i1,int i2);
    static int formed(PTR($1) hp);

#ifdef NOT_CONVERTED_YET
    static void setMaxCap(unsigned int maxCap){maxCapacity=maxCap;}
    static int increment(PTR($1) hp);
    static void extract(PTR($1) hp,const int k);// remove and shrink array
    static void remove(PTR($1) hp,const unsigned int k); // fast but order changed
    static void insert(PTR($1) hp,const int k,PTR($2) t); 
    static int reduce(PTR($1) hp);             // reduce the array to its used size
    static int reduce(PTR($1) hp,const unsigned int newCap); // capacity to newCap
    static int grow(PTR($1) hp,const unsigned int newCap);
    static void sortSubset($$_sortFun cmp,PTR($1) hp,int i1,int i2);
    static PTR($2) ind(PTR($1) hp,int i);
    static void addOrd($$_sortFun cmpF,PTR($1) hp,PTR($2) op);

    // array as a stack 
    // ------------------------------------------------------------
    static void push(PTR($1) hp,PTR($2) e);
    static PTR($2) pop(PTR($1) hp);

    // array as a partially sorted, binary heap
    // -----------------------------------------
    static void inHeap($$_sortFun cmpF,PTR($1) hp,PTR($2) e,$$_callback callback);
    static int outHeap($$_sortFun cmpF,PTR($1) hp,PTR($2) e,$$_callback callback);
    static void updHeap($$_sortFun cmpF,PTR($1) hp,int n,$$_callback callback);
    static void delHeap($$_sortFun cmpF,PTR($1) hp,int n,$$_callback callback);

    static PTR($2) head(PTR($1) hp); //fast way of getting array[0]
    static void reset(PTR($1) hp,int newSz,int incr);
    static void ins(PTR($1) hp,int k,PTR($2) t){insert(hp,k,t);}
    static void delOrd($$_sortFun cmpF,PTR($1) hp,int k){ extract(hp,k); }
    static void delOrd($$_sortFun cmpF,PTR($1) hp,PTR($2) obj);
    static int getBit(PTR($1) hp,int i);
    static int setBit(PTR($1) hp,int i,int val);
    static int getOrd($$_sortFun cmpF,PTR($1) hp,PTR($2) op,int *found);
private:
    static PTR($2) indHeap(PTR($1) hp,int i,$$_callback callback); // like ind() but callback
    static void upHeap($$_sortFun cmpF,PTR($1) hp,int n,$$_callback callback);
    static void downHeap($$_sortFun cmpF,PTR($1) hp,int n,$$_callback callback);
#endif // NOT_CONVERTED_YET
};

#endif // ZZ_$$_ARRAY_INCLUDED

