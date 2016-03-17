#ifndef MAGLIB_INVENTORY_HAS_METHOD_H
#define MAGLIB_INVENTORY_HAS_METHOD_H

// http://stackoverflow.com/questions/87372/
// check-if-a-class-has-a-member-function-of-a-given-signature

#include <type_traits>

#define HAS_METHOD_INIT(method) \
namespace mag_detail { \
template <typename N, typename T> \
struct has_##method { \
    static_assert( std::integral_constant<T, false>::value \
                 , "Second template parameter needs to be of function type."); \
}; \
\
template <typename C, typename Ret, typename... Args> \
struct has_##method<C, Ret(Args...)> { \
private: \
    template <typename T> \
    static constexpr auto check(T*) -> typename std::is_same< \
        decltype(std::declval<T>().method(std::declval<Args>()...)), \
            Ret>::type; \
\
    template <typename> \
    static constexpr std::false_type check(...); \
\
public: \
    static constexpr bool value = decltype(check<C>(nullptr))::value; \
}; \
}

#define HAS_METHOD(type, method, method_type) \
mag_detail::has_##method<type, method_type>::value

#endif //MAGLIB_INVENTORY_HAS_METHOD_H
