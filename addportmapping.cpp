#include <QtGui>
#include <QString>
#include <QList>
#include <QMessageBox>
#include "addportmapping.h"

AddPortMappingDialog::AddPortMappingDialog(QWidget *parent, IGDDevice *igd) : QDialog(parent), igd(igd) {
  const char *ip;

  setupUi(this);

  ip = igd->getLanIp();
  if(ip != NULL)
    internalIPAddressLineEdit->setText(ip);

  connect( buttonBox, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( buttonBoxClicked( QAbstractButton* ) ) );
}

void AddPortMappingDialog::buttonBoxClicked(QAbstractButton *button) {
  QDialogButtonBox::ButtonRole r = buttonBox->buttonRole(button);
  if(r == QDialogButtonBox::AcceptRole || r == QDialogButtonBox::ApplyRole)
    AddPortMappingFromDialog();
}

void AddPortMappingDialog::AddPortMappingFromDialog() {
  QString ExtPort, IntPort, IntClient, Description, error;
  QList<QString> Protocols;
  QList<QString>::iterator p;
  int i;
  QMessageBox errorDlg;

  ExtPort = externalPortLineEdit->text();
  IntPort = internalPortLineEdit->text();
  IntClient = internalIPAddressLineEdit->text();
  Description = descriptionLineEdit->text();
  if(ExtPort.length() == 0 || IntPort.length() == 0 || IntClient.length() == 0 || Description.length() == 0)
    return;
  i = protocolComboBox->currentIndex();
  if(i == 0 || i == 2)
    Protocols.push_back("TCP");
  if(i == 1 || i == 2)
    Protocols.push_back("UDP");

  for(p=Protocols.begin(); p!=Protocols.end(); p++) {
    if(!igd->addPortMappingFromStrings(error, ExtPort, *p, IntPort, IntClient, Description)) {
      errorDlg.setIcon(QMessageBox::Critical);
      errorDlg.setText("Could not add port mapping:\n" + error);
      errorDlg.setWindowTitle("IGD GUI Error");
      errorDlg.exec();
    }
  }
}
