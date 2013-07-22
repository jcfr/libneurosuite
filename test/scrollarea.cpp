#include "scrollarea.h"
#include "itemgroupview.h"
#include "listview.h"
#include <QPalette>
#include <QVBoxLayout>
#
ScrollArea::ScrollArea(QWidget *parent)
    : QScrollArea(parent)
{
    setWidgetResizable(true);

    QFont f("Helvetica",8);
    QFontInfo fontInfo = QFontInfo(f);
    labelSize = fontInfo.pixelSize() * 3;


    setAutoFillBackground(true);
    //Set the palette color
    QPalette palette;
    backgroundColor = QColor(Qt::black);
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

void ScrollArea::createItemList(const QString &groupName)
{
    createGroup(groupName);
    update();
}

void ScrollArea::createGroup(const QString &id)
{
    ItemGroupView* group = new ItemGroupView(backgroundColor,this);

    group->setObjectName(id);
    GroupNameLabel* label = new GroupNameLabel(id,group);
    group->setLabel(label);

    //Set the size to 2 digits, max 99 groups
    label->setFixedWidth(labelSize);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QFont f("Helvetica",8);
    label->setFont(f);
    label->adjustSize();

    ListWidget* iconView = new ListWidget;
    iconView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    /*
    if(!iconviewDict.isEmpty()){
        QHashIterator<QString, ItemIconView*> iterator(iconviewDict);
        while (iterator.hasNext()) {
            iterator.next();
            iconView->resize((iconviewDict[iterator.key()])->size().width(),2);
        }
    }
    else
        iconView->adjustSize();
        */
    //group->setStretchFactor(label,0);
    //group->setStretchFactor(iconView,200);
    group->setIconView(iconView);

    delete spaceWidget;
    spaceWidget = new QWidget;
    verticalContainer->addWidget(spaceWidget);
    spaceWidget->show();
    verticalContainer->setStretchFactor(spaceWidget,2);

    //iconviewDict.insert(id,iconView);

    //itemGroupViewDict.insert(id,group);
    group->adjustSize();
    iconView->show();
    group->show();
    if (iconView->size().isNull()) {
        iconView->resize(50,50);
    }
    qDebug()<<" group"<<group->size();
    qDebug()<<" iconView"<<iconView->size();

/*
    //Signal and slot connection
    connect(iconView,SIGNAL(itemSelectionChanged()),this, SLOT(slotClickRedraw()));
    connect(iconView,SIGNAL(mousePressMiddleButton(QString,QListWidgetItem*)),this, SLOT(slotMousePressed(QString,QListWidgetItem*)));
    connect(this,SIGNAL(paletteResized(int,int)),group,SLOT(reAdjustSize(int,int)));
    connect(iconView,SIGNAL(mousePressWoModificators(QString)),this, SLOT(slotMousePressWoModificators(QString)));
    connect(iconView,SIGNAL(mousePressWAltButton(QString,QListWidgetItem*)),this, SLOT(slotMousePressWAltButton(QString,QListWidgetItem*)));
    connect(iconView,SIGNAL(mouseReleased(QString)),this, SLOT(slotMouseReleased(QString)));

    connect(label,SIGNAL(leftClickOnLabel(QString,bool,bool)),this, SLOT(slotMousePressed(QString,bool,bool)));
*/
    //TOODO
    //orderTheGroups();
    //emit paletteResized(viewport()->width(),labelSize);
    update();
}
