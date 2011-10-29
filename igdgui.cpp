#include <QtGui>
#include <QMessageBox>
#include "igdgui.h"
#include "addportmapping.h"
#include "portmappingdetails.h"

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
  connect( buttonDelete, SIGNAL( clicked() ), this, SLOT( DeleteSelectedPortMapping() ) );
  connect( buttonNew, SIGNAL( clicked() ), this, SLOT( ShowAddPortMappingDialog() ) );
  connect( buttonDetails, SIGNAL( clicked() ), this, SLOT( ShowSelectedPortMappingDetails() ) );

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

void IgdGuiApp::DeleteSelectedPortMapping() {
  QStandardItem *current;
  QString error;
  QMessageBox errorDlg;

  current = igd->getForwardDataModel()->itemFromIndex(forwardList->currentIndex());
  if(current != NULL)
    if(!igd->deletePortMapping(error, current)) {
      errorDlg.setIcon(QMessageBox::Critical);
      errorDlg.setText("Could not remove port mapping:\n" + error);
      errorDlg.setWindowTitle("IGD GUI Error");
      errorDlg.exec();
    }
}

void IgdGuiApp::ShowAddPortMappingDialog() {
  AddPortMappingDialog *d = new AddPortMappingDialog(this, igd);
  d->setAttribute(Qt::WA_DeleteOnClose);
  d->show();
  d->raise();
  d->activateWindow();
}

void IgdGuiApp::ShowSelectedPortMappingDetails() {
  QStandardItem *current;
  PortMappingDetailsDialog *d;

  current = igd->getForwardDataModel()->itemFromIndex(forwardList->currentIndex());
  if(current != NULL) {
    d = new PortMappingDetailsDialog(this, current);
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->show();
    d->raise();
    d->activateWindow();
  }
}
