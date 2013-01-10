/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#include "qrecentfileaction.h"

#include <QSettings>
#include <QMenu>
#include <QDebug>
#include <QFileInfo>

QT_BEGIN_NAMESPACE

class QRecentFileActionPrivate
{
public:
    QRecentFileActionPrivate(QRecentFileAction *qq)
        : maximumFileCount(10),
          noEntriesAction(0),
          clearSeparator(0),
          clearAction(0),
          q(qq),
          initialized(false)
    {
        createRecentMenu();
    }

    void createRecentMenu();
    void addAction(const QString &file);
    void removeAction(QAction *act);
    void removeAction(const QString &file);
    void updateActionsState();
    void initializeRecentMenu();
    void fileSelected(QAction *action);

    QStringList recentFiles;
    int maximumFileCount;
    QAction *noEntriesAction;
    QAction *clearSeparator;
    QAction *clearAction;
    QRecentFileAction *q;
    bool initialized;
};

void QRecentFileActionPrivate::fileSelected(QAction *action)
{
    if (action && (action != clearAction) && (action != noEntriesAction) && (action != clearSeparator)) {
        Q_EMIT q->recentFileSelected(action->data().toString());
    }
}


void QRecentFileActionPrivate::initializeRecentMenu()
{
    if (initialized) {
        return;
    }

    initialized = true;

    noEntriesAction = q->menu()->addAction(q->tr("No Entries"));
    noEntriesAction->setEnabled(false);
    clearSeparator = q->menu()->addSeparator();
    clearAction = q->menu()->addAction(q->tr("Clear List"), q, SLOT(clear()));

    q->connect(q->menu(), SIGNAL(triggered(QAction*)),q, SLOT(fileSelected(QAction*)));

    if (!recentFiles.isEmpty()) {
        const int numberOfRecentFile(recentFiles.count());
        for( int i = 0 ; i < numberOfRecentFile; ++i ) {
            addAction(recentFiles.at(i));
            if( i == maximumFileCount) {
                break;
            }
        }
    }
    updateActionsState();

}

void QRecentFileActionPrivate::createRecentMenu()
{
    if (initialized) {
        return;
    }

    delete q->menu();
    QMenu *menu = new QMenu();
    q->setMenu(menu);
    q->connect(menu, SIGNAL(aboutToShow()), q, SLOT(initializeRecentMenu()));
}

void QRecentFileActionPrivate::addAction(const QString &file)
{
    if (file.isEmpty()) {
        return;
    }

    QString troncateFileName = file;
    if (file.length() > 30) {
        troncateFileName = file.left(15) + QLatin1String("...") + file.right(15);
    }

    const QString actionText = QString::fromLatin1("%1 [%2]").arg(QFileInfo(file).fileName()).arg(troncateFileName);
    QAction* action = new QAction(actionText,q);
    action->setToolTip(file);
    action->setData(file);
    q->menu()->insertAction(q->menu()->actions().value(0), action);
}

void QRecentFileActionPrivate::removeAction(QAction *act)
{
    q->menu()->removeAction(act);
    recentFiles.removeAll(act->data().toString());
}

void QRecentFileActionPrivate::updateActionsState()
{
    if(!initialized)
        return;
    //We have noEntriesAction/clearSeparator/clearAction action by default
    //=> an empty menu has number action <4
    const bool isMenuEmpty = (q->menu()->actions().count() < 4);
    noEntriesAction->setVisible(isMenuEmpty);
    clearSeparator->setVisible(!isMenuEmpty);
    clearAction->setVisible(!isMenuEmpty);
}

void QRecentFileActionPrivate::removeAction(const QString &file)
{
    Q_FOREACH (QAction *action, q->menu()->actions()) {
        if ( action->data().toString() == file ) {
            removeAction(action);
            break;
        }
    }
}

/*!
    Constructs a recent file menu with a \a parent.

    Although a popup menu is always a top-level widget, if a parent is
    passed the popup menu will be deleted when that parent is
    destroyed (as with any other QObject).
*/
QRecentFileAction::QRecentFileAction(QObject *parent)
    : QAction(parent), d(new QRecentFileActionPrivate(this))
{
    setText(tr("Recent Files..."));
}

/*!
    Destroys the recent file menu.
*/
QRecentFileAction::~QRecentFileAction()
{
    if (d->initialized) {
        save();
    }
    delete d;
}

/*!
    Clear the recent menu and displays "No entry" and emit recentFileCleared signal.
*/
void QRecentFileAction::clear()
{
    Q_FOREACH (QAction *action, menu()->actions()) {
        if ((action != d->clearAction) && (action != d->noEntriesAction) && (action != d->clearSeparator))
            d->removeAction(action);
    }
    save();
    Q_EMIT recentFileCleared();
}

/*!
    Append a new file with \a file to recent file list.
*/
void QRecentFileAction::addRecentFile(const QString &file)
{
    if (file.isEmpty())
        return;

    // remove file if already in list
    d->removeAction(file);
    if (d->maximumFileCount && menu()->actions().count() == d->maximumFileCount) {
        QAction *act = menu()->actions().first();
        d->removeAction(act);
    }
    d->recentFiles.append(file);
    d->addAction(file);
    save();
}

/*!
    Remove recent file \a file from recent file list.
*/
void QRecentFileAction::removeRecentFile(const QString &file)
{
    d->recentFiles.removeAll(file);
    d->removeAction(file);
    save();
}

/*!
    Emit recentFileListChanged signal when we must save recent file list.
*/
void QRecentFileAction::save()
{
    d->updateActionsState();
    Q_EMIT recentFileListChanged();
}

/*!
    Return the number of maximum file stored.
*/
int QRecentFileAction::maximumFileCount() const
{
    return d->maximumFileCount;
}

/*!
    This function will set the maximum file stored
*/
void QRecentFileAction::setMaximumFileCount(int maximumRecentFile )
{
    if (d->maximumFileCount != maximumRecentFile) {
        d->maximumFileCount = maximumRecentFile;
        // remove all excess items
        while ( menu()->actions().count() > d->maximumFileCount ) {
            QAction *act = menu()->actions().last();
            d->removeAction(act);
        }
    }
}

/*!
    This function will set the list of recent file which will displaying in recent file menu
*/
void QRecentFileAction::setListOfRecentFile(const QStringList& lst)
{
    d->recentFiles = lst;
    d->initialized = false;
    d->createRecentMenu();
}

/*!
    Return the list of recent file list.
*/
QStringList QRecentFileAction::listOfRecentFile() const
{
    return d->recentFiles;
}

/*!
    \fn void QRecentFileAction::recentFileListChanged();

    This signal is emitted when recent list file was changed.
*/



/*!
    \fn void QRecentFileAction::recentFileCleared();

    This signal is emitted when user clicks on "Clear" menu entry.
*/

/*!
    \fn void QRecentFileAction::recentFileSelected(const QString &file);

    This signal is emitted when a recent file is selected.

    \a file is the file selected.
*/


QT_END_NAMESPACE

#include "moc_qrecentfileaction.cpp"
