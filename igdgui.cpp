#include <QtGui> 
#include "igdgui.h"
 
IgdGuiApp::IgdGuiApp(QWidget *parent) : QMainWindow(parent) {
  setupUi(this);
  igd = new IGDDevice(this);
  connect( igd, SIGNAL( IgdStartScan() ), this, SLOT( SetScanWaitStatus() ) );
  connect( igd, SIGNAL( IgdScanFinished() ), this, SLOT( SetScanFinishedStatus() ) );
  connect( igd, SIGNAL( IgdDataRefreshed() ), this, SLOT( ReadIGDData() ) );
  igd->scan();
  
  connect( buttonScan, SIGNAL( clicked() ), igd, SLOT( scan() ) );
  connect( buttonRefresh, SIGNAL( clicked() ), igd, SLOT( refresh() ) );
  connect( buttonQuit, SIGNAL( clicked() ), qApp, SLOT( closeAllWindows() ) );

  forwardList->setModel(igd->getForwardDataModel());
}

void IgdGuiApp::SetScanWaitStatus() {
  labelStatus->setText("Scanning for UPnP IGD ...");
}

void IgdGuiApp::SetScanFinishedStatus() {
  labelStatus->setText("Scan finished.");
}

void IgdGuiApp::ReadIGDData() {
  lANIPAddressLineEdit->clear();
  externalIPAddressLineEdit->clear();
  if(igd->validIGD()) {
    labelStatus->setText("Connected to valid UPnP IGD.");
    lANIPAddressLineEdit->setText(igd->getLanIp());
    externalIPAddressLineEdit->setText(igd->getExternalIp());
  } else {
    labelStatus->setText("No valid UPnP IGD found.");
  }
}
