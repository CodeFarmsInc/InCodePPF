#include "environ.h"
#include "gen.h"
#include "employee.h"

// ------------------------------------------------------------
void Employee::prt(PTR(Employee) p){
    printf("   flag=%d name=%s ID=%d\n",
		    p->flg,(eName::get(p)).getPtr(),p->getID());
}
