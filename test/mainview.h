#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>
class ScrollArea;
class MainView : public QWidget
{
    Q_OBJECT
public:
    explicit MainView(QWidget *parent=0);
private:
    ScrollArea *scrollArea;
};

#endif // MAINVIEW_H
