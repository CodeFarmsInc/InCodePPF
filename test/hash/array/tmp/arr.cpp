
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// WARNING: ONLY THE MAIN FUNCTIONS HAVE BEEN CONVERTED TO PPF
//   ... see #ifdef NOT_CONVERTED_YET
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#include <stdlib.h>
// --------------------------------------------------------
unsigned int arr_Array::maxCapacity=0;

// --------------------------------------------------------------
// Create array of pointers with the initial size s
// Returns: Returs the array, NULL when the allocation fails
// --------------------------------------------------------------
PTR(A) arr_Array::form(PTR(B) hp,unsigned int const sz,int const incr){
    A at; // temporary  A object

    if(hp->ZZds.ZZarr.array)free(hp);
    hp->ZZds.ZZarr.array.newArr(sz);
    if(hp->ZZds.ZZarr.array==NULL){
        printf("arr error, cannot allocate array size=%d\n",sz);
        return NULL;
    }
    hp->ZZds.ZZarr.sz=sz;
    hp->ZZds.ZZarr.num=0;
    hp->ZZds.ZZarr.incr=incr;

    at=hp->ZZds.ZZarr.array[sz-1]; // in order to record the end-of-array on the file
    return hp->ZZds.ZZarr.array;
}

unsigned int arr_Array::size(PTR(B) hp) {return (unsigned int)(hp->ZZds.ZZarr.num);} 

A arr_Array::get(PTR(B) hp,const unsigned int k){  // a= &(array[k])
    if(k>=hp->ZZds.ZZarr.sz)sizeChange(hp,k+1,0);
    return hp->ZZds.ZZarr.array[k];
}

void arr_Array::set(PTR(B) hp,const unsigned int k,PTR(A) ap){ // array[k]=(*a)
    int kk=k;
    if(kk>=hp->ZZds.ZZarr.sz)sizeChange(hp,kk+1,0);
    if(kk>=hp->ZZds.ZZarr.num)hp->ZZds.ZZarr.num=kk+1;
    hp->ZZds.ZZarr.array[kk]=(*ap);
}

void arr_Array::set(PTR(B) hp,const unsigned int k,A a){ // array[k]=(*a)
    int kk=k;
    if(kk>=hp->ZZds.ZZarr.sz)sizeChange(hp,kk+1,0);
    if(kk>=hp->ZZds.ZZarr.num)hp->ZZds.ZZarr.num=kk+1;
    hp->ZZds.ZZarr.array[kk]=a;
}

void arr_Array::free(PTR(B) hp){ 
    if(hp->ZZds.ZZarr.array){
        (hp->ZZds.ZZarr.array).delArr();
        hp->ZZds.ZZarr.array=NULL;
    }
    hp->ZZds.ZZarr.num=0;
    hp->ZZds.ZZarr.sz=0;
}

// --------------------------------------------------------------
// This function changes the array to new capacity, newCap.
// Parameter 'exact' determines the multiples in which the array should grow:
//  exact=0  frow using the increment
//  exact=1  exactly to newSz
// When decreasing the size of the array, it is always decreased to
// exactly newSz.
// When the size is fixed, only a change for newSz to be exact is accepted.
//   This prevents erroneous automatic size change, but still allows to
// enforce the array size when intentional.
// Warning: When reducing the size, entries with index >=newSz are
//          removed from the array.
// Returns 0 if everything OK, returns 1 when problem.
// --------------------------------------------------------------
int arr_Array::sizeChange(PTR(B) hp,const unsigned int newCap,const int exact){
    PTR(A) a; PTR(A) arr; 
    int nCap,j,s,sn,cap,num,incr; long oldInd,adj;
    A at; // temporary, not persistent object

    nCap=newCap;
    arr=hp->ZZds.ZZarr.array;
    cap=hp->ZZds.ZZarr.sz;
    num=hp->ZZds.ZZarr.num;
    incr=hp->ZZds.ZZarr.incr;

    if(incr==0 && !exact){
    printf("error for arr: attempt to change a fixed array into flexible one\n");
    return 1;
    }
    if(!arr){
        printf("error for arr: cannot modify capacity, array not formed\n");
        return 1;
    }

    if(nCap==cap)return 0; // nothing to do
    if(maxCapacity>0 && nCap>maxCapacity){
        printf("error for arr: requested capacity=%d, maxCapacity=%d\n",
                                                 nCap,maxCapacity);
        return 1;
    }

    if(nCap<cap || exact==1)s=cap;
    else { // increase the size using increment
        for(s=cap; s<nCap; s=sn){
            if(incr>0) sn=s+incr;
            else       sn=s*(-incr);
          
            if(maxCapacity>0 && sn>maxCapacity){s=maxCapacity; break;}
        }
    }

	// If the array which we are trying to enlarge is at the end of the disk file,
	// we do not have to replace the entire array, only expand its size.
	// This saves a lot of space and processing time, and is a relatively frequent case.
	oldInd=arr.getIndex();
	adj=a.adjArray(oldInd,cap,s);
	if(adj==0){ // we have to reallocate the array and move the old one to the free storage
		a.newArr(s);
        if(a==NULL){
            printf("error for arr: failed to allocate array capacity=%d\n",s);
            return 1;
		}
	    if(s<=num)sn=s-1; else sn=num;
        for(j=0; j<=sn; j++){
	        a[j]=arr[j]; // will this do it even in PPF? JS
        }
        arr.delArr();
        hp->ZZds.ZZarr.array=a;
	}
    hp->ZZds.ZZarr.sz=s;
    if(num>=s)hp->ZZds.ZZarr.num=s-1;

    return 0;
}

