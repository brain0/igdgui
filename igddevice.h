#ifndef IGDDEVICE_H
#define IGDDEVICE_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <miniupnpc/miniupnpc.h>
#include <miniupnpc/upnpcommands.h>

class ScanThread : public QThread {
  Q_OBJECT
  
protected:
  void run();
public:
  ScanThread(QObject*, UPNPDev*&, QMutex&);
private:
  UPNPDev * & devlist;
  QMutex & devlist_mutex;
};

class IGDDevice : public QObject {
  Q_OBJECT
  
public:
  IGDDevice(QObject *parent = NULL);
  ~IGDDevice();
  
  bool validIGD() {
    return hasValidIGD;
  }
  const char* getLanIp() {
    if(!validIGD()) {
      return NULL;
    }
    return lanaddr;
  }
  const char* getExternalIp() {
    if(!validIGD()) {
      return NULL;
    }
    return externalip;
  }
public slots:
  void scan();
  void refresh();
signals:
  void IgdStartScan();
  void IgdScanFinished();
  void IgdDataRefreshed();
private:
  bool hasValidIGD;
  ScanThread *scanner;
  QMutex devlist_mutex;
  struct UPNPDev *devlist, *dev;
  struct UPNPUrls urls;
  struct IGDdatas data;
  char lanaddr[16], externalip[16];
};

#endif