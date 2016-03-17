#ifndef MAGLIB_INVENTORY_INVENTORY_ITEM_POINTER_ADAPTER_H
#define MAGLIB_INVENTORY_INVENTORY_ITEM_POINTER_ADAPTER_H

#include <limits>

template <typename pointed_type>
class inv_pointer_adapter {
public:

    typedef inv_pointer_adapter<pointed_type> this_type;

    explicit inv_pointer_adapter(pointed_type* const setPointer)
        : pointer(setPointer) {}
    inv_pointer_adapter() : pointer(nullptr) {}

    explicit inv_pointer_adapter(const this_type& other)
        : pointer(other.release()) {}

    ~inv_pointer_adapter() { delete pointer; }

    pointed_type* release() const;

    bool operator==(const this_type& other) const;
    void operator= (const this_type& other);

    bool operator!=(const this_type& other) const
        { return (not operator==(other)); }

    const pointed_type& operator*() const { return *pointer; }
    const pointed_type* operator->() const { return pointer; }
    pointed_type* operator->() { return pointer; }

    const pointed_type* get() const { return pointer; }

    int get_max_stack_size() const;

private:

    mutable pointed_type* pointer ;
};

template <typename pointed_type>
pointed_type* inv_pointer_adapter<pointed_type>::release() const {
    pointed_type* const ret = pointer;
    pointer = nullptr;
    return ret;
}

template <typename pointed_type>
void inv_pointer_adapter<pointed_type>::operator=(
    const inv_pointer_adapter & other)
{
    if (pointer != other.pointer) {
        delete pointer;
        pointer = other.pointer;
    }
    other.pointer = nullptr;
}

template <typename pointed_type>
bool inv_pointer_adapter<pointed_type>::operator==(
    const inv_pointer_adapter<pointed_type> &other) const
{
    if (pointer == other.pointer) return true;
    if (pointer == nullptr || other.pointer == nullptr) return false;
    return (*pointer == *(other.pointer));
}

template <typename pointed_type>
int inv_pointer_adapter<pointed_type>::get_max_stack_size() const {
    if (pointer == nullptr) return std::numeric_limits<int>::max();
    return pointer->get_max_stack_size();
}

#endif // MAGLIB_INVENTORY_INVENTORY_ITEM_POINTER_ADAPTER_H
