#ifndef ADDPORTMAPPING_H
#define ADDPORTMAPPING_H

#include "ui_addportmapping.h"
#include "igddevice.h"

class AddPortMappingDialog : public QDialog, private Ui::AddPortMappingDialogUi {
  Q_OBJECT

public:
  AddPortMappingDialog(QWidget *parent, IGDDevice *igd);
private:
  IGDDevice* igd;
  void AddPortMappingFromDialog();
private slots:
  void buttonBoxClicked(QAbstractButton *button);
};

#endif
