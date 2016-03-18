#ifndef MAGLIB_INVENTORY_MANAGED_POINTER_H
#define MAGLIB_INVENTORY_MANAGED_POINTER_H

#include <limits>

namespace mag {

template <typename pointed_type>
class managed_pointer {
public:

    explicit managed_pointer(pointed_type* const set) : pointer(set) {}

    managed_pointer() : pointer(nullptr) {}

    explicit managed_pointer(const managed_pointer& other)
        : pointer(new pointed_type(*other)) {}

    explicit managed_pointer(managed_pointer&& other)
        : pointer(other.release()) { }

    ~managed_pointer() { delete pointer; }

    pointed_type* release() const;

    bool operator==(const managed_pointer& other) const;
    bool operator!=(const managed_pointer& other) const;

    managed_pointer<pointed_type>& operator=(const managed_pointer& other);

    const pointed_type& operator* () const { return *pointer; }
    const pointed_type* operator->() const { return  pointer; }
    pointed_type* operator->() { return pointer; }

    const pointed_type* get() const { return pointer; }

    int get_max_stack_size() const;

private:

    mutable pointed_type* pointer;
};

template <typename pointed_type>
pointed_type* managed_pointer<pointed_type>::release() const {
    pointed_type* const ret = pointer;
    pointer = nullptr;
    return ret;
}

template <typename pointed_type>
managed_pointer<pointed_type>& managed_pointer<pointed_type>::operator=(
    const managed_pointer& other) {
    if (pointer != other.pointer) {
        delete pointer;
        pointer = other.pointer;
    }
    other.pointer = nullptr;
    return *this;
}

template <typename p>
bool managed_pointer<p>::operator==(const managed_pointer<p>& other) const {
    if (pointer == other.pointer) return true;
    if (pointer == nullptr || other.pointer == nullptr) return false;
    return (*pointer == *(other.pointer));
}

template <typename p>
bool managed_pointer<p>::operator!=(const managed_pointer<p>& other) const {
    return (not operator==(other));
}

template <typename pointed_type>
int managed_pointer<pointed_type>::get_max_stack_size() const {
    if (pointer == nullptr) return std::numeric_limits<int>::max();
    return pointer->get_max_stack_size();
}

} // namespace mag

template <typename P>
std::ostream& operator<<(std::ostream& str, const mag::managed_pointer<P>& p) {
    return (str << p.get());
}

#endif // MAGLIB_INVENTORY_MANAGED_POINTER_H
