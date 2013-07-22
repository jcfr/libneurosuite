#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QScrollArea>
#include <QLabel>
#include <QMouseEvent>

class QVBoxLayout;
class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit ScrollArea(QWidget *parent=0);

    void createItemList(const QString &groupName);
    void createGroup(const QString &id);

private:
    QVBoxLayout* verticalContainer;
    /**Dummy widget used to keep the iconviews nicely display in the pannel.*/
    QWidget* spaceWidget;
    QColor backgroundColor;
    int labelSize;
};

class GroupNameLabel : public QLabel{
    Q_OBJECT
public:
    GroupNameLabel(const QString& text,QWidget* parent):
        QLabel(text,parent){}

Q_SIGNALS:
    void leftClickOnLabel(const QString& sourceId,bool shiftKey,bool ctrlAlt);
    void middleClickOnLabel(const QString& sourceId);

protected:
    void mousePressEvent(QMouseEvent* e){
        if(e->button() == Qt::LeftButton && !(e->modifiers() & Qt::ShiftModifier) && !(e->modifiers() & Qt::ControlModifier) && !(e->modifiers() & Qt::AltModifier)){
            emit leftClickOnLabel(parent()->objectName(),false,false);
        }
        if(e->button() == Qt::LeftButton && (e->modifiers() & Qt::ShiftModifier) && !(e->modifiers() & Qt::ControlModifier) && !(e->modifiers() & Qt::AltModifier)){
            emit leftClickOnLabel(parent()->objectName(),true,false);
        }
        if(e->button() == Qt::LeftButton && (e->modifiers() & Qt::ControlModifier) && (e->modifiers() & Qt::AltModifier)){
            emit leftClickOnLabel(parent()->objectName(),false,true);
        }
        if(e->button() == Qt::MidButton){
            emit middleClickOnLabel(parent()->objectName());
        }
    }

};

#endif // SCROLLAREA_H
