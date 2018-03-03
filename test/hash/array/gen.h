#ifndef ZZ_CODEGEN_INCLUDE
#define ZZ_CODEGEN_INCLUDE

#include "..\..\lib/general.h"

class B;
class A;

#include "tmp/arr.h"

// -----------------------------------------------
typedef class arr_Array arr;
// -----------------------------------------------


class ZZ_B {
public:
    arr_ArrayHolder ZZarr;
};
#define ZZ_A static int


#endif // ZZ_CODEGEN_INCLUDE
