/*
    This file is part of the KDE Libraries

    Copyright (C) 2006 Tobias Koenig (tokoe@kde.org)
    Copyright (C) 2007 Rafael Fernández López (ereslibre@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB. If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "qpageview.h"
#include "qpageview_p.h"

#include "qpagemodel.h"

#include <kdialog.h>
#include <kiconloader.h>
#include <ktitlewidget.h>

#include <QAbstractItemView>
#include <QGridLayout>
#include <QLabel>
#include <QSize>
#include <QStackedWidget>
#include <QTimer>

void QPageViewPrivate::_k_rebuildGui()
{
  // clean up old view
    Q_Q(QPageView);

  QModelIndex currentLastIndex;
  if ( view && view->selectionModel() ) {
        QObject::disconnect(view->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                q, SLOT(_k_pageSelected(QItemSelection,QItemSelection)));
        currentLastIndex = view->selectionModel()->currentIndex();
  }

  delete view;
    view = q->createView();

  Q_ASSERT( view );

  view->setSelectionBehavior( QAbstractItemView::SelectItems );
  view->setSelectionMode( QAbstractItemView::SingleSelection );

  if ( model ) {
    view->setModel( model );
  }

  // setup new view
  if ( view->selectionModel() ) {
    QObject::connect(view->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), q, SLOT(_k_pageSelected(QItemSelection,QItemSelection)));

    if ( currentLastIndex.isValid() )
      view->selectionModel()->setCurrentIndex( currentLastIndex, QItemSelectionModel::Select );
    else if ( model )
      view->selectionModel()->setCurrentIndex( model->index( 0, 0 ), QItemSelectionModel::Select );
  }

  if (faceType == QPageView::Tabbed) {
    stack->setVisible(false);
    layout->removeWidget( stack );
  } else {
    layout->addWidget( stack, 2, 1 );
    stack->setVisible(true);
  }

    titleWidget->setVisible(q->showPageHeader());

    Qt::Alignment alignment = q->viewPosition();
  if ( alignment & Qt::AlignTop )
    layout->addWidget( view, 2, 1 );
  else if ( alignment & Qt::AlignRight )
    layout->addWidget( view, 1, 2, 2, 1 );
  else if ( alignment & Qt::AlignBottom )
    layout->addWidget( view, 4, 1 );
  else if ( alignment & Qt::AlignLeft )
    layout->addWidget( view, 1, 0, 2, 1 );
}

void QPageViewPrivate::updateSelection()
{
  /**
   * Select the first item in the view if not done yet.
   */

  if ( !model )
    return;

  if ( !view || !view->selectionModel() )
    return;

  const QModelIndex index = view->selectionModel()->currentIndex();
  if ( !index.isValid() )
    view->selectionModel()->setCurrentIndex( model->index( 0, 0 ), QItemSelectionModel::Select );
}

void QPageViewPrivate::cleanupPages()
{
  /**
   * Remove all orphan pages from the stacked widget.
   */

  const QList<QWidget*> widgets = collectPages();

  for ( int i = 0; i < stack->count(); ++i ) {
    QWidget *page = stack->widget( i );

    bool found = false;
    for ( int j = 0; j < widgets.count(); ++j ) {
      if ( widgets[ j ] == page )
        found = true;
    }

    if ( !found )
      stack->removeWidget( page );
  }
}

QList<QWidget *> QPageViewPrivate::collectPages(const QModelIndex &parentIndex)
{
  /**
   * Traverse through the model recursive and collect all widgets in
   * a list.
   */
  QList<QWidget*> retval;

  int rows = model->rowCount( parentIndex );
  for ( int j = 0; j < rows; ++j ) {
    const QModelIndex index = model->index( j, 0, parentIndex );
    retval.append( qvariant_cast<QWidget*>( model->data( index, QPageModel::WidgetRole ) ) );

    if ( model->rowCount( index ) > 0 ) {
      retval += collectPages( index );
    }
  }

  return retval;
}

QPageView::FaceType QPageViewPrivate::detectAutoFace() const
{
  if ( !model )
    return QPageView::Plain;

  /**
   * Check whether the model has sub pages.
   */
  bool hasSubPages = false;
  const int count = model->rowCount();
  for ( int i = 0; i < count; ++i ) {
    if ( model->rowCount( model->index( i, 0 ) ) > 0 ) {
      hasSubPages = true;
      break;
    }
  }

  if ( hasSubPages )
    return QPageView::Tree;

  if ( model->rowCount() > 1 )
    return QPageView::List;

  return QPageView::Plain;
}

