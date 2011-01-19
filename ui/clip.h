#ifndef CLIP_H
#define CLIP_H

#include <QMainWindow>
#include <QSignalMapper>

#include "tools/mousepositioninfo.h"

class Projector;
class ProjectionPlane;
class Crystal;
class MousePositionInfo;

namespace Ui {
  class Clip;
}

class Clip : public QMainWindow {
  Q_OBJECT

public:
  static Clip* getInstance();
  static void clearInstance();

  Crystal* getMostRecentCrystal(bool checkProjectors=false);
  Projector* getMostRecentProjector(bool withCrystal=false);
private:
  explicit Clip(QWidget *parent = 0);
  Clip(const Clip&);
  virtual ~Clip();
  static Clip* instance;
signals:
  void projectorRotation(double);
  void windowChanged();
  void mousePositionInfo(MousePositionInfo);
  void highlightMarker(Vec3D);
public slots:
  // Menu Slots
  void on_newCrystal_triggered();
  void on_newLaue_triggered();
  void on_newStereo_triggered();
  void on_actionAbout_triggered(bool);
  void on_actionAbout_Qt_triggered(bool);

  // Loads the initial Workspace (DefaultWorkspace.cws from ressource)
  void loadInitialWorkspace();
  bool loadWorkspaceFile(QString filename);

  // Slot for update of the Window-Submenu
  void slotUpdateWindowMenu();
  // Used by the Windows-Submenu
  void setActiveSubWindow(QWidget *window);
  void addMdiWindow(QWidget*);
protected:
  Projector* connectToLastCrystal(Projector*);
  ProjectionPlane* addProjector(Projector*);
  void addActions();
private:
  Ui::Clip *ui;

  QAction *closeAct;
  QAction *closeAllAct;
  QAction *tileAct;
  QAction *cascadeAct;
  QAction *nextAct;
  QAction *previousAct;
  QAction *separatorAct;
  QSignalMapper *windowMapper;

private slots:
    void on_actionSave_Workspace_triggered();
    void on_actionOpen_Workspace_triggered();
    void on_actionReorientation_triggered();
    void on_actionRotation_triggered();
    void on_actionReflection_Info_triggered();
};

#endif // CLIP_H
