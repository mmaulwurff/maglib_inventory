#ifndef MAGLIB_INVENTORY_POINTER_INVENTORY_ITEM_H
#define MAGLIB_INVENTORY_POINTER_INVENTORY_ITEM_H

#include "InventoryItemInterface.h"
#include "InventoryItemPointerAdapter.h"

template <typename PointedType>
class PointerInventoryItem
    : public InventoryItemInterface< InventoryItemPointerAdapter<PointedType>
                                   , PointerInventoryItem<PointedType> >
{
public:

    int getMaxStackSizeInner() const;

    bool receiveIntoInner(const InventoryItemPointerAdapter<PointedType>&);

};

template <typename PointedType>
bool PointerInventoryItem<PointedType>::receiveIntoInner(
    const InventoryItemPointerAdapter<PointedType> &)
{
    return InventoryItemInterface< InventoryItemPointerAdapter<PointedType>
                                 , PointerInventoryItem<PointedType>
                                 >::content->receiveIntoInner();
}

template <typename PointedType>
int PointerInventoryItem<PointedType>::getMaxStackSizeInner() const {
    return InventoryItemInterface< InventoryItemPointerAdapter<PointedType>
                                 , PointerInventoryItem<PointedType>
                                 >::content->getMaxStackSize();
}

#endif // MAGLIB_INVENTORY_POINTER_INVENTORY_ITEM_H