void QPageViewPrivate::_k_modelChanged()
{
  if ( !model )
    return;

  /**
   * If the face type is Auto, we rebuild the GUI whenever the layout
   * of the model changes.
   */
    if (faceType == QPageView::Auto) {
        _k_rebuildGui();
        // If you discover some crashes use the line below instead...
        //QTimer::singleShot(0, q, SLOT(_k_rebuildGui()));
    }

  /**
   * Set the stack to the minimum size of the largest widget.
   */
  QSize size = stack->size();
  const QList<QWidget*> widgets = collectPages();
  for ( int i = 0; i < widgets.count(); ++i ) {
    const QWidget *widget = widgets[ i ];
    if ( widget )
      size = size.expandedTo( widget->minimumSizeHint() );
  }
  stack->setMinimumSize( size );

  updateSelection();
}

void QPageViewPrivate::_k_pageSelected(const QItemSelection &index, const QItemSelection &previous)
{
  if ( !model )
    return;

  // Return if the current Index is not valid
  if ( index.indexes().size() != 1 ) {
    return;
  }
  QModelIndex currentIndex = index.indexes().first();

  QModelIndex previousIndex;
  // The previous index can be invalid
  if ( previous.indexes().size() == 1 ) {
    previousIndex = previous.indexes().first();
  }

  if (faceType != QPageView::Tabbed) {
  QWidget *widget = qvariant_cast<QWidget*>( model->data( currentIndex, QPageModel::WidgetRole ) );

  if ( widget ) {
    if ( stack->indexOf( widget ) == -1 ) { // not included yet
      stack->addWidget( widget );
    }

    stack->setCurrentWidget( widget );
  } else {
    stack->setCurrentWidget( defaultWidget );
  }

  updateTitleWidget(currentIndex);
  }

  Q_Q(QPageView);
  emit q->currentPageChanged(currentIndex, previousIndex);
}

void QPageViewPrivate::updateTitleWidget(const QModelIndex& index)
{
    Q_Q(QPageView);

    QString header = model->data( index, QPageModel::HeaderRole ).toString();
    if ( header.isNull() ) { //TODO KDE5 remove that ugly logic, see also doxy-comments in QPageWidgetItem::setHeader()
        header = model->data( index, Qt::DisplayRole ).toString();
    }

    const QIcon icon = model->data( index, Qt::DecorationRole ).value<QIcon>();
    titleWidget->setPixmap( icon.pixmap( 22, 22 ) );
    titleWidget->setText( header );

    titleWidget->setVisible(q->showPageHeader());
}

void QPageViewPrivate::_k_dataChanged(const QModelIndex &, const QModelIndex &)
{
  /**
   * When data has changed we update the header and icon for the currently selected
   * page.
   */
  if ( !view )
    return;

  QModelIndex index = view->selectionModel()->currentIndex();
  if ( !index.isValid() )
    return;

  updateTitleWidget( index );
}

QPageViewPrivate::QPageViewPrivate(QPageView *_parent)
    : q_ptr(_parent), model(0), faceType(QPageView::Auto),
    layout(0), stack(0), titleWidget(0), view(0)
{
}

void QPageViewPrivate::init()
{
    Q_Q(QPageView);
    layout = new QGridLayout(q);
    stack = new QPageStackedWidget(q);
    titleWidget = new KTitleWidget(q);
    QPixmap emptyPixmap(22, 22);
    emptyPixmap.fill(Qt::transparent);
    titleWidget->setPixmap(emptyPixmap);
    layout->addWidget(titleWidget, 1, 1);
    layout->addWidget(stack, 2, 1);

    defaultWidget = new QWidget(q);
    stack->addWidget(defaultWidget);

    // stack should use most space
    layout->setColumnStretch(1, 1);
    layout->setRowStretch(2, 1);
}

/**
 * QPageView Implementation
 */
QPageView::QPageView( QWidget *parent )
    : QWidget(parent), d_ptr(new QPageViewPrivate(this))
{
    d_ptr->init();
}

