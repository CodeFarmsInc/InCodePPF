#ifndef ZZ_CODEGEN_INCLUDE
#define ZZ_CODEGEN_INCLUDE

#include "..\..\lib/general.h"

class B;
class C;

#include "tmp/BtoC.h"
#include "tmp/BtoC1.h"

// -----------------------------------------------
typedef class BtoC_LinkedList1 BtoC;
// -----------------------------------------------


class ZZ_B {
public:
    BtoC_LinkedList1Parent ZZBtoC;
};

class ZZ_C {
public:
    BtoC_LinkedList1Child ZZBtoC;
};

typedef BtoC_LinkedList1Iterator BtoC_Iterator;

#endif // ZZ_CODEGEN_INCLUDE
