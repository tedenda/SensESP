#ifndef _SENSEAIR_CO2_
#define _SENSEAIR_CO2_

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "sensor.h"

class SenseairCO2 {
 public:
  SenseairCO2();
  SenseairCO2(int rx, int tx);
  virtual ~SenseairCO2();

  void begin(int rx, int tx);

 protected:
  int readco2ppm(void);

 private:
  int _rx_pin = -1;
  int _tx_pin = -1;

  int ModbusRTU_CRC(byte buf[], int len, byte check_sum[2]);
  int SendCommand(byte command[]);

  void BuildFrame(byte slave_address, byte function_code, short start_address,
                  short number_of_registers, byte frame[8]);
};

#endif