#ifndef MAGLIB_INVENTORY_MAG_ASSERT_H
#define MAGLIB_INVENTORY_MAG_ASSERT_H

/** void mag_assert(int expression);
 *
 *  If NDEBUG is defined, do nothing.
 *  If not, and EXPRESSION is zero, throw exception with an error message.
 *  mag_assert is C assert with stack unwinding, it helps debugging.
 *  */

#ifdef NDEBUG

#define mag_assert(expr) static_cast<void>(0)

#else

#include <stdexcept>

#define TO_STRING(x) TO_STRING_DETAIL(x)
#define TO_STRING_DETAIL(x) #x
#define LINE_STRING TO_STRING(__LINE__)

#define mag_assert(expr) if (! (expr)) { \
    throw std::runtime_error(#expr " at " __FILE__ ":" LINE_STRING); }

#endif

#endif //MAGLIB_INVENTORY_MAG_ASSERT_H
