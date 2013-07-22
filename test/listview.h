#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QListWidget>

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent=0);
    void updateToContentSize();
protected:
#if 1
    QSize sizeHint() const;
    void resizeEvent(QResizeEvent *event);
#endif
};

#endif // LISTVIEW_H
