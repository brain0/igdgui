#include "igddevice.h"

ScanThread::ScanThread(QObject *parent, UPNPDev*& devlist, QMutex& devlist_mutex) : QThread(parent), devlist(devlist), devlist_mutex(devlist_mutex) {}

void ScanThread::run() {
  int upnp_error;
  UPNPDev* my_devlist;

  my_devlist = upnpDiscover(2000, NULL, NULL, 0, 0, &upnp_error);

  devlist_mutex.lock();
  if(devlist != NULL)
    freeUPNPDevlist(devlist);
  devlist = my_devlist;
  devlist_mutex.unlock();
}

IGDDevice::IGDDevice(QObject *parent) : QObject(parent), hasValidIGD(false), devlist(NULL) {
  scanner = new ScanThread(this, devlist, devlist_mutex);
}

IGDDevice::~IGDDevice() {
  if(scanner != NULL)
    scanner->wait();
  if(devlist != NULL)
    freeUPNPDevlist(devlist);
  if(hasValidIGD)
    FreeUPNPUrls(&urls);
}

void IGDDevice::scan() {
  emit IgdStartScan();
  if(scanner->isRunning())
    return;
  connect( scanner, SIGNAL( finished() ), this, SIGNAL( IgdScanFinished() ) );
  connect( scanner, SIGNAL( finished() ), this, SLOT( refresh() ) );
  scanner->start();
}

void IGDDevice::refresh() {
  int i;

  if(hasValidIGD)
    FreeUPNPUrls(&urls);
  hasValidIGD = false;

  devlist_mutex.lock();
  i = UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr));
  devlist_mutex.unlock();
  if(i!=0) {
    if(i==3) {
      FreeUPNPUrls(&urls);
    } else {
      hasValidIGD = true;
      if(UPNP_GetExternalIPAddress(urls.controlURL, data.first.servicetype, externalip) != 0)
        externalip[0] = '\0';
    }
  }
  emit IgdDataRefreshed();
}
