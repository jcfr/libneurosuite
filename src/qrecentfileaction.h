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
    Q_PROPERTY(int maximumNumberOfRecentFile READ maximumNumberOfRecentFile WRITE setMaximumNumberOfRecentFile)
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
     * @brief saveRecentFile
     */
    void saveRecentFile();

    /**
     * @brief maximumNumberOfRecentFile
     * @return
     */
    int maximumNumberOfRecentFile() const;
    /**
     * @brief setMaximumNumberOfRecentFile
     */
    void setMaximumNumberOfRecentFile(int) const;


private Q_SLOTS:
    void fileSelected(QAction*);

public Q_SLOTS:
    void clear();

Q_SIGNALS:
    void recentFileSelected(const QString&);
    void recentFileCleared();
private:
    QRecentFileActionPrivate *d;
    Q_DISABLE_COPY(QRecentFileAction)

};

#endif // QRECENTFILEACTION_H
