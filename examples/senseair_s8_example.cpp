#include <Arduino.h>
#define USE_LIB_WEBSOCKET true

#include "sensesp_app.h"

#include "signalk/signalk_output.h"

#include "sensors/senseair_s8.h"

ReactESP app([]() {
#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  sensesp_app = new SensESPApp();

  // Create the sensor and declare the communication pins (rx,tx)
  auto *s8_sensor = new Senseair_S8(D8, D7);

  // Define the read_delay you're going to use, if other than the default of 500
  // ms.
  const uint read_delay = 5000;

  // Create an co2_sensor_value, which is used to read a specific value from the
  // co2 sensor, and send its output to SignalK as a number (int).

  auto *s8_sensor_ppm = new Senseair_s8_value(s8_sensor, ppm, read_delay,
                                              "/enviroment/co2ppm");

  s8_sensor_ppm->connect_to(
      new SKOutputNumber("enviroment.vberth.co2ppm"));

  sensesp_app->enable();
});
