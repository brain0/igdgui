#include <QApplication>
#include "igdgui.h"
 
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  IgdGuiApp *mw = new IgdGuiApp;
 
  mw->show();
  return app.exec();
}
