#include <iostream>
using namespace std;
#include "gen.h"

class A;
class B;
class C;
#include "b.h"

// This function has been expanded to print also all the associated G's

void B::prt(void){
    cout << "  B=" << bb << "\n";
};
