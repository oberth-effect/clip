/**********************************************************************
  Copyright (C) 2008-2011 Olaf J. Schumann

  This file is part of the Cologne Laue Indexation Program.
  For more information, see <http://clip4.sf.net>

  Clip is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Clip is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see http://www.gnu.org/licenses/
  or write to the Free Software Foundation, Inc., 51 Franklin Street,
  Fifth Floor, Boston, MA 02110-1301, USA.
 **********************************************************************/

#include "indexdisplay.h"
#include "ui_indexdisplay.h"

 
 
#include <QSortFilterProxyModel>
#include <QShortcut>

#include "core/crystal.h"
#include "indexing/indexer.h"
#include "indexing/livemarkermodel.h"
#include "tools/zipiterator.h"
#include "tools/threadrunner.h"


IndexDisplay::IndexDisplay(Crystal* _c, QWidget* _parent) :
    QWidget(_parent),
    ui(new Ui::Indexing),
    crystal(_c),
    solutions(),
    indexer(nullptr),
    threads(new ThreadRunner)
{
  ui->setupUi(this);

  ui->SolutionSelector->setModel(&solutions);
  ui->SolutionSelector->sortByColumn(0, Qt::AscendingOrder);

  ui->SolutionSelector ->verticalHeader()->setDefaultSectionSize(fontMetrics().lineSpacing());

  ui->SolutionSelector->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  connect(ui->SolutionSelector->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(updateSolutionDisplay(QModelIndex,QModelIndex)));
  connect(&solutions, SIGNAL(solutionNumberChanged(int)), this, SLOT(showNumberOfSolutions(int)));
  connect(ui->maxIndex, SIGNAL(valueChanged(int)), this, SIGNAL(maxSearchIndexChanged(int)));

}

IndexDisplay::~IndexDisplay()
{
  stopIndexer();
  delete ui;
  delete threads;
}

int IndexDisplay::maxSearchIndex() {
  return ui->maxIndex->value();
}

void IndexDisplay::updateSolutionDisplay(QModelIndex index, QModelIndex /*prev*/) {
  int n=index.row();
  if (n>=0) {
    Solution s = solutions.getSolution(n);
    crystal->setRotation(s.bestRotation);
  }
}

void IndexDisplay::on_startButton_clicked()
{
  if (indexer) {
    stopIndexer();
  } else {
    ui->startButton->setText("Stop");
    ui->progress->setEnabled(true);
    ui->progress->setValue(0);

    indexer = new Indexer(crystal->getMarkers(),
                                   crystal->getRealOrientationMatrix(),
                                   crystal->getReziprocalOrientationMatrix(),
                                   M_PI/180.0*ui->AngDev->value(),
                                   0.01*ui->IntDev->value(),
                                   ui->maxIndex->value(),
                                   crystal->getSpacegroup()->getLauegroup(),
                                   this);
    connect(indexer, SIGNAL(publishSolution(Solution)), &solutions, SLOT(addSolution(Solution)));
    connect(indexer, SIGNAL(publishMultiSolutions(QList<Solution>)), &solutions, SLOT(addSolutions(QList<Solution>)));
    connect(indexer, SIGNAL(nextMajorIndex(int)), this, SLOT(showMajorIndex(int)));
    connect(indexer, SIGNAL(progressInfo(int)), this, SLOT(setProgress(int)));
    solutions.clear();

    threads->start(*indexer);
  }
}

void IndexDisplay::stopIndexer() {
  if (indexer) {
    indexer->stop();
    threads->join();
    delete indexer;
    indexer = nullptr;
    ui->startButton->setText("Start");
    ui->maxIndexDisplay->setText("");
    ui->progress->setEnabled(false);
    ui->progress->setValue(0);
  }
}

void IndexDisplay::showMajorIndex(int n) {
  ui->maxIndexDisplay->setText(QString::number(n));
  ui->progress->setMaximum((n+2)*(n+1)/2-1);
}

void IndexDisplay::setProgress(int value) {
  ui->progress->setValue(value);
}

void IndexDisplay::showNumberOfSolutions(int n) {
  ui->solutionsCount->setText(QString::number(n));
}


