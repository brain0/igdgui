#ifndef IGDGUI_H
#define IGDGUI_H
 
#include "ui_igdgui.h"
#include "igddevice.h"
#include <QTimer>

class IgdGuiApp : public QMainWindow, private Ui::igdGuiMainUi {
  Q_OBJECT

public:
  IgdGuiApp(QWidget *parent = 0);
  ~IgdGuiApp();
private slots:
  void SetScanWaitStatus();
  void SetScanFinishedStatus();
  void DeleteSelectedPortMapping();
  void ShowSelectedPortMappingDetails();
  void changeAutoRescanState(int);
  void changeAutoRefreshState(int);
  void changeAutoRescanTimeout();
  void changeAutoRefreshTimeout();
public slots:
  void ReadIGDData();
  void ShowAddPortMappingDialog();
private:
  IGDDevice *igd;
  QTimer *rescanTimer, *refreshTimer;
  int getValue(QLineEdit *e, int min);
};

#endif
