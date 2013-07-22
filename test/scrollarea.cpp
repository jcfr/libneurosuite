#include "scrollarea.h"
#include <QPalette>
#include <QVBoxLayout>

ScrollArea::ScrollArea(QWidget *parent)
    : QScrollArea(parent)
{
    setWidgetResizable(true);

    setAutoFillBackground(true);
    //Set the palette color
    QPalette palette;
    QColor backgroundColor(Qt::black);
    palette.setColor(backgroundRole(), backgroundColor);
    //Set the palette color, the foreground color depends on the background color
    int h;
    int s;
    int v;
    backgroundColor.getHsv(&h,&s,&v);
    QColor legendColor;
    if(s <= 80 && v >= 240 || (s <= 40 && v >= 220))
        legendColor = Qt::black;
    else
        legendColor = Qt::white;
    palette.setColor(foregroundRole(), legendColor);
    setPalette(palette);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QWidget *w = new QWidget;
    verticalContainer = new QVBoxLayout;
    w->setLayout(verticalContainer);
    setWidget(w);
    verticalContainer->setSpacing(5);

    //Set the legend in the good language
    languageChange();
    adjustSize();
}
