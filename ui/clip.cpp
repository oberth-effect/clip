#include "clip.h"
#include "ui_clip.h"
#include <ui/crystaldisplay.h>
#include <QMdiSubWindow>
#include <core/projector.h>
#include <ui/projectionplane.h>
#include <core/laueplaneprojector.h>
#include <core/stereoprojector.h>
#include <QMessageBox>
#include <defs.h>
#include "ui/mouseinfodisplay.h"



Clip::Clip(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Clip)
{
  ui->setupUi(this);

  windowMapper = new QSignalMapper(this);
  connect(windowMapper, SIGNAL(mapped(QWidget*)),
          this, SLOT(setActiveSubWindow(QWidget*)));

  addActions();
  connect(ui->menuWindows, SIGNAL(aboutToShow()), this, SLOT(slotUpdateWindowMenu()));

  on_newCrystal_triggered();
  on_newLaue_triggered();
  on_newStereo_triggered();

}

Clip::Clip(const Clip &) {
}


Clip::~Clip(){
  delete ui;
}

Clip* Clip::instance = 0;

Clip* Clip::getInstance() {
  if (!instance)
    instance = new Clip();
  return instance;
}

void Clip::clearInstance() {
  if (instance) {
    delete instance;
    instance = 0;
  }
}

void Clip::on_newCrystal_triggered() {
  addMdiWindow(new CrystalDisplay(this));
}


void Clip::on_newLaue_triggered() {
  addProjector(new LauePlaneProjector(this));
}

void Clip::on_newStereo_triggered() {
  addProjector(new StereoProjector(this));
}

void Clip::addProjector(Projector* p) {
  ProjectionPlane* pp = new ProjectionPlane(connectToLastCrystal(p), this);
  addMdiWindow(pp);
}

Projector* Clip::connectToLastCrystal(Projector *p) {
  QList<QMdiSubWindow*> mdiWindows = ui->mdiArea->subWindowList(QMdiArea::StackingOrder);
  while (!mdiWindows.empty()) {
    QMdiSubWindow* window = mdiWindows.takeLast();
    CrystalDisplay* cd = dynamic_cast<CrystalDisplay*>(window->widget());
    if (cd) {
      p->connectToCrystal(cd->getCrystal());
      break;
    }
  }
  return p;
}

void Clip::addMdiWindow(QWidget* w) {
  QMdiSubWindow* m = ui->mdiArea->addSubWindow(w);
  m->setAttribute(Qt::WA_DeleteOnClose);
  m->show();
  connect(w, SIGNAL(destroyed()), m, SLOT(deleteLater()));
}

void Clip::setActiveSubWindow(QWidget *window) {
  if (!window)
    return;
  QMdiSubWindow* mdiWin= qobject_cast<QMdiSubWindow *>(window);
  if (mdiWin) {
    ui->mdiArea->setActiveSubWindow(mdiWin);
  } else {
    foreach (mdiWin, ui->mdiArea->subWindowList()) {
      if (mdiWin->widget()==window) {
        ui->mdiArea->setActiveSubWindow(mdiWin);
      }
    }
  }
}

void Clip::on_actionAbout_triggered(bool) {
  QString message("This is the Cologne Laue Indexation Program (CLIP)\n");
  message += "a program to index and refine laue exposures.\n\n";
  message += "Version 4.0alpha\n";
  message += "Mercurial ID: " + QString(HG_REPRO_ID) + " Revision: " + HG_REPRO_REV + "\n";
  message += "Mercurial Date: " + QString(HG_REPRO_DATE) + "\n";
  message += "Build Date: " + QString(BUILD_DATE) +" " + QString(BUILD_TIME) + "\n\n";
  message += "Copyright (C) 2010 O.J.Schumann (o.j.schumann@gmail.com)\n";
  message += "Clip is licensed under the terms of the GNU General Public License.";


  QMessageBox::about(this, "Cologne Laue Indexation Program", message);
}

void Clip::on_actionAbout_Qt_triggered(bool) {
  QMessageBox::aboutQt(this, "Cologne Laue Indexation Program");
}


void Clip::slotUpdateWindowMenu() {
  ui->menuWindows->clear();
  ui->menuWindows->addAction(closeAct);
  ui->menuWindows->addAction(closeAllAct);
  ui->menuWindows->addSeparator();
  ui->menuWindows->addAction(tileAct);
  ui->menuWindows->addAction(cascadeAct);
  ui->menuWindows->addSeparator();
  ui->menuWindows->addAction(nextAct);
  ui->menuWindows->addAction(previousAct);
  ui->menuWindows->addAction(separatorAct);

  QList<QMdiSubWindow *> windows = ui->mdiArea->subWindowList();
  separatorAct->setVisible(!windows.isEmpty());

  for (int i = 0; i < windows.size(); ++i) {

    QString text;
    if (i < 9) {
      text = tr("&%1 %2").arg(i + 1)
             .arg(windows.at(i)->widget()->windowTitle());
    } else {
      text = tr("%1 %2").arg(i + 1)
             .arg(windows.at(i)->widget()->windowTitle());
    }
    QAction *action  = ui->menuWindows->addAction(text);
    action->setCheckable(true);
    action ->setChecked(windows.at(i) == ui->mdiArea->activeSubWindow());
    connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
    windowMapper->setMapping(action, windows.at(i));
  }
}

void Clip::addActions() {
  closeAct = new QAction(tr("Cl&ose"), this);
  closeAct->setStatusTip(tr("Close the active window"));
  connect(closeAct, SIGNAL(triggered()),
          ui->mdiArea, SLOT(closeActiveSubWindow()));

  closeAllAct = new QAction(tr("Close &All"), this);
  closeAllAct->setStatusTip(tr("Close all the windows"));
  connect(closeAllAct, SIGNAL(triggered()),
          ui->mdiArea, SLOT(closeAllSubWindows()));

  tileAct = new QAction(tr("&Tile"), this);
  tileAct->setStatusTip(tr("Tile the windows"));
  connect(tileAct, SIGNAL(triggered()), ui->mdiArea, SLOT(tileSubWindows()));

  cascadeAct = new QAction(tr("&Cascade"), this);
  cascadeAct->setStatusTip(tr("Cascade the windows"));
  connect(cascadeAct, SIGNAL(triggered()), ui->mdiArea, SLOT(cascadeSubWindows()));

  nextAct = new QAction(tr("Ne&xt"), this);
  nextAct->setShortcuts(QKeySequence::NextChild);
  nextAct->setStatusTip(tr("Move the focus to the next window"));
  connect(nextAct, SIGNAL(triggered()),
          ui->mdiArea, SLOT(activateNextSubWindow()));

  previousAct = new QAction(tr("Pre&vious"), this);
  previousAct->setShortcuts(QKeySequence::PreviousChild);
  previousAct->setStatusTip(tr("Move the focus to the previous "
                               "window"));
  connect(previousAct, SIGNAL(triggered()),
          ui->mdiArea, SLOT(activatePreviousSubWindow()));

  separatorAct = new QAction(this);
  separatorAct->setSeparator(true);

}


void Clip::on_actionReflection_Info_triggered() {
  MouseInfoDisplay* info = new MouseInfoDisplay();
  foreach (QMdiSubWindow* mdi, ui->mdiArea->subWindowList()) {
    if (ProjectionPlane* pp = dynamic_cast<ProjectionPlane*>(mdi->widget())) {
      connect(pp, SIGNAL(mousePositionInfo(MousePositionInfo)), info, SLOT(showMouseInfo(MousePositionInfo)));
    }
  }
  addMdiWindow(info);
}