QPageView::QPageView(QPageViewPrivate &dd, QWidget *parent)
    : QWidget(parent), d_ptr(&dd)
{
    d_ptr->init();
}

QPageView::~QPageView()
{
    delete d_ptr;
}

void QPageView::setModel(QAbstractItemModel *model)
{
    Q_D(QPageView);
  // clean up old model
  if ( d->model ) {
        disconnect(d->model, SIGNAL(layoutChanged()), this, SLOT(_k_modelChanged()));
        disconnect(d->model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SLOT(_k_dataChanged(QModelIndex,QModelIndex)));
  }

  d->model = model;

  if ( d->model ) {
        connect(d->model, SIGNAL(layoutChanged()), this, SLOT(_k_modelChanged()));
        connect(d->model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SLOT(_k_dataChanged(QModelIndex,QModelIndex)));

    // set new model in navigation view
    if ( d->view )
      d->view->setModel( model );
  }

    d->_k_rebuildGui();
}

QAbstractItemModel* QPageView::model() const
{
    return d_func()->model;
}

void QPageView::setFaceType( FaceType faceType )
{
    Q_D(QPageView);
  d->faceType = faceType;

    d->_k_rebuildGui();
}

QPageView::FaceType QPageView::faceType() const
{
    return d_func()->faceType;
}

void QPageView::setCurrentPage( const QModelIndex &index )
{
    Q_D(QPageView);
  if ( !d->view || !d->view->selectionModel() )
    return;

  d->view->selectionModel()->setCurrentIndex( index, QItemSelectionModel::SelectCurrent );
}

QModelIndex QPageView::currentPage() const
{
    Q_D(const QPageView);
  if ( !d->view || !d->view->selectionModel() )
    return QModelIndex();

  return d->view->selectionModel()->currentIndex();
}

void QPageView::setItemDelegate( QAbstractItemDelegate *delegate )
{
    Q_D(QPageView);
  if ( d->view )
    d->view->setItemDelegate( delegate );
}

QAbstractItemDelegate* QPageView::itemDelegate() const
{
    Q_D(const QPageView);
  if ( d->view )
    return d->view->itemDelegate();
  else
    return 0;
}

void QPageView::setDefaultWidget( QWidget *widget )
{
    Q_D(QPageView);

    Q_ASSERT(widget);

    bool isCurrent = (d->stack->currentIndex() == d->stack->indexOf( d->defaultWidget ));

    // remove old default widget
    d->stack->removeWidget( d->defaultWidget );
    delete d->defaultWidget;

    // add new default widget
    d->defaultWidget = widget;
    d->stack->addWidget(d->defaultWidget);

    if (isCurrent)
        d->stack->setCurrentWidget(d->defaultWidget);
}

QAbstractItemView* QPageView::createView()
{
    Q_D(QPageView);
  if ( d->faceType == Auto ) {
    const FaceType faceType = d->detectAutoFace();

    if ( faceType == Plain )
      return new KDEPrivate::QPagePlainView( this );
    else if ( faceType == List )
      return new KDEPrivate::QPageListView( this );
    else if ( faceType == Tree )
      return new KDEPrivate::QPageTreeView( this );
    else // should never happen
      return 0;
  } else if ( d->faceType == Plain )
    return new KDEPrivate::QPagePlainView( this );
  else if ( d->faceType == List )
    return new KDEPrivate::QPageListView( this );
  else if ( d->faceType == Tree )
    return new KDEPrivate::QPageTreeView( this );
  else if ( d->faceType == Tabbed )
    return new KDEPrivate::QPageTabbedView( this );
  else
    return 0;
}

bool QPageView::showPageHeader() const
{
    Q_D(const QPageView);
  FaceType faceType = d->faceType;

  if ( faceType == Auto )
    faceType = d->detectAutoFace();

  if ( faceType == Tabbed ) {
    return false;
  } else {
    return !d->titleWidget->text().isEmpty();
  }
}

Qt::Alignment QPageView::viewPosition() const
{
    Q_D(const QPageView);
  FaceType faceType = d->faceType;

  if ( faceType == Auto )
    faceType = d->detectAutoFace();

  if ( faceType == Plain || faceType == Tabbed )
    return Qt::AlignTop;
  else
    return Qt::AlignLeft;
}

#include "qpageview.moc"
