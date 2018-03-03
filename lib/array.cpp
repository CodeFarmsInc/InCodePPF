
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// WARNING: ONLY THE MAIN FUNCTIONS HAVE BEEN CONVERTED TO PPF
//   ... see #ifdef NOT_CONVERTED_YET
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#include <stdlib.h>
// --------------------------------------------------------
unsigned int $$_Array::maxCapacity=0;

// --------------------------------------------------------------
// Create array of pointers with the initial size s
// Returns: Returs the array, NULL when the allocation fails
// --------------------------------------------------------------
PTR($2) $$_Array::form(PTR($1) hp,unsigned int const sz,int const incr){
    $2 at; // temporary  $2 object

    if(hp->$0.array)free(hp);
    hp->$0.array.newArr(sz);
    if(hp->$0.array==NULL){
        printf("$$ error, cannot allocate array size=%d\n",sz);
        return NULL;
    }
    hp->$0.sz=sz;
    hp->$0.num=0;
    hp->$0.incr=incr;

    at=hp->$0.array[sz-1]; // in order to record the end-of-array on the file
    return hp->$0.array;
}

unsigned int $$_Array::size(PTR($1) hp) {return (unsigned int)(hp->$0.num);} 

$2 $$_Array::get(PTR($1) hp,const unsigned int k){  // a= &(array[k])
    if(k>=hp->$0.sz)sizeChange(hp,k+1,0);
    return hp->$0.array[k];
}

PTR($2) $$_Array::ind(PTR($1) hp,const unsigned int k){  // a=array[k]
    PTR($2) p; int nInd;
    if(k>=hp->$0.sz)sizeChange(hp,k+1,0);
    p=hp->$0.array;
    nInd=(int)p; // gives index of p
    nInd=nInd+(k-1)*sizeof($2);
    p=nInd; // sets index of p
    return p; 
}

void $$_Array::set(PTR($1) hp,const unsigned int k,PTR($2) ap){ // array[k]=(*a)
    int kk=k;
    if(kk>=hp->$0.sz)sizeChange(hp,kk+1,0);
    if(kk>=hp->$0.num)hp->$0.num=kk+1;
    hp->$0.array[kk]=(*ap);
}

void $$_Array::set(PTR($1) hp,const unsigned int k,$2 a){ // array[k]=(*a)
    int kk=k;
    if(kk>=hp->$0.sz)sizeChange(hp,kk+1,0);
    if(kk>=hp->$0.num)hp->$0.num=kk+1;
    hp->$0.array[kk]=a;
}

void $$_Array::free(PTR($1) hp){ 
    if(hp->$0.array){
        (hp->$0.array).delArr();
        hp->$0.array=NULL;
    }
    hp->$0.num=0;
    hp->$0.sz=0;
}

