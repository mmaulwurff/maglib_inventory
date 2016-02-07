#ifndef MAGLIB_INVENTORY_INVENTORY_H
#define MAGLIB_INVENTORY_INVENTORY_H

#include "DirectInventoryItem.h"

/** FixedInventory is a container for pointers to DirectInventoryItem.
 *
 */
/*template <int rows, int columns>
class FixedInventory {
public:

    FixedInventory();

    void show() const;

private:

    static int indexAt(int row, int column);

    static const int size = rows * columns;
    static const bool enableBoundsCheck = false;

    DirectInventoryItem* container[size];
};

template <int r, int c>
FixedInventory<r, c>::FixedInventory()
    : container{nullptr}
{}

template <int r, int c>
void FixedInventory<r, c>::show() const {
    for(const DirectInventoryItem* const item : container) {
        std::cout << item << std::endl;
    }
}*/

#endif //MAGLIB_INVENTORY_INVENTORY_H
