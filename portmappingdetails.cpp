#include "portmappingdetails.h"
#include "igddevice.h"

PortMappingDetailsDialog::PortMappingDetailsDialog(QWidget *parent, QStandardItem *q) : QDialog(parent) {
  uint proto;

  setupUi(this);

  descriptionLineEdit->setText(q->data(IGDDevice::DescriptionRole).toString());
  externalPortLineEdit->setText(q->data(IGDDevice::ExternalPortRole).toString());
  proto = q->data(IGDDevice::ProtocolRole).toUInt();
  if(proto == IGDDevice::ProtoTcp)
    protocolLineEdit->setText("TCP");
  else if(proto == IGDDevice::ProtoUdp)
    protocolLineEdit->setText("UDP");
  internalIPAddressLineEdit->setText(q->data(IGDDevice::InternalIpRole).toString());
  internalPortLineEdit->setText(q->data(IGDDevice::InternalPortRole).toString());
  durationLineEdit->setText(q->data(IGDDevice::DurationRole).toString());
  remoteHostLineEdit->setText(q->data(IGDDevice::RemoteHostRole).toString());
  enabledLineEdit->setText(q->data(IGDDevice::EnabledRole).toString());
}
