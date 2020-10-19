
#include "signalk_attitude.h"

template <>
String SKOutput<Attitude>::as_signalk() {
  DynamicJsonDocument jsonDoc(1024);
  String json;
  JsonObject root = jsonDoc.as<JsonObject>();
  root["path"] = this->get_sk_path();
  JsonObject value = root.createNestedObject("value");
  value["roll"] = output.roll;
  value["pitch"] = output.pitch;
  value["yaw"] = output.yaw;
  serializeJson(root, json);
  return json;
}
