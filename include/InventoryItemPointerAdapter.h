#ifndef MAGLIB_INVENTORY_INVENTORY_ITEM_POINTER_ADAPTER_H
#define MAGLIB_INVENTORY_INVENTORY_ITEM_POINTER_ADAPTER_H

template <typename PointedType>
class InventoryItemPointerAdapter {
public:

    InventoryItemPointerAdapter() : pointer(nullptr) {}
    explicit InventoryItemPointerAdapter(PointedType* const setPointer)
        : pointer(setPointer) {}
    InventoryItemPointerAdapter(const InventoryItemPointerAdapter<PointedType>& other)
        : pointer(new PointedType(*other.pointer)) {}
    InventoryItemPointerAdapter(InventoryItemPointerAdapter<PointedType>&& other)
        : pointer(other.release()) {}

    ~InventoryItemPointerAdapter() { delete pointer; }

    PointedType* release();

    bool operator==(const InventoryItemPointerAdapter<PointedType>& other) const;
    void operator= (const InventoryItemPointerAdapter<PointedType>& other);

    const PointedType& operator*() const { return *pointer; }
    const PointedType* operator->() const { return pointer; }
    PointedType* operator->() { return pointer; }

private:

    mutable PointedType* pointer ;
};

template <typename PointedType>
PointedType* InventoryItemPointerAdapter<PointedType>::release() {
    PointedType* const ret = pointer;
    pointer = nullptr;
    return ret;
}

template <typename PointedType>
void InventoryItemPointerAdapter<PointedType>::operator=(
    const InventoryItemPointerAdapter& other)
{
    if (pointer != other.pointer) {
        delete pointer;
        pointer = other.pointer;
        other.pointer = nullptr;
    }
}

template <typename PointedType>
bool InventoryItemPointerAdapter<PointedType>::operator==(
    const InventoryItemPointerAdapter<PointedType> &other) const
{
    if (pointer == other.pointer) return true;
    if (pointer == nullptr || other.pointer == nullptr) return false;
    return (*pointer == *(other.pointer));
}


#endif // MAGLIB_INVENTORY_INVENTORY_ITEM_POINTER_ADAPTER_H