unsigned int arr_Array::capacity(PTR(B) hp) {return hp->ZZds.ZZarr.sz;} 

// --------------------------------------------------------------
// The usual qsort() cannot be used for sorting PPF arrays, because
// PPF does not allow casting to void* or char*; all objects have 
// have their proper type.
// For this reason, I coded a special PPF generic version of Quicksort
// based on pivotal splitting, and using the algorithm described 
// in T.A.Budd's "Classical Data structures in C++", pp.344-345.
// Unfortunately this is a recursive code which represents some
// performance loss.
// arr_sortFun is the compare function, same as used for the standard
// qsort.
// Array arr should be split in the section from i1 to i2, including
// both ends.
// For subsets smaller for 4 elements or less, we use simple bubble sort.
// --------------------------------------------------------------
#define SWAP(I,J) {at=arr[I]; arr[I]=arr[J]; arr[J]=at;}
	

void arr_Array::split(arr_sortFun cmp,PTR(B) hp,long i1, long i2){
    long i,k,low,high,n;
    static A at; // temporary object for exchanges
    PTR(A) arr=hp->ZZds.ZZarr.array;
    i=0;  // otherwise you get a false compiler warning (with MS VS2010)

    n=i2-i1;
    switch (n){
	case 0: return;
	case 1: if(cmp(get(hp,i1),get(hp,i2)) > 0){
		       	SWAP(i1,i2); 
		}
		return;
	case 2:
	case 3: for(i=i1; i<i2; i++){
                  for(k=i+1; k<=i2; k++){
	            if(cmp(get(hp,i),get(hp,k)) > 0){
			    SWAP(i,k);
	            }
                  }
		}
		return;
	default: break;
    }

    // for longer sections, use the pivotal split
    k=(i1+i2)/2; SWAP(k,i1);
    low=i1+1; high=i2;
    while(low<=high){
	if     (cmp(get(hp,low),get(hp,i1)) <= 0){
		low++;
	}
	else if(cmp(get(hp,i1),get(hp,high)) <  0){
		high--;
	}
	else {
	       	SWAP(low,high);
	}
    }
    // put pivot between the two groups
    SWAP(i1,high);
    // split the remaining parts recursively
    if(high-1>i1)split(cmp,hp,i1,high-1);
    if(high+1<i2)split(cmp,hp,high+1,i2);
}