// --------------------------------------------------------------
// This function changes the array to new capacity, newCap.
// Parameter 'exact' determines the multiples in which the array should grow:
//  exact=0  frow using the increment, copy the old data in
//  exact=1  exactly to newSz, copy the old data in
//  exact=2  exactly to newSz, initialize the new array by 0
// When decreasing the size of the array, it is always decreased to
// exactly newSz.
// When the size is fixed, only a change for newSz to be exact is accepted.
//   This prevents erroneous automatic size change, but still allows to
// enforce the array size when intentional.
// Warning: When reducing the size, entries with index >=newSz are
//          removed from the array.
// Returns 0 if everything OK, returns 1 when problem.
// --------------------------------------------------------------
int $$_Array::sizeChange(PTR($1) hp,const unsigned int newCap,const int exact){
    PTR($2) a; PTR($2) arr;
    int nCap,j,s,sn,cap,num,incr;

    nCap=newCap;
    arr=hp->$0.array;
    cap=hp->$0.sz;
    num=hp->$0.num;
    incr=hp->$0.incr;

    if(incr==0 && exact==0){
    printf("error for $$: attempt to change a fixed array into flexible one\n");
    return 1;
    }
    if(!arr){
        printf("error for $$: cannot modify capacity, array not formed\n");
        return 1;
    }

    if(nCap==cap)return 0; // nothing to do
    if(maxCapacity>0 && nCap>maxCapacity){
        printf("error for $$: requested capacity=%d, maxCapacity=%d\n",
                                                 nCap,maxCapacity);
        return 1;
    }

    if(nCap<cap || exact>=1)s=cap;
    else { // increase the size using increment
        for(s=cap; s<nCap; s=sn){
            if(incr>0) sn=s+incr;
            else       sn=s*(-incr);
          
            if(maxCapacity>0 && sn>maxCapacity){s=maxCapacity; break;}
        }
    }

    a=new $2[s];
    if(!a){
        printf("error for $$: failed to allocate array capacity=%d\n",s);
        return 1;
    }

    if(s<=num)sn=s-1; else sn=num;
    if(exact<=1){ // copy old data into the new array
        for(j=0; j<=sn; j++)a[j]=arr[j];
    }
    else { // initialize the new array by zeros
        memset((void*)a,0,(size_t)(s*sizeof($2)));
    }
    delete[] arr;
    hp->$0.array=a;
    hp->$0.sz=s;
    if(num>=s)hp->$0.num=s-1;

    return 0;
}

unsigned int $$_Array::capacity(PTR($1) hp) {return hp->$0.sz;} 

// --------------------------------------------------------------
// The usual qsort() cannot be used for sorting PPF arrays, because
// PPF does not allow casting to void* or char*; all objects have 
// have their proper type.
// For this reason, I coded a special PPF generic version of Quicksort
// based on pivotal splitting, and using the algorithm described 
// in T.A.Budd's "Classical Data structures in C++", pp.344-345.
// Unfortunately this is a recursive code which represents some
// performance loss.
// $$_sortFun is the compare function, same as used for the standard
// qsort.
// Array arr should be split in the section from i1 to i2, including
// both ends.
// For subsets smaller for 4 elements or less, we use simple bubble sort.
// --------------------------------------------------------------
#define SWAP(I,J) {at=arr[I]; arr[I]=arr[J]; arr[J]=at;}
	

