#ifndef MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H
#define MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H

#include <cassert>

template <typename ContentType, class Derived>
class InventoryItemInterface {
public:

    /** Shows how much of this item can fit in a stack (one cell in inventory).
     * @returns maximum number of items in stack. */
    int getMaxStackSize() const
        { return static_cast<const Derived*>(this)->getMaxStackSizeInner(); }

    bool push(const ContentType& receivedContent);

    int getCount() const { return count; }

    const ContentType& showContent() const { return content; }

    ContentType pop();

    typedef ContentType content_type;

protected:

    int count = 0;
    ContentType content = ContentType();
};

template <typename ContentType, typename Derived>
bool InventoryItemInterface<ContentType, Derived>::push(const ContentType& newContent) {
    if (static_cast<Derived*>(this)->receiveIntoInner(newContent)) return true;

    if ((count == 0) || (count < getMaxStackSize() && newContent == content)) {
        content = newContent;
        ++count;
        return true;
    } else {
        return false;
    }
}

template <typename ContentType, typename Derived>
ContentType InventoryItemInterface<ContentType, Derived>::pop() {
    --count;
    assert(count >= 0);
    return ContentType(content);
}

#endif //MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H
