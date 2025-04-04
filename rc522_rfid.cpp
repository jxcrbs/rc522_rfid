#include "rc522_rfid.h"
#include "esphome/core/log.h"

namespace esphome {
namespace rc522_rfid {

static const char *TAG = "rc522_rfid";

void RC522RFID::setup() {
  // Initialize the MFRC522_I2C instance.
  mfrc522_ = new MFRC522_I2C(rfid_addr_, rst_pin_);
  mfrc522_->PCD_Init();
  ESP_LOGI(TAG, "RFID reader initialized on I2C address 0x%02X", rfid_addr_);
}

void RC522RFID::update() {
  // Check if a new card is present.
  if (!mfrc522_->PICC_IsNewCardPresent()) {
    return;
  }
  // Attempt to read the card serial.
  if (!mfrc522_->PICC_ReadCardSerial()) {
    return;
  }
  
  // Build a UID string.
  char uid_str[32] = {0};
  int index = 0;
  for (byte i = 0; i < mfrc522_->uid.size; i++) {
    index += snprintf(uid_str + index, sizeof(uid_str) - index, "%02X", mfrc522_->uid.uidByte[i]);
    if (i < mfrc522_->uid.size - 1) {
      index += snprintf(uid_str + index, sizeof(uid_str) - index, ":");
    }
  }
  ESP_LOGD(TAG, "Card UID: %s", uid_str);
  
  // Publish the UID to the text sensor.
  rfid_uid_text_sensor->publish_state(String(uid_str));
  
  // Halt the current card.
  mfrc522_->PICC_HaltA();
}

}  // namespace rc522_rfid
}  // namespace esphome
