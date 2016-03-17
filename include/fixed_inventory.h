#ifndef MAGLIB_INVENTORY_INVENTORY_H
#define MAGLIB_INVENTORY_INVENTORY_H

#include <ostream>

/*
template <typename CellType, int rows, int columns, bool boundsCheck = false>
class FixedInventory {
public:

    FixedInventory()
        : container()
    {}

    const CellType& showAt(c_int row, c_int col) const
        { return container[indexAt(row, col)]; }

    bool push(c_int row, c_int col, const CellType& object)
        { showAt(row, col).push(object); }

private:

    static int indexAt(c_int row, c_int col) { return row * columns + col; }

    CellType& showAt(c_int row, c_int col)
        { return container[indexAt(row, col)]; }

    static const int size = rows * columns;

    CellType container[size];
};

template <typename Ct, int rows, int columns, bool b>
std::ostream& operator<<( std::ostream&stream
                        , const FixedInventory<Ct, rows, columns, b>& inventory)
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            stream << inventory.showAt(i, j) << '\t';
        }
        stream << std::endl;
    }
    return stream;
}
*/
#endif //MAGLIB_INVENTORY_INVENTORY_H
