// ************************************************************************
//                       Code Farms Inc.
//          7214 Jock Trail, Richmond, Ontario, Canada, K0A 2Z0
//          tel 613-838-3622           htpp://www.codefarms.com
// 
//                     Copyright (C) 2008
//        This software is subject to copyright protection under
//        the laws of Canada, United States, and other countries.
// ************************************************************************

#ifndef VECTOR_DEFINED
#define VECTOR_DEFINED

#include "..\factory.h"

// -----------------------------------------------------------
// Generic version of traditional Vector, with interface similar to STL
// This version may not be safe under all circumstances. For example get() 
// returns directly the object, not PersistPtr, so its life span of its
// memory address can be very short.
// Also, this is a LIGHT version which uses PersistPtr<>
// Parametrization with PTR(..) is not possible, because the VS2010
// compiler first expands template and only after that it applies macros.
// -----------------------------------------------------------
template<class T> class vector {
    static int defaultSz; // default capacity
    int sz; // allocated size of the array
    int num; // number of entires used, must be < sz
    PersistPtr<T> arr; // array of T objects
public:
    vector(){sz=defaultSz; arr.newArr(sz); num=0;}
    vector(int n){sz=n; arr.newArr(sz); num=0;}
    // ~vector(); should not be used, PPF takes care of destroying arr

    // ----------------------------------------------------------------------
    T& get(int i){ // index i=0,1,2,...7
       if(i>=num){
           cout << "error accessing index=" << i <<
              " using max.index=" << num-1 << "instead\n"; cout.flush();
           i=num-1;
       }
       return arr[i];
    }
    // ----------------------------------------------------------------------
    void set(int i,T* val){ // index i=0,1,2,...
        PersistPtr<T> nArr,oArr; int k;

        if(i>=sz) resize(i+1); 
        arr[i]=(*val);
        if(i>=num)num=i+1;
    }
    // ----------------------------------------------------------------------
    int capacity(){return sz;} // currently allocated size
    int size(){return num;} // true size of the vector, number of entries
    // ----------------------------------------------------------------------
    // The following functions normal return is 0, return > 0 indicates error
    // All operations are safe to perform on the same vector.
    // insert and append do not change the source vector v.
    // ----------------------------------------------------------------------
    int insert(T t,int i){ // insert given object T before index i
        if(moveTail(0,1)) return 1
        arr[0]=t;
        return 0;
    }
    int insert(vector v){ // insert entire v at the beginning of this vector
        if(moveTail(0,v.num)) return 1
        return copyVect(v,0,v.num-1,0);
    }
    int insert(vector v,int i){ // insert entire vector v before index i
        if(moveTail(i,i+v.num)) return 1
        return insert(v,0,v.num-1,i);
    }
    int insert(vector v,int k1,int k2,int i){ // insert section of v before i
        if(moveTail(i,i+k2-k1+1)) return 1;
        if(copyVect(v,k1,k2,i)) retun 1;
        return 0;
    }
    int append(T t,int i){ // append given object T after index i
        if(moveTail(i+1,i+2)) return 1;
        arr[i+1]=t;
        return 0;
    }
    int append(vector v){ // append v at end of this vector
        return append(vector v,0,v.num-1,num-1);
    }
    int append(vector v,int i){ // append vector v after index i
        return append(v,0,v.num-1,i);
    }
    int append(vector v,int k1,int k2,int i){ // append section of v after i
        if(moveTail(i+1,i+k2-k1+2)) return 1
        if(copyVect(v,k1,k2,i+1))return 1;
        return 0;
    }
    int remove(int i){ // remove entry i
        return moveTail(i+1,i-1);
    }
    int remove(){ // remove the netire vector
        num=0;
        return 0;
    }
    int remove(int i1,int i2){ // remove entries i1,...,i2
        if(i2>=num-1){num=i1; return 0;}
        return moveTail(i2+1,i1-1);
    }
    int reduceSize(int n){  // reduce size of the vector to n (cut off the tail)
        if(n<0 || n>num)return 1;
        num=n;
    }
    void reduceCap(){ // reduce capacity to the existing size of the vector
        reduceCap(num);
    }
    void reduceCap(int n){ // when n<num, size is also reduced
        resize(n);
    }
    // ----------------------------------------------------------------------
    // Copy section of vector v from...to to this vector starting at start,
    // and overwrite whatever is there
    // The function is smart enough to handle even the case when v==this
    // ----------------------------------------------------------------------
    int copyVect(vector v,int from,int to,int start){ 
        int x,n;

        if(from>to)return 1;
        x=n=start+to-from+1;
        if(x>sz){
            if(x<2*sz)x=2*sz;
            resize(x);
        }

        if(start<from)for(i=from; i<=to; i++) arr[start+i-from]=v.arr[i];
        else          for(i=to; i>=from; i--) arr[start+i-from]=v.arr[i];

        if(n>num)num=n;
        return 0;
    }
    // ----------------------------------------------------------------------

private:
    // ----------------------------------------------------------------------
    int moveTail(int from,int to){ // from<to, from>to, from=num are allowed
        int i,x;

        if(from==to)return 0; // nothing to do
        if(from<0 || from>num)return 1;

        if(from==num){ // no tail, possibly just modify num and sz
            x=to;
            if(to>from){
                if(x>sz){
                    if(x<2*sz)x=2*sz;
                    resize(x);
                }
            }
            num=to-1;
            return 0;
        }
                
        if(from<to){
            x=num+to-from;
            if(x>sz){
                if(x<2*sz)x=2*sz;
                resize(x);
            }
            x=to-from;
            for(i=num-1; i>=from; i--) arr[i+x]=arr[i];
            num=num+x;
        }
        else { // from>to
            if(to<0 || from>num)return 1;
            x=from-to;
            for(i=from; i<num; i++) arr[i-x]=arr[i];
            num=num-x;
        }
        return 0;
    }
        
    // ----------------------------------------------------------------------
    // When newSz<num, num is also reduced (cutting off the tail)
    // When newSz<defaultSz, the vector is reduced to its minimal (default)size
    // ----------------------------------------------------------------------
    void resize(int newSz){
        PersistPtr<T> nArr,oArr; int k;

        if(newSz==sz)return;
        if(newSz<defaultSz)newSz=defaultSz;
        if(newSz<num)num=newSz;
        nArr.newArr(newSz);
        for(k=0; k<num; k++)nArr[k]=arr[k];
        oArr=arr;
        arr=nArr;
        oArr.delArr();
        sz=newSz;
    }
    // ----------------------------------------------------------------------
};

template<class T> int vector<T>::defaultSz=8;
#endif VECTOR_DEFINED
