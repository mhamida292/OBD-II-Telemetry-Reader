// ESP32 + SN65HVD230
// OBD-II Fuel Level (Mode 01, PID 0x2F)
// Pins: TX=GPIO22, RX=GPIO23
// Connect to OBD-II: CANH=pin 6, CANL=pin 14, GND=pin 4/5

#include "driver/twai.h"

#define RX_PIN 23
#define TX_PIN 22

void setup() {
  Serial.begin(115200);

  // TWAI init: NORMAL mode @ 500 kbps
  twai_general_config_t g =
      TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_NORMAL);
  twai_timing_config_t  t = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t  f = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  if (twai_driver_install(&g, &t, &f) != ESP_OK || twai_start() != ESP_OK) {
    Serial.println("TWAI init failed");
    while (1) {}
  }

  Serial.println("OBD-II Fuel Level (PID 0x2F) reader started...");
}

void loop() {
  // Build functional request frame: ID 0x7DF, data = 02 01 2F 00 00 00 00 00
  twai_message_t tx = {};
  tx.identifier = 0x7DF;
  tx.extd = 0; tx.rtr = 0; tx.data_length_code = 8;
  tx.data[0] = 0x02;   // 2 more bytes (01, 2F)
  tx.data[1] = 0x01;   // Mode 01: current data
  tx.data[2] = 0x2F;   // PID 0x2F = Fuel Level
  for (int i = 3; i < 8; i++) tx.data[i] = 0;

  if (twai_transmit(&tx, pdMS_TO_TICKS(100)) != ESP_OK) {
    Serial.println("TX failed");
  }

  // Wait up to 800 ms for reply
  bool got = false;
  uint32_t t0 = millis();
  while (millis() - t0 < 800) {
    twai_message_t rx;
    if (twai_receive(&rx, pdMS_TO_TICKS(50)) == ESP_OK) {
      if (!rx.extd && !rx.rtr &&
          rx.identifier >= 0x7E8 && rx.identifier <= 0x7EF &&
          rx.data_length_code >= 4 &&
          rx.data[1] == 0x41 && rx.data[2] == 0x2F) {

        uint8_t raw = rx.data[3];              // single byte
        float fuel_pct = (raw * 100.0f) / 255.0f;
        Serial.printf("Fuel Level: %.1f%% (raw=0x%02X from ID 0x%03lX)\n",
                      fuel_pct, raw, rx.identifier);
        got = true;
        break;
      }
    }
  }

  if (!got) {
    Serial.println("No reply for Fuel Level (PID 0x2F) — may not be supported.");
  }

  delay(2000); // poll every 2s
}
