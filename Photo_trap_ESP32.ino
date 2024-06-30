#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"

#define PWR_PIN 12  // Pin sterujący zasilaniem ESP32-CAM
#define MOTION_PIN 13 // Pin, do którego podłączony jest czujnik LD2410C

volatile bool motionDetected = false;

void IRAM_ATTR handleMotion() {
  motionDetected = true;
}

void setup() {
  pinMode(PWR_PIN, OUTPUT);
  pinMode(MOTION_PIN, INPUT);
  digitalWrite(PWR_PIN, LOW); // ESP32-CAM początkowo wyłączone

  Serial.begin(115200);
  if(!SD_MMC.begin("/sdcard", true)){
    Serial.println("Card Mount Failed");
    return;
  }

  attachInterrupt(digitalPinToInterrupt(MOTION_PIN), handleMotion, RISING);
}

void loop() {
  if (motionDetected) {
    motionDetected = false; // Resetuj flagę
    digitalWrite(PWR_PIN, HIGH); // Włącz ESP32-CAM
    delay(1000); // Czas na stabilizację zasilania

    // Inicjalizacja kamery
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    // Inicjalizacja kamery
    if (esp_camera_init(&config) != ESP_OK) {
      Serial.println("Camera init failed");
      digitalWrite(PWR_PIN, LOW); // Wyłącz ESP32-CAM w przypadku niepowodzenia
      return;
    }

    // Przechwyć obraz
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      esp_camera_deinit();
      digitalWrite(PWR_PIN, LOW); // Wyłącz ESP32-CAM w przypadku niepowodzenia
      return;
    }

    // Zapisz obraz na kartę SD
    fs::FS &fs = SD_MMC;
    File file = fs.open("/photo.jpg", FILE_WRITE);
    if(!file){
      Serial.println("Failed to open file in writing mode");
    } else {
      file.write(fb->buf, fb->len); // zapisz dane obrazu na karcie
      Serial.print("Saved file to path: ");
      Serial.println("/photo.jpg");
      file.close();
    }

    // Zwolnij bufor obrazu
    esp_camera_fb_return(fb);

    // Uśpij ESP32-CAM po zakończeniu pracy
    esp_camera_deinit();
    digitalWrite(PWR_PIN, LOW); // Wyłącz ESP32-CAM
  }
}