// -----------------------------------------------------------------------
// Binary search, returns the index of the object with the same key as obj
// or the first object with the higher key than that.
// Function also returns int match=1 if a perfect match is found, returns 0
// otherwise.
// When bigger than the last index, it returns -1, and match=0.
// Return -2 means that the search has not been performed because
// the array has not been formed or is empty.
// -----------------------------------------------------------------------
int arr_Array::binSearch(arr_sortFun cmpF,PTR(B) hp,A obj,int *match,int i1,int i2){
    int k,cmp,num; PTR(A) arr; long sz;
    static A at; // temporary (not persistent) object

    arr=hp->ZZds.ZZarr.array;
    num=hp->ZZds.ZZarr.num;
    if(i1<0)i1=0;
    if(i2>num-1)i2=num-1;
    if(arr==NULL || num<=0 || i2<i1){
        printf("Warning: array arr not formed or empty, bypassing binSearch() \n");
        return  -2;
    }

    k= -1;
    
    at=arr[i1];
    cmp=cmpF(obj,at);
    if(cmp==0){*match=1; return i1;}
    if(cmp<0) {*match=0; return i1;}
    at=arr[i2]; cmp=cmpF(obj,at);
    if     (cmp>0) {*match=0; return -1;}
    else if(cmp==0){*match=1; return i2;} 

    k=(i1+i2)/2;
    while(k>i1){
        at=arr[k]; cmp=cmpF(at,obj);
        if(cmp>0)i2=k;
        else if(cmp<0)i1=k;
        else {*match=1; return k;}
        k=(i1+i2)/2;
    }
    k++;
    at=arr[k]; cmp=cmpF(at,obj);
    if(cmp==0)*match=1; else *match=0;
    return k;
}

int arr_Array::binSearch(arr_sortFun cmpF,PTR(B) hp, A obj,int *match){
	return binSearch(cmpF,hp,obj,match,0,hp->ZZds.ZZarr.num-1); }


// sort the entire array
void arr_Array::sort(arr_sortFun cmp,PTR(B) hp){
	split(cmp,hp,0,hp->ZZds.ZZarr.num -1);
}

// sort only the sections (including the endpoints
void arr_Array::sort(arr_sortFun cmp,PTR(B) hp,long i1,long i2){
	split(cmp,hp,i1,i2);
}

int arr_Array::formed(PTR(B) hp){ if(hp->ZZds.ZZarr.array)return 1; return 0;}



// ===============================================================
#ifdef NOT_CONVERTED_YET // WARNING: check 'num' when converting
// It is the number of the valid entries, NOT the last index !!!
// ===============================================================

int arr_Array::increment(PTR(B) hp) {return hp->ZZds.ZZarr.incr;} 

// --------------------------------------------------------------
// Remove the entry from the array, and shrink the remaining part
// of the array. This changes 'num' and indexes of all entries
// above this index.
// See array.h for method remove() which is much faster but changes
// the order of the array.
// --------------------------------------------------------------
void arr_Array::extract(PTR(B) hp, const int k){
    int j,num;

    num=hp->ZZds.ZZarr.num;
    if(k>=num)return; // nothing to do
    if(k==num-1){hp->ZZds.ZZarr.num=num-1; return;}
    for(j=k+1; j<num; j++)hp->ZZds.ZZarr.array[j-1]=hp->ZZds.ZZarr.array[j];
    hp->ZZds.ZZarr.num=num-1;
}

// --------------------------------------------------------------
// Insert the entry into the array[k], and expand the remaining part
// of the array. This changes 'num' and indexes of all entries
// above this index. 
// --------------------------------------------------------------
void arr_Array::insert(PTR(B) hp,const int k,A *t){
    int j,kk,oldNum,newNum,sz;

    if(k<0)kk=0; else kk=k;
    oldNum=hp->ZZds.ZZarr.num;
    sz=hp->ZZds.ZZarr.sz;
    if(kk>oldNum) newNum=kk; else newNum=oldNum+1;
    if(newNum>=sz){
        (void)sizeChange(hp,newNum+1,0);
    }
    for(j=oldNum; j>=kk; j--) hp->ZZds.ZZarr.array[j+1]=hp->ZZds.ZZarr.array[j];
    hp->ZZds.ZZarr.array[kk]=(*t);
    hp->ZZds.ZZarr.num=newNum;
}

// --------------------------------------------------------------
// Return pointer to the element[i] of the array.
// WARNING: The pointer thus obtained must be used either immediately,
//          or when we are absolutely sure that the array will not 
//          automatically re-allocate, for example when working with
//          a fixed array (incr=0).
// --------------------------------------------------------------
PTR(A) arr_Array::ind(PTR(B) hp,int i){
    if(!(hp->ZZds.ZZarr.array)){
        printf("error for arr: index operation attempted before formed\n");
        return NULL;
    }
    if(i>=hp->ZZds.ZZarr.sz){
        if(sizeChange(hp,i+1,0))return NULL; // error message inside there
    }
    if(hp->ZZds.ZZarr.num<i)hp->ZZds.ZZarr.num=i;
    return &(hp->ZZds.ZZarr.array[i]);
}

// --------------------------------------------------------------
// Fast way of getting ind[0]
// --------------------------------------------------------------
PTR(A) arr_Array::head(PTR(B) hp){
    return (A*)(hp->ZZds.ZZarr.array);
}


