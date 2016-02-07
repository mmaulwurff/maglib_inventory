#include "FixedInventory.h"
#include "InventoryItemPointerAdapter.h"
#include "PointerInventoryItem.h"
#include "DirectInventoryItem.h"

#include <cassert>
#include <cstdio>

void passPuts(const char* const name) {
    std::printf("%s test passed.\n", name);
}

class ExampleClass { public:
    explicit ExampleClass(int setN) : n(setN) {}
    ~ExampleClass() { std::printf("ExampleClass destruction, n = %d\n", n); }

    static int getMaxStackSize() { return 3; }
    bool receiveIntoInner() { return false; }

    bool operator==(const ExampleClass & other) const { return n == other.n; }

private: int n;
};

typedef InventoryItemPointerAdapter<ExampleClass> MyClassItem;

typedef void (*TestFunction)();
const TestFunction tests[] = {
[]() {
    DirectInventoryItem<int> inventoryItem;
    assert(inventoryItem.getMaxStackSize() == 1);
    assert(inventoryItem.getCount() == 0);

    inventoryItem.receiveInto(5);
    assert(inventoryItem.getCount() == 1);
    assert(inventoryItem.showContent() == 5);

    inventoryItem.receiveInto(5);
    assert(inventoryItem.getCount() == 1); // maximum stack size is 1.

    passPuts("DirectInventoryItem");
},

[]() {
    PointerInventoryItem<ExampleClass> inventoryItem;
    for (int i = 0; i < ExampleClass::getMaxStackSize(); ++i) {
        MyClassItem item(new ExampleClass(1));
        inventoryItem.receiveInto(item);
        assert(inventoryItem.getCount() == i + 1);
    }

    {
        MyClassItem item(new ExampleClass(2));
        const bool receiveSuccess = inventoryItem.receiveInto(item);
        assert(not receiveSuccess);
        assert(inventoryItem.getCount() == ExampleClass::getMaxStackSize());
        delete item.release();
    }
    passPuts("DirectInventoryItem with pointer");
},

};

int main() {
    for (TestFunction test : tests) {
        test();
    }

    return 0;
}

