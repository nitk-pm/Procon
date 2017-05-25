#ifndef EDITOR__H
#define EDITOR__H

#include <QtCore/QObject>

class QEvent;
class QGraphicsSceneMouseEvent;
class Document;

class Editor : public QObject {
    Q_OBJECT

public:
    Editor(QObject *parent = 0);
    Editor(Document *document, QObject *parent = 0);

private:
    bool eventFilter(QObject *watched, QEvent *event);

public:
    Document* document() const;
    void setDocument(Document *document);

    virtual void sceneEvent(QGraphicsSceneMouseEvent *event) {}

private:
    Document *_document;
};

#endif /* end of include guard: EDITOR__H */
