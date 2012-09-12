/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#ifndef QHELPVIEWER_H
#define QHELPVIEWER_H
#include <QDialog>

class QHelpViewer : public QDialog
{
public:
    explicit QHelpViewer(QWidget *parent);
    ~QHelpViewer();
    void setHtml(const QString& filename);
private:
};

#endif // QHELPVIEWER_H
