#include "senseair_co2.h"

#include <RemoteDebug.h>

#include "Arduino.h"
#include "sensesp.h"

SenseairCO2::SenseairCO2() {}

SenseairCO2::SenseairCO2(int rx, int tx) { begin(rx, tx); }

SenseairCO2::~SenseairCO2() {}

void SenseairCO2::begin(int rx, int tx) {
  _rx_pin = rx;
  _tx_pin = tx;
}

int SenseairCO2::ModbusRTU_CRC(byte buf[], int len, byte check_sum[2]) {
  unsigned int crc = 0xFFFF;

  for (int pos = 0; pos < len; pos++) {
    crc ^= (unsigned int)buf[pos];  // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--) {  // Loop over each bit
      if ((crc & 0x0001) != 0) {    // If the LSB is set
        crc >>= 1;                  // Shift right and XOR 0xA001
        crc ^= 0xA001;
      } else        // Else LSB is not set
        crc >>= 1;  // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or
  // swap bytes)
  check_sum[1] = (byte)((crc >> 8) & 0xFF);
  check_sum[0] = (byte)(crc & 0xFF);
  return crc;
}

void SenseairCO2::BuildFrame(byte slave_address, byte function_code,
                             short start_address, short number_of_registers,
                             byte frame[8]) {
  frame[0] = slave_address;
  frame[1] = function_code;
  frame[2] = (byte)(start_address >> 8);
  frame[3] = (byte)(start_address);
  frame[4] = (byte)(number_of_registers >> 8);
  frame[5] = (byte)(number_of_registers);
  // CRC-calculation
  byte check_sum[2] = {0};
  ModbusRTU_CRC(frame, 6, check_sum);
  frame[6] = check_sum[0];
  frame[7] = check_sum[1];
}

int SenseairCO2::SendCommand(byte command[]) {
  byte recv_buf[7] = {0xff};
  byte data_buf[2] = {0xff};
  long value = -1;

  SoftwareSerial serial(_rx_pin, _tx_pin);
  serial.begin(9600);
  serial.write(command, 8);
  serial.flush();

  delay(50);

  // Read answer from sensor
  int byte_counter = 0;
  while (serial.available()) {
    recv_buf[byte_counter] = serial.read();
    byte_counter++;
  }

  data_buf[0] = recv_buf[3];
  data_buf[1] = recv_buf[4];
  value = (data_buf[0] << 8) | (data_buf[1]);

  return value;
}

int SenseairCO2::readco2ppm(void) {
  int co2 = 0;
  byte frame[8] = {0};
  BuildFrame(0xFE, 0x04, 0x03, 1, frame);
  co2 = SendCommand(frame);
  return co2;
}
