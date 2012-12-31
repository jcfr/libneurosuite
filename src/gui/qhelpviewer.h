/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#ifndef QHELPVIEWER_H
#define QHELPVIEWER_H
#include <QDialog>

#include "libklustersshared_export.h"
class QWebView;

class KLUSTERSSHARED_EXPORT QHelpViewer : public QDialog
{
public:
    explicit QHelpViewer(QWidget *parent);
    ~QHelpViewer();
    void setHtml(const QString& filename, const QString &anchor = QString());
private:
    QWebView *mView;
};

#endif // QHELPVIEWER_H