// --------------------------------------------------------------
// The following functions use the array as a partially sorted binary heap, 
//    inHeap() inserts element e into the heap,
//    outHeap() returns in e the top of the heap, and reduces the heap,
//    updHeap() updates the heap if the element in postion i changes its value,
//    delHeap() deletes elemenr in the position i and updates the heap.
//    indHeap() is a private utility, ind() function with the callback
// All these functions use cmpF() to compare two entries from the array.
// Whenever an element of the heap changes its postion, the new position
// is reported by the callback function. This is handy in some advance
// application.
// --------------------------------------------------------------

// --------------------------------------------------------------
// Insert the new element, e, into the heap. The entire object is inserted,
// not just its pointer.
// --------------------------------------------------------------
void arr_Array::inHeap(arr_sortFun cmpF,PTR(B) hp,PTR(A) e,arr_callback callback){
    int i; PTR(A) newE;

    i=hp->ZZds.ZZarr.num + 1;
    newE=indHeap(hp,i,callback);
    if(!newE)return;
    (*newE)=(*e);
    if(callback) (*callback)(&(hp->ZZds.ZZarr.array[i]),i);
    if(i>1)upHeap(cmpF,hp,i,callback);
}

// --------------------------------------------------------------
// Assuming a new element in location n, re-sort the heap proceeding
// from n to all its ascendents.
// --------------------------------------------------------------
void arr_Array::upHeap(arr_sortFun cmpF,PTR(B) hp,int n,arr_callback callback){
    int i1,i2; PTR(A) a1; PTR(A) a2;

    // aa is a temporary object used in exchanges, not persistent.
    // It must be automatically allocated, not from heap,
    // otherwise it adds data space for each run.
    A aa; 

    if(n==0)return;
    // traverse the ascendants
    for(i1=n, i2=(n-1)/2, a1=ind(hp,n); i1>0; i1=i2, i2=(i2-1)/2, a1=a2){
        a2=ind(hp,i2);
        if(cmpF(a2,a1)<=0)break;
        // exchange the contents of a1 and a2
        aa=(*a1); (*a1)=(*a2); (*a2)=aa;
        if(callback){
            (*callback)(&(hp->ZZds.ZZarr.array[i1]),i1);
            (*callback)(&(hp->ZZds.ZZarr.array[i2]),i2);
        }
    }
}

// --------------------------------------------------------------
// Return the top of the heap in e, and remove it from the heap.
// Return 1 when successful, return 0 when the heap is empty
// --------------------------------------------------------------
int arr_Array::outHeap(arr_sortFun cmpF,PTR(B) hp,PTR(A) e,arr_callback callback){
    int num;

    num=hp->ZZds.ZZarr.num;
    if(num<0) return 0;
    (*e)=hp->ZZds.ZZarr.array[0];
    if(callback) (*callback)(&(hp->ZZds.ZZarr.array[0]),-1);
    if(num>0){
       hp->ZZds.ZZarr.array[0]=hp->ZZds.ZZarr.array[num];
       if(callback) (*callback)(&(hp->ZZds.ZZarr.array[0]),0);
       downHeap(cmpF,hp,0,callback);
    }
    (hp->ZZds.ZZarr.num)--;
    return 1;
}


// --------------------------------------------------------------
// Assuming a new element in location n, re-sort the heap proceeding
// from n to all its descendents.
// --------------------------------------------------------------
void arr_Array::downHeap(arr_sortFun cmpF,PTR(B) hp,int n,arr_callback callback){
    int k,i1,i2,num; PTR(A) ak; PTR(A) a1; PTR(A) a2; 

    // aa is a temporary object used in exchanges, not persistent.
    // It must be automatically allocated, not from heap,
    // otherwise it adds data space for each run.
    A aa; 

    num=hp->ZZds.ZZarr.num;
    if(n>=num)return;
    // traverse the descendents
    for(k=n, ak=ind(hp,n); k<num; k=i1, ak=a1){
        i1=2*k+1;
        i2=2*k+2;
        if(i1>num)break;
        a1=ind(hp,i1);
        if(i2<=num){
            a2=ind(hp,i2);
            if(cmpF(a1,a2)>0){
                a1=a2;
                i1=i2;
            }
        }
        if(cmpF(ak,a1)<=0)break;
        // exchange the contents of ak and a1 (locations k and i1)
        aa=(*ak); (*ak)=(*a1); (*a1)=aa;
        if(callback){
            (*callback)(&(hp->ZZds.ZZarr.array[i1]),i1);
            (*callback)(&(hp->ZZds.ZZarr.array[k]),k);
        }
    }
}

