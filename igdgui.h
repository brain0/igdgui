#ifndef IGDGUI_H
#define IGDGUI_H
 
#include "ui_igdgui.h"
#include "igddevice.h"

class IgdGuiApp : public QMainWindow, private Ui::igdGuiMainUi {
  Q_OBJECT

public:
  IgdGuiApp(QWidget *parent = 0);
private slots:
  void SetScanWaitStatus();
  void SetScanFinishedStatus();
public slots:
  void ReadIGDData();
private:
  IGDDevice *igd;
};

#endif
