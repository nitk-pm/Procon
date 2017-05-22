#ifndef EDITOR__H
#define EDITOR__H

#include <QtCore/QObject>

class QEvent;
class QGraphicsSceneMouseEvent;
class Document;

class Editor : public QObject {
    Q_OBJECT

public:
    Editor(QObject *parent = 0) : QObject(parent) {}
    Editor(Document *document, QObject *parent = 0);

private:
    bool eventFilter(QObject *watched, QEvent *event);

public:
    Document* document() const;
    void setDocument(Document *document);

    virtual void sceneEvent(QGraphicsSceneMouseEvent *event) = 0;

private:
    Document *_document;
};

Q_DECLARE_METATYPE(Editor);

#endif /* end of include guard: EDITOR__H */
