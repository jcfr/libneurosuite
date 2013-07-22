#include "listview.h"
#include <QFont>
#include <QPalette>
#include <QDebug>

ListWidget::ListWidget(QWidget *parent)
    : QListWidget(parent)
{
    QFont font( "Helvetica",8);
    setFont(font);
    setSpacing(4);
    setFrameStyle(QFrame::NoFrame);
    setResizeMode(QListWidget::Adjust);
    setViewMode(QListView::IconMode);
    setGridSize(QSize(50,50));
    setWordWrap(false);
    setAutoFillBackground(true);
    viewport()->setAutoFillBackground(false);
    //Set the iconView color, the foreground color depends on the background color
    QPalette palette;
    QColor backgroundColor(Qt::black);
    palette.setColor(backgroundRole(), backgroundColor);
    int h;
    int s;
    int v;
    backgroundColor.getHsv(&h,&s,&v);
    QColor legendColor;
    if(s <= 80 && v >= 240 || (s <= 40 && v >= 220))
        legendColor = Qt::black;
    else
        legendColor = Qt::white;
    palette.setColor(QPalette::Text, legendColor);
    setPalette(palette);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setMovement(QListView::Static);

    setSpacing(4);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);
    setSortingEnabled(true);

}

void ListWidget::updateToContentSize()
{
    const QSize size = QSize(contentsSize().width() + 5, qMax(contentsSize().height() + 5, 40));
    qDebug()<<" size "<<size;
    resize(size);
}

#if 1
QSize ListWidget::sizeHint() const
{
    const QSize size = QSize(contentsSize().width() + 5, qMax(contentsSize().height() + 5, 40));
    return size;
}

void ListWidget::resizeEvent(QResizeEvent *event)
{
    QListWidget::resizeEvent(event);
    resize(sizeHint());
}
#endif
