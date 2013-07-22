#include "mainview.h"
#include "scrollarea.h"
#include <QSplitter>
#include <QHBoxLayout>
#include <QTextEdit>

MainView::MainView(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout;
    QSplitter *splitter = new QSplitter;

    scrollArea = new ScrollArea;
    splitter->addWidget(scrollArea);
    splitter->addWidget(new QTextEdit);

    lay->addWidget(splitter);
    setLayout(lay);

    resize(800,600);
}
