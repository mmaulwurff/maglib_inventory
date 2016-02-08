#include "FixedInventory.h"
#include "InventoryItemPointerAdapter.h"
#include "PointerInventoryItem.h"
#include "DirectInventoryItem.h"

#include <cassert>
#include <cstdio>
#include <utility>

void passPuts(const char* const name) {
    std::printf("%s test passed.\n", name);
}

class ExampleClass { public:
    explicit ExampleClass(int setN) : n(setN) {}
    explicit ExampleClass(const ExampleClass& other) : n(other.n) {}
    ~ExampleClass() {}

    static int getMaxStackSize() { return 3; }
    bool receiveIntoInner() { return false; }

    int showN() const { return n; }

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

    inventoryItem.push(5);
    assert(inventoryItem.getCount() == 1);
    assert(inventoryItem.showContent() == 5);

    inventoryItem.push(5);
    assert(inventoryItem.getCount() == 1); // maximum stack size is 1.

    while (inventoryItem.getCount() != 0) {
        const int popped = inventoryItem.pop();
        assert(popped == 5);
        assert(inventoryItem.getCount() == 0);
    }

    passPuts("DirectInventoryItem");
},

[]() {
    PointerInventoryItem<ExampleClass> inventoryItem;
    for (int i = 1; i <= ExampleClass::getMaxStackSize(); ++i) { // fill
        MyClassItem item(new ExampleClass(1));
        inventoryItem.push(item);
        assert(inventoryItem.getCount() == i);
    }
    assert(inventoryItem.showContent()->showN() == 1);

    while (inventoryItem.getCount()) {
        MyClassItem popped(std::move(inventoryItem.pop()));
    }

    {
        MyClassItem item2(new ExampleClass(2));
        inventoryItem.push(item2);
    }

    {
        MyClassItem item(new ExampleClass(3));
        const bool receiveSuccess = inventoryItem.push(item);
        assert(not receiveSuccess);
        delete item.release(); // without release(), item will delete by itself.
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

