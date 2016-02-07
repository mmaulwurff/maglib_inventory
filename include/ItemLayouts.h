#ifndef MAGLIB_INVENTORY_ITEMLAYOUTS_H
#define MAGLIB_INVENTORY_ITEMLAYOUTS_H

#include <iostream>

class ItemLayout {
public:
    constexpr explicit ItemLayout(int setLayout);

    int constexpr getLayout() const;

    ItemLayout constexpr rotatedRight() const;
    ItemLayout constexpr rotatedLeft() const;
    ItemLayout constexpr rotated180() const;

    ItemLayout constexpr flippedHorizontal() const;
    ItemLayout constexpr flippedVertical() const;

    ItemLayout constexpr movedRight() const;
    ItemLayout constexpr movedLeft() const;
    ItemLayout constexpr movedUp() const;
    ItemLayout constexpr movedDown() const;

    bool constexpr freeBottom() const;
    bool constexpr freeTop() const;
    bool constexpr freeLeft() const;
    bool constexpr freeRight() const;

    int constexpr countSlots() const;

private:

    int layout;

    static constexpr int getBit(int integer, int position);

    template<int (*)(int)>
    static constexpr int modifyBits(int layout);

    static constexpr int rotateRightIndices(int i);
    static constexpr int rotateLeftIndices(int i);
    static constexpr int rotate180Indices(int i);
    static constexpr int flipHorIndices(int i);
    static constexpr int flipVerIndices(int i);

};

namespace mag {
    constexpr int makeFigure(const char* str);
}

std::ostream& operator<<(std::ostream& stream, const ItemLayout& layout) {
    int l = layout.getLayout();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            stream << ((l & 0x8000) ? '#' : '.');
            l <<= 1;
        }
        stream << std::endl;
    }
    return stream;
}

#define IL constexpr ItemLayout

IL::ItemLayout(const int setLayout)
    : layout(setLayout)
{}

int IL::flipVerIndices    (const int i) { return  3 + (i / 4) * 4 - i % 4; }
int IL::flipHorIndices    (const int i) { return 12 - (i / 4) * 4 + i % 4; }
int IL::rotateLeftIndices (const int i) { return 12 - (i % 4) * 4 + i / 4; }
int IL::rotateRightIndices(const int i) { return  3 + (i % 4) * 4 - i / 4; }
int IL::rotate180Indices  (const int i) { return i; }

int IL::getLayout() const { return layout; }

template <int (*func)(int)>
int IL::modifyBits(const int layout) {
    int rotated = 0;
    for (int i = 0; i < 16; ++i) {
        rotated <<= 1;
        rotated |= getBit(layout, func(i));
    }
    return rotated;
}

int IL::getBit(const int integer, const int position)
    { return (integer >> position) & 1; }

bool IL::freeLeft()   const { return (layout & 0x8888) == 0; }
bool IL::freeRight()  const { return (layout & 0x1111) == 0; }
bool IL::freeTop()    const { return (layout & 0xf000) == 0; }
bool IL::freeBottom() const { return (layout & 0x000f) == 0; }

constexpr int mag::makeFigure(const char* const str) {
    return ((str[0] == '\0') ?
            0 :
            (makeFigure(str + 1) >> 1) | ((str[0] == '#') * 0x8000));
}

ItemLayout IL::movedDown()  const { return ItemLayout(layout / 16); }
ItemLayout IL::movedUp()    const { return ItemLayout(layout * 16); }
ItemLayout IL::movedLeft()  const { return ItemLayout(layout *  2); }
ItemLayout IL::movedRight() const { return ItemLayout(layout /  2); }

#define Modification(name, function) ItemLayout IL::name() const \
{ return ItemLayout(modifyBits<function>(layout)); }

Modification(flippedVertical,   flipVerIndices)
Modification(flippedHorizontal, flipHorIndices)
Modification(rotated180,        rotate180Indices)
Modification(rotatedLeft,       rotateLeftIndices)
Modification(rotatedRight,      rotateRightIndices)

int IL::countSlots() const {
    int layout = getLayout(), sum = 0;
    for (int i = 0; i < 16; ++i) {
        sum += layout & 1;
        layout >>= 1;
    }
    return sum;
}

#undef IL

enum Tetris {
    O = mag::makeFigure(
        "##.."
        "##.."),

    I = mag::makeFigure(
        "####"),

    T = mag::makeFigure(
        "###."
        ".#.."),

    L = mag::makeFigure(
        "#..."
        "#..."
        "##.."),

    J = mag::makeFigure(
        ".#.."
        ".#.."
        "##.."),

    S = mag::makeFigure(
        ".##."
        "##.."),

    Z = mag::makeFigure(
        "##.."
        ".##."),
};

#endif //MAGLIB_INVENTORY_ITEMLAYOUTS_H