// --------------------------------------------------------------
// Update heap when the value associated with entry n has changed
// --------------------------------------------------------------
void arr_Array::updHeap(arr_sortFun cmpF,PTR(B) hp,int n,arr_callback callback){
    int num,n1,n2;

    num=hp->ZZds.ZZarr.num;
    if(n<0 || n>num){
        printf("error updating heap=arr, n=%d num=%d\n",n,num);
        return;
    }
    downHeap(cmpF,hp,n,callback);
    upHeap(cmpF,hp,n,callback);
}

// --------------------------------------------------------------
// Delete item index n from the heap.
// --------------------------------------------------------------
void arr_Array::delHeap(arr_sortFun cmpF,PTR(B) hp,int n,arr_callback callback){
    int num; PTR(A) arr;

    num=hp->ZZds.ZZarr.num;
    arr=hp->ZZds.ZZarr.array;
    if(n<0 || n>num){
        printf("error deleting from heap=arr, n=%d num=%d\n",n,num);
        return;
    }
    (hp->ZZds.ZZarr.num)--;
    if(callback) (*callback)(&(hp->ZZds.ZZarr.array[num]),-1);
    if(n==num) return;
    arr[n]=arr[num];
    if(callback) (*callback)(&(hp->ZZds.ZZarr.array[n]),n);
    updHeap(cmpF,hp,n,callback);
}

// --------------------------------------------------------------
// Same function as ind() but providing a callback function.
// This function is needed when relocating heaps automatically.
// --------------------------------------------------------------
PTR(A) arr_Array::indHeap(PTR(B) hp,int i,arr_callback callback){
    int k,num,sz;
    PTR(A) elem;

    num=hp->ZZds.ZZarr.num;
    sz=hp->ZZds.ZZarr.sz;
    elem=ind(hp,i);
    if(!elem)return NULL;
    // report changes of position when re-allocating
    if(i>=sz){
        for(k=0; k<num; k++){
            if(callback) (*callback)(&(hp->ZZds.ZZarr.array[k]),k);
        }
    }
    return elem;
}
// --------------------------------------------------------------




void arr_Array::remove(PTR(B) hp,const unsigned int k){ // fast but order changed
    int kk=k;
    int num=hp->ZZds.ZZarr.num;
    if(kk<num){hp->ZZds.ZZarr.array[k]=hp->ZZds.ZZarr.array[num]; (hp->ZZds.ZZarr.num)--;}
}
int arr_Array::reduce(PTR(B) hp){             // reduce the array to its used size
    return sizeChange(hp,hp->ZZds.ZZarr.num+1,1);
}
int arr_Array::reduce(PTR(B) hp,const unsigned int newCap){ // reduce size to newSz
    if(newCap>=hp->ZZds.ZZarr.sz)return hp->ZZds.ZZarr.sz;
    return sizeChange(hp,newCap,1);
} 
int arr_Array::grow(PTR(B) hp,const unsigned int newCap){
    return sizeChange(hp,newCap,0);
} 
void arr_Array::push(PTR(B) hp,PTR(A) e){
    unsigned int num,sz;

    num=hp->ZZds.ZZarr.num+1; sz=hp->ZZds.ZZarr.sz;
    if(num>=sz)sizeChange(hp,num+1,0);
    hp->ZZds.ZZarr.array[num]=(*e); (hp->ZZds.ZZarr.num)++;
}

PTR(A) arr_Array::pop(PTR(B) hp){PTR(A) e=NULL; int num=hp->ZZds.ZZarr.num;
    if(num>=0){e= &(hp->ZZds.ZZarr.array[num]); (hp->ZZds.ZZarr.num)--;}
    return e;
}

void arr_Array::reset(PTR(B) hp,int newSz,int incr){
    hp->ZZds.ZZarr.num=newSz-1; hp->ZZds.ZZarr.incr=incr;
}

// -----------------------------------------------------------------------
// Add a new object into the ordered collection. This is relatively
// fast using binary search to find the position, but a section of
// the array must move to open one slot.
// -----------------------------------------------------------------------
void arr_Array::addOrd(arr_sortFun cmpF,PTR(B) hp,PTR(A) op){
    int k,found;

    if(hp->ZZds.ZZarr.num<0)k=0;
    else k=binSearch(cmpF,hp,op,&found);
    insert(hp,k,op); // insert even if the same key is already there
}

