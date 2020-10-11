#ifndef _senseair_s8_H_
#define _senseair_s8_H_

#include <Wire.h>
#include <senseair_co2.h>

#include "sensor.h"

class Senseair_S8 : public Sensor {
 public:
  Senseair_S8(int rx, int tx, String config_path = "");
  SenseairCO2* senseair_s8;
};

// Pass one of these in the constructor to co2_sensor_value() to tell which type
// of value you want to output
enum senseair_s8_value_type { ppm };

// co2_sensor_value reads and outputs the specified value of a SenseairCO2
// sensor.
class Senseair_s8_value : public NumericSensor {
 public:
  Senseair_s8_value(Senseair_S8* senseair_sensor,
                    senseair_s8_value_type val_type, uint read_delay = 5000,
                    String config_path = "");
  void enable() override final;
  Senseair_S8* senseair_s8;

 private:
  senseair_value_type val_type;
  uint read_delay;
  virtual void get_configuration(JsonObject& doc) override;
  virtual bool set_configuration(const JsonObject& config) override;
  virtual String get_config_schema() override;
};