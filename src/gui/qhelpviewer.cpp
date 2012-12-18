/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#include "qhelpviewer.h"
#include <QHBoxLayout>
#include <QWebView>

QHelpViewer::QHelpViewer(QWidget *parent)
    :QDialog(parent)
{
    QHBoxLayout *lay = new QHBoxLayout;
    mView = new QWebView;
    lay->addWidget(mView);
    setLayout(lay);
}

QHelpViewer::~QHelpViewer()
{
}

void QHelpViewer::setHtml(const QString& filename)
{
    mView->load(QUrl(filename));
}
