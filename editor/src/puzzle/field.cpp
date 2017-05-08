#include "puzzle/field.h"

Field::Field(int width, int height, QObject *parent) : QObject(parent), width(width), height(height) {
    field = new int[width * height]{0};
}

Field::~Field() {
    delete[] field;
}

const QPoint& Field::getPrevSetPos() const {
    return prev_set_pos;
}

int Field::getData(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return -1;
    return field[y * width + x];
}

int Field::getData(const QPoint &pos) const {
    if (pos.x() < 0 || pos.x() >= width || pos.y() < 0 || pos.y() >= height) return -1;
    return field[pos.y() * width + pos.x()];
}

void Field::setData(int x, int y, int value) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    field[y * width + x] = value;
    prev_set_pos = QPoint(x, y);
}

void Field::setData(const QPoint &pos, int value) {
    if (pos.x() < 0 || pos.x() >= width || pos.y() < 0 || pos.y() >= height) return;
    field[pos.y() * width + pos.x()] = value;
    prev_set_pos = pos;
}
