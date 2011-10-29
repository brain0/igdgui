#ifndef IGDDEVICE_H
#define IGDDEVICE_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QStandardItemModel>
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
  
  static const int PortProtoSortRole = Qt::UserRole,
                   ExternalPortRole = Qt::UserRole + 1,
                   InternalIpRole = Qt::UserRole + 2,
                   InternalPortRole = Qt::UserRole + 3,
                   ProtocolRole = Qt::UserRole + 4,
                   DescriptionRole = Qt::UserRole + 5,
                   EnabledRole = Qt::UserRole + 6,
                   RemoteHostRole = Qt::UserRole + 7,
                   DurationRole = Qt::UserRole + 8;
  static const uint ProtoInvalid = 2, ProtoTcp = 0, ProtoUdp = 1;

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
  QStandardItemModel *getForwardDataModel() { return mForwardData; }
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
  QStandardItemModel *mForwardData;
  void readPortMappingsIntoModel();
};

#endif