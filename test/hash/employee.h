#include "environ.h"
#include "..\..\ppf\factory.h"

class Employee {
PersistClass(Employee);
    int ID;
public:
    ZZ_Employee ZZds;
    int flg;
    Employee(){ID=0;} // for temporary objects, has no name
    PTR(Employee) static newEmployee(char *name,int id){
	PTR(Employee) ep=new Employee(); 
	ep->ID=id; 
	eName::addNew(ep,name);
        return ep;
    }
    ~Employee(){} // only for automatically allocated objects, disregard eName
    int getID(){return ID;}
    void setID(int id){ID=id;}
    void static prt(PTR(Employee) p);
};
