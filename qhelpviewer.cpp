/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#include "qhelpviewer.h"
#include <QHBoxLayout>

QHelpViewer::QHelpViewer(QWidget *parent)
    :QDialog(parent)
{
    QHBoxLayout *lay = new QHBoxLayout;
    setLayout(lay);
}

QHelpViewer::~QHelpViewer()
{
}

void QHelpViewer::setHtml(const QString& filename)
{
    //TODO
}
