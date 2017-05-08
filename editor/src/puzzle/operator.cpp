#include "puzzle/field.h"
#include "puzzle/piece.h"
#include "puzzle/operator.h"

QMap<QString, Field*> Operator::field_pool = QMap<QString, Field*>();

Operator::Operator(QObject *parent) : QObject(parent), selected_field(0) {}

Operator::~Operator() {}

Field* Operator::createField(int width, int height) {
    return new Field(width, height);
}

void Operator::addFieldToPool(const QString &key, Field *field) {
    if (!field_pool.contains(key)) {
        field_pool.insert(key, field);
    }
}

Field* Operator::field() const {
    return selected_field;
}

void Operator::selectFieldFromPool(const QString &key) {
    selected_field = field_pool.value(key, 0);
}

void Operator::selectField(Field *field) {
    selected_field = field;
}

void Operator::addVertex(int x, int y) {

}
