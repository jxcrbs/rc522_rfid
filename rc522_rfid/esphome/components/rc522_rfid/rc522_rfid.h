#pragma once
#include "esphome.h"
#include <Wire.h>
#include <MFRC522_I2C.h>

namespace esphome {
namespace rc522_rfid {

class RC522RFID : public PollingComponent {
 public:
  // Constructor: update interval in milliseconds (default 1000ms)
  RC522RFID(uint8_t rfid_addr, uint8_t rst_pin, uint32_t update_interval_ms = 1000)
      : PollingComponent(update_interval_ms), rfid_addr_(rfid_addr), rst_pin_(rst_pin) {}

  // Pointer to the text sensor used to publish the UID string.
  text_sensor::TextSensor *rfid_uid_text_sensor = new text_sensor::TextSensor();

  void setup() override;
  void update() override;

 protected:
  uint8_t rfid_addr_;
  uint8_t rst_pin_;
  MFRC522_I2C *mfrc522_;
};

}  // namespace rc522_rfid
}  // namespace esphome
