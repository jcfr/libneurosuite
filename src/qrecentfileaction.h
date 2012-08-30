/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#ifndef QRECENTFILEACTION_H
#define QRECENTFILEACTION_H

#include "libklustersshared_export.h"
#include <QAction>

class QRecentFileActionPrivate;

class KLUSTERSSHARED_EXPORT QRecentFileAction : QAction
{
    Q_OBJECT
public:
    explicit QRecentFileAction(QObject *parent);
    ~QRecentFileAction();

    void addRecentFile(const QString&);
    void removeRecentFile(const QString&);
    void saveRecentFile();
    void clearRecentFile();

    int maximumNumberOfRecentFile() const;
    void setMaximumNumberOfRecentFile(int) const;

    void fileSelected(QAction*);
private:
    void initMenu();
    void loadRecentFile();
    void fillRecentMenu();
    QRecentFileActionPrivate *d;
    Q_DISABLE_COPY(QRecentFileAction)

};

#endif // QRECENTFILEACTION_H
