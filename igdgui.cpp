#include <QtGui>
#include <QMessageBox>
#include "igdgui.h"
#include "addportmapping.h"
#include "portmappingdetails.h"

#define AUTO_RESCAN_MIN_TIMEOUT 60
#define AUTO_REFRESH_MIN_TIMEOUT 5

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

  rescanTimer = new QTimer(this);
  refreshTimer = new QTimer(this);
  connect( rescanTimer, SIGNAL( timeout() ), igd, SLOT( scan() ) );
  connect( refreshTimer, SIGNAL( timeout() ), igd, SLOT( refresh() ) );
  connect( checkAutoRescan, SIGNAL( stateChanged(int) ), this, SLOT( changeAutoRescanState(int) ) );
  connect( checkAutoRefresh, SIGNAL( stateChanged(int) ), this, SLOT( changeAutoRefreshState(int) ) );
  connect( editAutoRescanTimeout, SIGNAL( editingFinished() ), this, SLOT( changeAutoRescanTimeout() ) );
  connect( editAutoRefreshTimeout, SIGNAL( editingFinished() ), this, SLOT( changeAutoRefreshTimeout() ) );
}

IgdGuiApp::~IgdGuiApp() {
  rescanTimer->stop();
  refreshTimer->stop();
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
  connectedCheckBox->setCheckState(Qt::Unchecked);
  if(igd->validIGD()) {
    labelStatus->setText("Connected to valid UPnP IGD.");
    lANIPAddressLineEdit->setText(igd->getLanIp());
    externalIPAddressLineEdit->setText(igd->getExternalIp());
    connectedCheckBox->setCheckState(igd->isConnected() ? Qt::Checked : Qt::Unchecked);
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

void IgdGuiApp::changeAutoRescanState(int state) {
  if(state == Qt::Checked) {
    editAutoRescanTimeout->setEnabled(true);
    rescanTimer->start(getValue(editAutoRescanTimeout, AUTO_RESCAN_MIN_TIMEOUT)*1000);
  } else if(state == Qt::Unchecked) {
    editAutoRescanTimeout->setEnabled(false);
  }
}

void IgdGuiApp::changeAutoRefreshState(int state) {
  if(state == Qt::Checked) {
    editAutoRefreshTimeout->setEnabled(true);
    refreshTimer->start(getValue(editAutoRefreshTimeout, AUTO_REFRESH_MIN_TIMEOUT)*1000);
  } else if(state == Qt::Unchecked) {
    editAutoRefreshTimeout->setEnabled(false);
  }
}

void IgdGuiApp::changeAutoRescanTimeout() {
  rescanTimer->setInterval(getValue(editAutoRescanTimeout, AUTO_RESCAN_MIN_TIMEOUT)*1000);
}

void IgdGuiApp::changeAutoRefreshTimeout() {
  refreshTimer->setInterval(getValue(editAutoRefreshTimeout, AUTO_REFRESH_MIN_TIMEOUT)*1000);
}

int IgdGuiApp::getValue(QLineEdit *e, int min) {
  int ret;

  ret = e->text().toInt();
  if(ret < min) {
    ret = min;
    e->setText(QString::number(min));
  }
  return ret;
}
