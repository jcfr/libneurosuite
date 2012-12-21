/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#ifndef QRECENTFILEACTION_H
#define QRECENTFILEACTION_H

#include "libklustersshared_export.h"
#include <QAction>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

class QRecentFileActionPrivate;

class KLUSTERSSHARED_EXPORT QRecentFileAction : public QAction
{
    Q_OBJECT
    Q_PROPERTY(int maximumFileCount READ maximumFileCount WRITE setMaximumFileCount)
public:
    explicit QRecentFileAction(QObject *parent = 0);
    ~QRecentFileAction();

    /**
     * @brief addRecentFile
     * @param file
     */
    void addRecentFile(const QString &file);
    /**
     * @brief removeRecentFile
     * @param file
     */
    void removeRecentFile(const QString &file);

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
    void setMaximumFileCount(int);

public Q_SLOTS:
    /**
     * @brief clear clear recent menu.
     */
    void clear();

Q_SIGNALS:
    void recentFileSelected(const QString &);
    void recentFileCleared();
    void recentFileListChanged();

private:
    QRecentFileActionPrivate * const d;
    friend class QRecentFileActionPrivate;
    Q_DISABLE_COPY(QRecentFileAction)
    Q_PRIVATE_SLOT(d, void initializeMenu())
    Q_PRIVATE_SLOT(d, void fileSelected(QAction *))
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QRECENTFILEACTION_H