// -----------------------------------------------------------------------
// Find the given object in the sorted array (using binary search0
// and remove it from the array without changing the order of the remaining
// elements.
// -----------------------------------------------------------------------
void arr_Array::delOrd(arr_sortFun cmpF,PTR(B) hp,PTR(A) obj){
    int k,found;

    k=binSearch(cmpF,hp,obj,&found);
    if(found) delOrd(cmpF,hp,k);
    else printf("warning: calling arr::delOrd() but object not there\n");
}

// -----------------------------------------------------------------------
// Returns the value of the bin No.i (0 or 1).
// Returns -1 in case of error.
// -----------------------------------------------------------------------
int arr_Array::getBit(PTR(B) hp,int i){
    int iChar,iBit,sz,k,newBytes; PTR(A) ap; unsigned char *p,b;

    sz=sizeof(A) * hp->ZZds.ZZarr.sz; // sz is size in bytes
    if(i>=sz*8){ // sz*8 is the size in bits
        ap=ind(hp,i/(8*sizeof(A)));
        if(!ap){
            printf("arr error: cannot realocate for %d bits\n",i);
            return -1;
        }

        // initialize the new part of the array to 0
        newBytes=sizeof(A)*hp->ZZds.ZZarr.sz;
        p=(unsigned char*)(hp->ZZds.ZZarr.array);
        for(k=sz; k<newBytes; k++) p[k]=0;
    }
    else if(hp->ZZds.ZZarr.num<0){ // first call, initialize all bits to 0
        p=(unsigned char*)(hp->ZZds.ZZarr.array);
        for(k=0; k<sz; k++) p[k]=0;
    }

    k=i/(8*sizeof(A)); // object index in which bit i is
    if(hp->ZZds.ZZarr.num < k)hp->ZZds.ZZarr.num=k;

    p=(unsigned char*)(hp->ZZds.ZZarr.array);
    iChar=i/8;             // character index where the bit is
    iBit=i-iChar*8;        // bit within that character
    p=p+iChar;
    b=1 << iBit;
    if(*p & b){
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------
// Set bit No.i to val, where val=0 or 1.
// Return 0=everything OK, -1 when error.
// -----------------------------------------------------------------------
int arr_Array::setBit(PTR(B) hp,int i,int val){
    int iChar,iBit,sz,k,newBytes; PTR(A) ap; unsigned char *p,b;

    sz=sizeof(A) * hp->ZZds.ZZarr.sz; // sz is size in bytes
    if(i>=sz*8){ // sz*8 is the size in bits
        ap=ind(hp,i/(8*sizeof(A)));
        if(!ap){
            printf("arr error: cannot realocate for %d bits\n",i);
            return -1;
        }

        // initialize the new part of the array to 0
        newBytes=sizeof(A)*hp->ZZds.ZZarr.sz;
        p=(unsigned char*)(hp->ZZds.ZZarr.array);
        for(k=sz; k<newBytes; k++) p[k]=0;
    }
    else if(hp->ZZds.ZZarr.num<0){ // first call, initialize all bits to 0
        p=(unsigned char*)(hp->ZZds.ZZarr.array);
        for(k=0; k<sz; k++) p[k]=0;
    }

    k=i/(8*sizeof(A)); // object index in which bit i is
    if(hp->ZZds.ZZarr.num < k)hp->ZZds.ZZarr.num=k;

    p=(unsigned char*)(hp->ZZds.ZZarr.array);
    iChar=i/8;             // character index where the bit is
    iBit=i-iChar*8;        // bit within that character
    p=p+iChar;
    b=1 << iBit;
    if(val)*p=*p | b;
    else   *p=*p & ~b;
    return 0;
}

// -----------------------------------------------------------------------
// For the key given in op, find the index of the object in the array,
// using the binary search. If the object is not found, return 
// the index of the entry before which the new object would have
// to be inserted. The function returns found=1 when found, =0 when not.
// -----------------------------------------------------------------------
int arr_Array::getOrd(arr_sortFun cmpF,PTR(B) hp,PTR(A) op,int *found){
    int k=binSearch(cmpF,hp,op,found);
    return k;
}

#endif // NOT_CONVERTED_YET


