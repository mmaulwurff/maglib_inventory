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

    managed_pointer(pointed_type* const set) : pointer(set) {}

    managed_pointer() : pointer(nullptr) {}

    explicit managed_pointer(const managed_pointer& other)
        : pointer(copier()(*other)) {}

    explicit managed_pointer(managed_pointer&& other)
        : pointer(other.release()) { }

    ~managed_pointer() { delete pointer; }

    pointed_type* release() const;

    bool operator==(const managed_pointer& other) const;
    bool operator!=(const managed_pointer& other) const;
    bool operator< (const managed_pointer& other) const;

    managed_pointer& operator=(const managed_pointer& other);

    const pointed_type& operator* () const { return *pointer; }
    const pointed_type* operator->() const { return  pointer; }
    pointed_type* operator->() { return pointer; }

    const pointed_type* get() const { return pointer; }

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

template <typename p, typename c>
managed_pointer<p, c>&
managed_pointer<p, c>::operator=(const managed_pointer<p, c>& other) {
    if (pointer != other.pointer) {
        delete pointer;
        pointer = other.pointer;
    }
    other.pointer = nullptr;
    return *this;
}

template <typename p, typename c>
bool managed_pointer<p, c>::operator==(const managed_pointer<p, c>& o) const {
    if (pointer == o.pointer) return true;
    if (pointer == nullptr || o.pointer == nullptr) return false;
    return (*pointer == *(o.pointer));
}

template <typename p, typename c>
bool
managed_pointer<p, c>::operator!=(const managed_pointer<p, c>& other) const {
    return (not operator==(other));
}

template <typename p, typename c>
bool managed_pointer<p, c>::operator<(const managed_pointer<p, c>& o) const {
    if (pointer == o.pointer) return true;
    if (pointer == nullptr || o.pointer == nullptr) return false;
    return (*pointer < *(o.pointer));
}

template <typename p, typename c>
int managed_pointer<p, c>::get_max_stack_size() const {
    if (pointer == nullptr) return null_max_stack_size;
    return pointer->get_max_stack_size();
}

} // namespace mag

template <typename P, typename c>
std::ostream&
operator<<(std::ostream& s, const mag::managed_pointer<P, c>& p) {
    s << p.get();
    if (p.get()) s << "(" << *(p.get()) << ")";
    return s;
}

#endif // MAGLIB_INVENTORY_MANAGED_POINTER_H
