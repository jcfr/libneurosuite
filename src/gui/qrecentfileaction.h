/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#ifndef QRECENTFILEACTION_H
#define QRECENTFILEACTION_H

#include "libklustersshared_export.h"
#include <QAction>

class QRecentFileActionPrivate;

class KLUSTERSSHARED_EXPORT QRecentFileAction : public QAction
{
    Q_OBJECT
    Q_PROPERTY(int maximumFileCount READ maximumFileCount WRITE setMaximumFileCount)
public:
    explicit QRecentFileAction(QObject *parent);
    ~QRecentFileAction();

    /**
     * @brief addRecentFile
     * @param file
     */
    void addRecentFile(const QString& file);
    /**
     * @brief removeRecentFile
     * @param file
     */
    void removeRecentFile(const QString& file);

    /**
     * @brief save Save list of recent file in settings.
     */
    void save();

    /**
     * @brief maximumFileCount
     * @return the number of maximum file stored.
     */
    int maximumFileCount() const;

    /**
     * @brief setMaximumFileCount
     */
    void setMaximumFileCount(int) const;

public Q_SLOTS:
    /**
     * @brief clear clear recent menu.
     */
    void clear();

Q_SIGNALS:
    void recentFileSelected(const QString&);
    void recentFileCleared();

private Q_SLOTS:
    void fileSelected(QAction*);
    void initializeMenu();
private:
    QRecentFileActionPrivate * const d;
    Q_DISABLE_COPY(QRecentFileAction)

};

#endif // QRECENTFILEACTION_H
