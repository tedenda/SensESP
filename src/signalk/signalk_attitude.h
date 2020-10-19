#ifndef _signalk_attitude_H_
#define _signalk_attitude_H_

#include <ArduinoJson.h>

#include <set>

#include "Arduino.h"
#include "sensesp.h"
#include "signalk_output.h"

///////////////////
// provide correct output formatting for attitude

struct Attitude {
  float roll;
  float pitch;
  float yaw;
};

template <>
String SKOutput<Attitude>::as_signalk();

typedef SKOutput<Attitude> SKOutputAttitude;

#endif
