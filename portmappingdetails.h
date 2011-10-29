#ifndef PORTMAPPINGDETAILS_H
#define PORTMAPPINGDETAILS_H

#include "ui_portmappingdetails.h"
#include <QStandardItem>

class PortMappingDetailsDialog : public QDialog, private Ui::PortMappingDetailsUi {
  Q_OBJECT

public:
  PortMappingDetailsDialog(QWidget *parent, QStandardItem *q);
};

#endif
