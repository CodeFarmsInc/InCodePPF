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
#include PPF_PATH
// #include "../../ppf/factory.h"

class Faculty {
    PersistClass(Faculty);
public:
    ZZ_Faculty ZZds;
};

class Student {
    PersistClass(Student);
public:
    ZZ_Student ZZds;
    int ID;
    Student(int i){ID=i;}
    Student(){}
};

class Course {
    PersistClass(Course);
public:
    ZZ_Course ZZds;
    int code;
    Course(int c){code=c;}
    Course(){}
};

class Takes {
    PersistClass(Takes);
public:
    ZZ_Takes ZZds;
    int mark;
    Takes(int m){mark=m;}
    Takes(){}
};


// ------------------------------------------------------------
int main(int argc,char **argv){
    PTR(Faculty) fp; PTR(Student) sp; PTR(Course) cp; PTR(Takes) tp;
    PTR(Student) sp1,sp2,sp3;
    int run; // 1 for the first run, 2 for the second run
    int i;
    students_Iterator sit;
    courses_Iterator cit;
    takes_Iterator tit;

    if(argc!=2)run=0;
    else if(!strcmp("1",argv[1]))run=1;
    else if(!strcmp("2",argv[1]))run=2;
    else run=0;

    if(run==0){
        cout<<"SYNTAX:\n"; 
	cout<<"   mton 1 ... creates data, tests it, and stores it on disk\n";
	cout<<"   mton 2 ... reads data from disk and checks it\n";
        return 2;
    }
    

    if(run==1){
        system("del *.ppf");
    }

    Faculty::startPager(24,2,100,0);
    Student::startPager(32,200,900,0);
    Course::startPager(32,200,900,0);
    Takes::startPager(32,200,900,0);
    PersistStart;
    // NOTE: pageSz for PersistString must be at least Company::txtBufSz


    if(run==1){
        fp=new Faculty(); 
        fp.setRoot();

        // generate 5 courses and 3 students
        // assign each student to 3 courses with some mark
	sp1= new Student(111); students::add(fp,sp1);
	sp2= new Student(222); students::add(fp,sp2);
	sp3= new Student(333); students::add(fp,sp3);

        cp=new Course(101); courses::add(fp,cp);
	    tp=new Takes(2); takes::add(tp,sp1,cp);
	    tp=new Takes(3); takes::add(tp,sp2,cp);
	    tp=new Takes(5); takes::add(tp,sp3,cp);
        cp=new Course(102); courses::add(fp,cp);
	    tp=new Takes(1); takes::add(tp,sp1,cp);
        cp=new Course(103); courses::add(fp,cp);
	    tp=new Takes(3); takes::add(tp,sp2,cp);
        cp=new Course(104); courses::add(fp,cp);
	    tp=new Takes(4); takes::add(tp,sp3,cp);
        cp=new Course(105); courses::add(fp,cp);
	    tp=new Takes(2); takes::add(tp,sp2,cp);
	    tp=new Takes(2); takes::add(tp,sp3,cp);
    }
    else { // run==2
	fp.getRoot();
	if(fp==NULL){
	    cout<<"WARNING: the root is NULL, no disk data\n";
	    return 1;
	}
    }
    
    // print students and all their courses:
    sit.start(fp);
    ITERATE(sit,sp){
	printf("student=%d\n",sp->ID);
        for(tp=tit.from1(sp); tp; tp=tit.next1()){
	    cp=takes::entity2(tp);
	    printf("    course=%d mark=%d\n",cp->code,tp->mark);
	}
    }
    // print courses and their participating students
    cit.start(fp);
    ITERATE(cit,cp){
	printf("course=%d\n",cp->code);
        for(tp=tit.from2(cp); tp; tp=tit.next2()){
	    sp=takes::entity1(tp);
	    printf("    student=%d mark=%d\n",sp->ID,tp->mark);
	}
    }
    
    Faculty::closePager();
    Student::closePager();
    Course::closePager();
    Takes::closePager();
    PersistClose;

    return 0;
}

PersistImplement(Faculty);
PersistImplement(Student);
PersistImplement(Course);
PersistImplement(Takes);

#include "gen.cpp"
#include "..\..\ppf\pointer.cpp"