void $$_Array::split($$_sortFun cmp,PTR($1) hp,long i1, long i2){
    long i,k,low,high,n;
    static $2 at; // temporary object for exchanges
    PTR($2) arr=hp->$0.array;
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
int $$_Array::binSearch($$_sortFun cmpF,PTR($1) hp,$2 obj,int *match,int i1,int i2){
    int k,cmp,num; PTR($2) arr; long sz;
    static $2 at; // temporary (not persistent) object

    arr=hp->$0.array;
    num=hp->$0.num;
    if(i1<0)i1=0;
    if(i2>num-1)i2=num-1;
    if(arr==NULL || num<=0 || i2<i1){
        printf("Warning: array $$ not formed or empty, bypassing binSearch() \n");
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

int $$_Array::binSearch($$_sortFun cmpF,PTR($1) hp, $2 obj,int *match){
	return binSearch(cmpF,hp,obj,match,0,hp->$0.num-1); }


// sort the entire array
void $$_Array::sort($$_sortFun cmp,PTR($1) hp){
	split(cmp,hp,0,hp->$0.num -1);
}

// sort only the sections (including the endpoints
void $$_Array::sort($$_sortFun cmp,PTR($1) hp,long i1,long i2){
	split(cmp,hp,i1,i2);
}

int $$_Array::formed(PTR($1) hp){ if(hp->$0.array)return 1; return 0;}



// ===============================================================
#ifdef NOT_CONVERTED_YET // WARNING: check 'num' when converting
// It is the number of the valid entries, NOT the last index !!!
// ===============================================================

int $$_Array::increment(PTR($1) hp) {return hp->$0.incr;} 

// --------------------------------------------------------------
// Remove the entry from the array, and shrink the remaining part
// of the array. This changes 'num' and indexes of all entries
// above this index.
// See array.h for method remove() which is much faster but changes
// the order of the array.
// --------------------------------------------------------------
void $$_Array::extract(PTR($1) hp, const int k){
    int j,num;

    num=hp->$0.num;
    if(k>=num)return; // nothing to do
    if(k==num-1){hp->$0.num=num-1; return;}
    for(j=k+1; j<num; j++)hp->$0.array[j-1]=hp->$0.array[j];
    hp->$0.num=num-1;
}

// --------------------------------------------------------------
// Insert the entry into the array[k], and expand the remaining part
// of the array. This changes 'num' and indexes of all entries
// above this index. 
// --------------------------------------------------------------
void $$_Array::insert(PTR($1) hp,const int k,$2 *t){
    int j,kk,oldNum,newNum,sz;

    if(k<0)kk=0; else kk=k;
    oldNum=hp->$0.num;
    sz=hp->$0.sz;
    if(kk>oldNum) newNum=kk; else newNum=oldNum+1;
    if(newNum>=sz){
        (void)sizeChange(hp,newNum+1,0);
    }
    for(j=oldNum; j>=kk; j--) hp->$0.array[j+1]=hp->$0.array[j];
    hp->$0.array[kk]=(*t);
    hp->$0.num=newNum;
}

// --------------------------------------------------------------
// Return pointer to the element[i] of the array.
// WARNING: The pointer thus obtained must be used either immediately,
//          or when we are absolutely sure that the array will not 
//          automatically re-allocate, for example when working with
//          a fixed array (incr=0).
// --------------------------------------------------------------
PTR($2) $$_Array::ind(PTR($1) hp,int i){
    if(!(hp->$0.array)){
        printf("error for $$: index operation attempted before formed\n");
        return NULL;
    }
    if(i>=hp->$0.sz){
        if(sizeChange(hp,i+1,0))return NULL; // error message inside there
    }
    if(hp->$0.num<i)hp->$0.num=i;
    return &(hp->$0.array[i]);
}

// --------------------------------------------------------------
// Fast way of getting ind[0]
// --------------------------------------------------------------
PTR($2) $$_Array::head(PTR($1) hp){
    return ($2*)(hp->$0.array);
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
void $$_Array::inHeap($$_sortFun cmpF,PTR($1) hp,PTR($2) e,$$_callback callback){
    int i; PTR($2) newE;

    i=hp->$0.num + 1;
    newE=indHeap(hp,i,callback);
    if(!newE)return;
    (*newE)=(*e);
    if(callback) (*callback)(&(hp->$0.array[i]),i);
    if(i>1)upHeap(cmpF,hp,i,callback);
}

// --------------------------------------------------------------
// Assuming a new element in location n, re-sort the heap proceeding
// from n to all its ascendents.
// --------------------------------------------------------------
void $$_Array::upHeap($$_sortFun cmpF,PTR($1) hp,int n,$$_callback callback){
    int i1,i2; PTR($2) a1; PTR($2) a2;

    // aa is a temporary object used in exchanges, not persistent.
    // It must be automatically allocated, not from heap,
    // otherwise it adds data space for each run.
    $2 aa; 

    if(n==0)return;
    // traverse the ascendants
    for(i1=n, i2=(n-1)/2, a1=ind(hp,n); i1>0; i1=i2, i2=(i2-1)/2, a1=a2){
        a2=ind(hp,i2);
        if(cmpF(a2,a1)<=0)break;
        // exchange the contents of a1 and a2
        aa=(*a1); (*a1)=(*a2); (*a2)=aa;
        if(callback){
            (*callback)(&(hp->$0.array[i1]),i1);
            (*callback)(&(hp->$0.array[i2]),i2);
        }
    }
}

// --------------------------------------------------------------
// Return the top of the heap in e, and remove it from the heap.
// Return 1 when successful, return 0 when the heap is empty
// --------------------------------------------------------------
int $$_Array::outHeap($$_sortFun cmpF,PTR($1) hp,PTR($2) e,$$_callback callback){
    int num;

    num=hp->$0.num;
    if(num<0) return 0;
    (*e)=hp->$0.array[0];
    if(callback) (*callback)(&(hp->$0.array[0]),-1);
    if(num>0){
       hp->$0.array[0]=hp->$0.array[num];
       if(callback) (*callback)(&(hp->$0.array[0]),0);
       downHeap(cmpF,hp,0,callback);
    }
    (hp->$0.num)--;
    return 1;
}


// --------------------------------------------------------------
// Assuming a new element in location n, re-sort the heap proceeding
// from n to all its descendents.
// --------------------------------------------------------------
void $$_Array::downHeap($$_sortFun cmpF,PTR($1) hp,int n,$$_callback callback){
    int k,i1,i2,num; PTR($2) ak; PTR($2) a1; PTR($2) a2; 

    // aa is a temporary object used in exchanges, not persistent.
    // It must be automatically allocated, not from heap,
    // otherwise it adds data space for each run.
    $2 aa; 

    num=hp->$0.num;
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
            (*callback)(&(hp->$0.array[i1]),i1);
            (*callback)(&(hp->$0.array[k]),k);
        }
    }
}

// --------------------------------------------------------------
// Update heap when the value associated with entry n has changed
// --------------------------------------------------------------
void $$_Array::updHeap($$_sortFun cmpF,PTR($1) hp,int n,$$_callback callback){
    int num,n1,n2;

    num=hp->$0.num;
    if(n<0 || n>num){
        printf("error updating heap=$$, n=%d num=%d\n",n,num);
        return;
    }
    downHeap(cmpF,hp,n,callback);
    upHeap(cmpF,hp,n,callback);
}

// --------------------------------------------------------------
// Delete item index n from the heap.
// --------------------------------------------------------------
void $$_Array::delHeap($$_sortFun cmpF,PTR($1) hp,int n,$$_callback callback){
    int num; PTR($2) arr;

    num=hp->$0.num;
    arr=hp->$0.array;
    if(n<0 || n>num){
        printf("error deleting from heap=$$, n=%d num=%d\n",n,num);
        return;
    }
    (hp->$0.num)--;
    if(callback) (*callback)(&(hp->$0.array[num]),-1);
    if(n==num) return;
    arr[n]=arr[num];
    if(callback) (*callback)(&(hp->$0.array[n]),n);
    updHeap(cmpF,hp,n,callback);
}

// --------------------------------------------------------------
// Same function as ind() but providing a callback function.
// This function is needed when relocating heaps automatically.
// --------------------------------------------------------------
PTR($2) $$_Array::indHeap(PTR($1) hp,int i,$$_callback callback){
    int k,num,sz;
    PTR($2) elem;

    num=hp->$0.num;
    sz=hp->$0.sz;
    elem=ind(hp,i);
    if(!elem)return NULL;
    // report changes of position when re-allocating
    if(i>=sz){
        for(k=0; k<num; k++){
            if(callback) (*callback)(&(hp->$0.array[k]),k);
        }
    }
    return elem;
}
// --------------------------------------------------------------




void $$_Array::remove(PTR($1) hp,const unsigned int k){ // fast but order changed
    int kk=k;
    int num=hp->$0.num;
    if(kk<num){hp->$0.array[k]=hp->$0.array[num]; (hp->$0.num)--;}
}
int $$_Array::reduce(PTR($1) hp){             // reduce the array to its used size
    return sizeChange(hp,hp->$0.num+1,1);
}
int $$_Array::reduce(PTR($1) hp,const unsigned int newCap){ // reduce size to newSz
    if(newCap>=hp->$0.sz)return hp->$0.sz;
    return sizeChange(hp,newCap,1);
} 
int $$_Array::grow(PTR($1) hp,const unsigned int newCap){
    return sizeChange(hp,newCap,0);
} 
void $$_Array::push(PTR($1) hp,PTR($2) e){
    unsigned int num,sz;

    num=hp->$0.num+1; sz=hp->$0.sz;
    if(num>=sz)sizeChange(hp,num+1,0);
    hp->$0.array[num]=(*e); (hp->$0.num)++;
}

PTR($2) $$_Array::pop(PTR($1) hp){PTR($2) e=NULL; int num=hp->$0.num;
    if(num>=0){e= &(hp->$0.array[num]); (hp->$0.num)--;}
    return e;
}

void $$_Array::reset(PTR($1) hp,int newSz,int incr){
    hp->$0.num=newSz-1; hp->$0.incr=incr;
}

// -----------------------------------------------------------------------
// Add a new object into the ordered collection. This is relatively
// fast using binary search to find the position, but a section of
// the array must move to open one slot.
// -----------------------------------------------------------------------
void $$_Array::addOrd($$_sortFun cmpF,PTR($1) hp,PTR($2) op){
    int k,found;

    if(hp->$0.num<0)k=0;
    else k=binSearch(cmpF,hp,op,&found);
    insert(hp,k,op); // insert even if the same key is already there
}

// -----------------------------------------------------------------------
// Find the given object in the sorted array (using binary search0
// and remove it from the array without changing the order of the remaining
// elements.
// -----------------------------------------------------------------------
void $$_Array::delOrd($$_sortFun cmpF,PTR($1) hp,PTR($2) obj){
    int k,found;

    k=binSearch(cmpF,hp,obj,&found);
    if(found) delOrd(cmpF,hp,k);
    else printf("warning: calling $$::delOrd() but object not there\n");
}

// -----------------------------------------------------------------------
// Returns the value of the bin No.i (0 or 1).
// Returns -1 in case of error.
// -----------------------------------------------------------------------
int $$_Array::getBit(PTR($1) hp,int i){
    int iChar,iBit,sz,k,newBytes; PTR($2) ap; unsigned char *p,b;

    sz=sizeof($2) * hp->$0.sz; // sz is size in bytes
    if(i>=sz*8){ // sz*8 is the size in bits
        ap=ind(hp,i/(8*sizeof($2)));
        if(!ap){
            printf("$$ error: cannot realocate for %d bits\n",i);
            return -1;
        }

        // initialize the new part of the array to 0
        newBytes=sizeof($2)*hp->$0.sz;
        p=(unsigned char*)(hp->$0.array);
        for(k=sz; k<newBytes; k++) p[k]=0;
    }
    else if(hp->$0.num<0){ // first call, initialize all bits to 0
        p=(unsigned char*)(hp->$0.array);
        for(k=0; k<sz; k++) p[k]=0;
    }

    k=i/(8*sizeof($2)); // object index in which bit i is
    if(hp->$0.num < k)hp->$0.num=k;

    p=(unsigned char*)(hp->$0.array);
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
int $$_Array::setBit(PTR($1) hp,int i,int val){
    int iChar,iBit,sz,k,newBytes; PTR($2) ap; unsigned char *p,b;

    sz=sizeof($2) * hp->$0.sz; // sz is size in bytes
    if(i>=sz*8){ // sz*8 is the size in bits
        ap=ind(hp,i/(8*sizeof($2)));
        if(!ap){
            printf("$$ error: cannot realocate for %d bits\n",i);
            return -1;
        }

        // initialize the new part of the array to 0
        newBytes=sizeof($2)*hp->$0.sz;
        p=(unsigned char*)(hp->$0.array);
        for(k=sz; k<newBytes; k++) p[k]=0;
    }
    else if(hp->$0.num<0){ // first call, initialize all bits to 0
        p=(unsigned char*)(hp->$0.array);
        for(k=0; k<sz; k++) p[k]=0;
    }

    k=i/(8*sizeof($2)); // object index in which bit i is
    if(hp->$0.num < k)hp->$0.num=k;

    p=(unsigned char*)(hp->$0.array);
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
int $$_Array::getOrd($$_sortFun cmpF,PTR($1) hp,PTR($2) op,int *found){
    int k=binSearch(cmpF,hp,op,found);
    return k;
}

#endif // NOT_CONVERTED_YET


