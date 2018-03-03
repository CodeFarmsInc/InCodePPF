// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// WARNING: ONLY THE MAIN FUNCTIONS HAVE BEEN CONVERTED TO PPF
//   ... see #ifdef NOT_CONVERTED_YET
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------- data structure DYNAMICALLY ADJUSTABLE ARRAY ---------------
// EQUIVALENT OF:
// template <class Holder,class Element> class arr_Array
// --------------------------------------------------------------------
// Implementation note:
// Internally, this class works with:
//    sz =presently allocated size
//    num=number of valid entries in the array (highest index used + 1)
// The external interface conforms to the commonly used parameters of Vectors:
//    capacity=allocated size (capacity=sz)
//    size    =size actually used (size=num+1)
// --------------------------------------------------------------------

#ifndef ZZ_arr_ARRAY_INCLUDED
#define ZZ_arr_ARRAY_INCLUDED

#include "environ.h"
#include "c:\ppf\factory.h"

// ----------------------------------------------------------
// description of the cooperating classes
// ----------------------------------------------------------
class arr_ArrayHolder {
public:
    unsigned int sz; // present allocated size (capacity)
    int num,incr; // number of valid entries,increment
    PTR(A) array;
    arr_ArrayHolder(){ sz=0; num=0; incr=0; array=NULL;}
};
// NOTE: incr=0 fixed size (attempt to change the size treated as error)
//       incr>0    az=sz+incr
//       incr<0    sz=sz*(-incr)

// ----------------------------------------------------------

class arr_Array {
protected:
    typedef int (*arr_sortFun)(const A, const A); // compare for sort,heap
    typedef void (*arr_callback)(void *, int); // callback function for heap operations
    static unsigned int maxCapacity;  // upper limit on any array, 0=disabled
    static int sizeChange(PTR(B) hp,const unsigned int newCap,const int exact);
             // exact=1 new size will be exactly newCap, mode=0 grow using incr
    static void split(arr_sortFun cmp,PTR(B) hp,long i1,long i2);

public:
    static PTR(A) form(PTR(B) hp,unsigned int const sz,int const incr);
    static unsigned int size(PTR(B) hp);
    static A get(PTR(B) hp,const unsigned int k);  // a= &(array[k])
    static void set(PTR(B) hp,const unsigned int k,A a); // array[k]=(*a)
    static void set(PTR(B) hp,const unsigned int k,PTR(A) a); // array[k]=(*a)
    static void free(PTR(B) hp);
    static unsigned int capacity(PTR(B) hp);
    static void sort(arr_sortFun cmp,PTR(B) hp); // entire array
    static void sort(arr_sortFun cmp,PTR(B) hp,long i1,long i2); // only section
    static int binSearch(arr_sortFun cmpF,PTR(B) hp, A obj,int *match);
    static int binSearch(arr_sortFun cmpF,PTR(B) hp,A obj,int *match,int i1,int i2);
    static int formed(PTR(B) hp);

#ifdef NOT_CONVERTED_YET
    static void setMaxCap(unsigned int maxCap){maxCapacity=maxCap;}
    static int increment(PTR(B) hp);
    static void extract(PTR(B) hp,const int k);// remove and shrink array
    static void remove(PTR(B) hp,const unsigned int k); // fast but order changed
    static void insert(PTR(B) hp,const int k,PTR(A) t); 
    static int reduce(PTR(B) hp);             // reduce the array to its used size
    static int reduce(PTR(B) hp,const unsigned int newCap); // capacity to newCap
    static int grow(PTR(B) hp,const unsigned int newCap);
    static void sortSubset(arr_sortFun cmp,PTR(B) hp,int i1,int i2);
    static PTR(A) ind(PTR(B) hp,int i);
    static void addOrd(arr_sortFun cmpF,PTR(B) hp,PTR(A) op);

    // array as a stack 
    // ------------------------------------------------------------
    static void push(PTR(B) hp,PTR(A) e);
    static PTR(A) pop(PTR(B) hp);

    // array as a partially sorted, binary heap
    // -----------------------------------------
    static void inHeap(arr_sortFun cmpF,PTR(B) hp,PTR(A) e,arr_callback callback);
    static int outHeap(arr_sortFun cmpF,PTR(B) hp,PTR(A) e,arr_callback callback);
    static void updHeap(arr_sortFun cmpF,PTR(B) hp,int n,arr_callback callback);
    static void delHeap(arr_sortFun cmpF,PTR(B) hp,int n,arr_callback callback);

    static PTR(A) head(PTR(B) hp); //fast way of getting array[0]
    static void reset(PTR(B) hp,int newSz,int incr);
    static void ins(PTR(B) hp,int k,PTR(A) t){insert(hp,k,t);}
    static void delOrd(arr_sortFun cmpF,PTR(B) hp,int k){ extract(hp,k); }
    static void delOrd(arr_sortFun cmpF,PTR(B) hp,PTR(A) obj);
    static int getBit(PTR(B) hp,int i);
    static int setBit(PTR(B) hp,int i,int val);
    static int getOrd(arr_sortFun cmpF,PTR(B) hp,PTR(A) op,int *found);
private:
    static PTR(A) indHeap(PTR(B) hp,int i,arr_callback callback); // like ind() but callback
    static void upHeap(arr_sortFun cmpF,PTR(B) hp,int n,arr_callback callback);
    static void downHeap(arr_sortFun cmpF,PTR(B) hp,int n,arr_callback callback);
#endif // NOT_CONVERTED_YET
};

#endif // ZZ_arr_ARRAY_INCLUDED

