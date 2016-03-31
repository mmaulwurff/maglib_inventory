#ifndef MAGLIB_INVENTORY_MANAGED_POINTER_H
#define MAGLIB_INVENTORY_MANAGED_POINTER_H

#include <limits>

namespace mag {

template <typename T>
struct default_copier {
    T* operator()(const T& origin) {
        return new T(origin);
    }
};

template <typename pointed_type, typename copier = default_copier<pointed_type>>
class managed_pointer {
public:

    managed_pointer();
    managed_pointer(pointed_type* set);

    explicit managed_pointer(const managed_pointer& other);
    explicit managed_pointer(managed_pointer&& other);

    ~managed_pointer();

    pointed_type* release() const;

    bool operator==(const managed_pointer& other) const;
    bool operator!=(const managed_pointer& other) const;
    bool operator< (const managed_pointer& other) const;

    managed_pointer& operator=(const managed_pointer& other);

    const pointed_type& operator* () const;
    const pointed_type* operator->() const;
    pointed_type* operator->();

    const pointed_type* get() const;

    int get_max_stack_size() const;

    static const int null_max_stack_size = std::numeric_limits<int>::max();

private:

    mutable pointed_type* pointer;
};

template <typename p, typename c>
p* managed_pointer<p, c>::release() const {
    p* const ret = pointer;
    pointer = nullptr;
    return ret;
}

#define T template <typename p, typename c>
#define C managed_pointer<p, c>

T C::managed_pointer(p* const set)
    : pointer(set)
{}

T C::managed_pointer()
    : pointer(nullptr)
{}

T C::managed_pointer(const managed_pointer& other)
    : pointer(c()(*other))
{}

T C::managed_pointer(managed_pointer&& other)
    : pointer(other.release())
{}

T C::~managed_pointer() {
    delete pointer;
}

T C& C::operator=(const managed_pointer& other) {
    if (pointer != other.pointer) {
        delete pointer;
        pointer = other.pointer;
    }
    other.pointer = nullptr;
    return *this;
}

T const p& C::operator* () const {
    return *pointer;
}

T const p* C::get()        const { return pointer; }
T const p* C::operator->() const { return pointer; }
T       p* C::operator->()       { return pointer; }

T bool C::operator==(const managed_pointer& o) const {
    if (pointer == o.pointer) return true;
    if (pointer == nullptr || o.pointer == nullptr) return false;
    return (*pointer == *(o.pointer));
}

T bool C::operator!=(const managed_pointer& other) const {
    return (not operator==(other));
}

T bool C::operator<(const managed_pointer& o) const {
    if (pointer == o.pointer) return true;
    if (pointer == nullptr || o.pointer == nullptr) return false;
    return (*pointer < *(o.pointer));
}

T int C::get_max_stack_size() const {
    if (pointer == nullptr) return null_max_stack_size;
    return pointer->get_max_stack_size();
}

#undef T
#undef C

} // namespace mag;

template <typename P, typename c>
std::ostream&
operator<<(std::ostream& s, const mag::managed_pointer<P, c>& p) {
    s << p.get();
    if (p.get()) s << "(" << *(p.get()) << ")";
    return s;
}

#endif // MAGLIB_INVENTORY_MANAGED_POINTER_H
