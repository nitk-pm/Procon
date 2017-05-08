#ifndef OPERATOR__H
#define OPERATOR__H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QMap>

class Field;
class Piece;

class Operator : public QObject {
    Q_OBJECT

public:
    explicit Operator(QObject *parent = 0);
    virtual ~Operator();

    static Field* createField(int width, int height);
    static void addFieldToPool(const QString &key, Field *field);

    Field* field() const;
    void selectFieldFromPool(const QString &key);
    void selectField(Field *field);
    void addVertex(int x, int y);
    void remove(int x, int y);

private:
    Field *selected_field;
    Piece *current_piece;

private:
    static QMap<QString, Field*> field_pool;
};

#endif // OPERATOR__H
