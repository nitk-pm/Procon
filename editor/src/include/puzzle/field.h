#ifndef FIELD__H
#define FIELD__H

#include <QtCore/QObject>
#include <QtCore/QPoint>

class Field : public QObject {
    Q_OBJECT

public:
    static const int NONE = 0;
    static const int FRAME = 1;

public:
    Field(int width, int height, QObject *parent = 0);
    ~Field();

    const QPoint& getPrevSetPos() const;
    int  getData(int x, int y) const;
    int  getData(const QPoint &pos) const;

public slots:
    void setData(int x, int y, int value);
    void setData(const QPoint &pos, int value);

private:
    const int width = 0;
    const int height = 0;
    int *field;
    QPoint prev_set_pos;
};

#endif
