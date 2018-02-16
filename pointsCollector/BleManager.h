#ifndef BleManager_h
#define BleManager_h



class BleManager{
  public:
  BleManager();
  void init();
  private:
  BLEService _pcs;
  BLECharacteristic _vc;
};

BleManager::BleManager(){}

void BleManager::init()
{
  _pcs = BLEService();
  _vc = BLECharacteristic();
}





#endif
