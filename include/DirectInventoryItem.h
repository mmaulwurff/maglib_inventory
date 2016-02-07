#ifndef MAGLIB_DIRECT_INVENTORY_INVENTORYITEM_H
#define MAGLIB_DIRECT_INVENTORY_INVENTORYITEM_H

#include "InventoryItemInterface.h"

template <typename ContentType>
class DirectInventoryItem
    : public InventoryItemInterface< ContentType, DirectInventoryItem<ContentType> >
{
public:

    int getMaxStackSizeInner() const { return 1; }

    bool receiveIntoInner(const ContentType&) { return false; }

};

#endif // DIRECT_MAGLIB_INVENTORY_INVENTORYITEM_H
