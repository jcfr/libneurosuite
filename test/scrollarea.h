#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QScrollArea>
class QVBoxLayout;
class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit ScrollArea(QWidget *parent=0);
private:
    QVBoxLayout* verticalContainer;

};

#endif // SCROLLAREA_H
