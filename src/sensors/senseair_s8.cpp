#include "sensors/senseair_s8.h"

#include <RemoteDebug.h>

#include "sensesp.h"

Senseair_S8::Senseair_S8(int rx, int tx, String config_path)
    : Sensor(config_path) {
  load_configuration();
  senseair_s8 = new SenseairCO2(rx, tx);
  senseair_s8->begin(rx, tx);
}

Senseair_s8_value::Senseair_s8_value(Senseair_S8* senseair_s8,
                                     Senseair_s8_value val_type,
                                     uint read_delay, String config_path)
    : NumericSensor(config_path),
      senseair_s8{senseair_s8},
      val_type{val_type},
      read_delay{read_delay} {
  load_configuration();
}

void Senseair_s8_value::enable() {
  app.onRepeat(read_delay, [this]() {
    switch (val_type) {
      case ppm:
        output = senseair_s8->readco2ppm;
        break;
      default:
        debugE("FATAL: invalid val_type parameter.");
    }

    notify();
  });
}

void Senseair_s8_value::get_configuration(JsonObject& root) {
  root["read_delay"] = read_delay;
  root["value"] = output;
}

static const char SCHEMA[] PROGMEM = R"###({
    "type": "object",
    "properties": {
        "read_delay": { "title": "Read delay", "type": "number", "description": "The time, in milliseconds, between each read of the input" },
        "value": { "title": "Last value", "type" : "number", "readOnly": true }
    }
  })###";

String Senseair_s8_value::get_config_schema() { return FPSTR(SCHEMA); }

bool Senseair_s8_value::set_configuration(const JsonObject& config) {
  String expected[] = {"read_delay"};
  for (auto str : expected) {
    if (!config.containsKey(str)) {
      return false;
    }
  }
  read_delay = config["read_delay"];
  return true;
}