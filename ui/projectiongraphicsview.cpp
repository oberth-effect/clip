#include "projectiongraphicsview.h"
#include <QMouseEvent>
#include <QTime>
#include <QGraphicsItem>
#include <QString>
#include <iostream>

using namespace std;

ProjectionGraphicsView::ProjectionGraphicsView(QWidget *parent) :
    QGraphicsView(parent) {
  viewIgnoresThisMouseEvent=false;
  frames=0;
}

void ProjectionGraphicsView::dragEnterEvent(QDragEnterEvent *e) {
  if (!not e->mimeData()->hasFormat("application/CrystalPointer"))
    QGraphicsView::dragEnterEvent(e);
  e->ignore();
}

void ProjectionGraphicsView::mousePressEvent(QMouseEvent *e) {
  QGraphicsView::mousePressEvent(e);
  viewIgnoresThisMouseEvent = (e->button()!=Qt::LeftButton) || !e->isAccepted();
  e->setAccepted(!viewIgnoresThisMouseEvent);
}

void ProjectionGraphicsView::mouseMoveEvent(QMouseEvent *e) {
  emit mouseMoved(mapToScene(e->pos()));
  if (viewIgnoresThisMouseEvent) {
    e->ignore();
  } else {
    QGraphicsView::mouseMoveEvent(e);
  }
}

void ProjectionGraphicsView::mouseReleaseEvent(QMouseEvent *e) {
  if (viewIgnoresThisMouseEvent) {
    e->ignore();
    viewIgnoresThisMouseEvent=false;
  } else {
    QGraphicsView::mouseReleaseEvent(e);
  }
}

void ProjectionGraphicsView::leaveEvent(QEvent *) {
  emit mouseLeft();
}

int ProjectionGraphicsView::getFrames() {
  int f = frames;
  frames = 0;
  return f;
}

void ProjectionGraphicsView::paintEvent(QPaintEvent *e) {
  QGraphicsView::paintEvent(e);
  frames++;
}

void ProjectionGraphicsView::setImage(LaueImage * img) {
  image = img;
}

#include <QPaintEngine>
#include <QImage>
#include <QRectF>
#include <tools/debug.h>


void ProjectionGraphicsView::drawBackground(QPainter *painter, const QRectF &rect) {
  if (image.isNull()) {
    painter->fillRect(rect, Qt::white);
  } else {
    QRectF visRect(mapToScene(0,0), mapToScene(viewport()->width(), viewport()->height()));
    QRectF sc(sceneRect());
    QRectF r((visRect.left()-sc.left())/sc.width(), (-visRect.bottom()-sc.top())/sc.height(), visRect.width()/sc.width(), visRect.height()/sc.height());
    QImage cache = image->getScaledImage(viewport()->size(), r.normalized());


    painter->save();
    painter->resetTransform();
    //painter->drawImage(viewportUpdateRect, cache, viewportUpdateRect);
    painter->drawImage(0, 0, cache);
    painter->restore();
  }
}

