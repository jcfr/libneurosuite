#include "mainview.h"
#include <QSplitter>
#include <QHBoxLayout>

MainView::MainView(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout;
    QSplitter *splitter = new QSplitter;
    lay->addWidget(splitter);
    setLayout(lay);
}
