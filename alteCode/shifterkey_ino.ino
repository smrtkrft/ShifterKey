#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

// OLED ekran ayarları
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// I2C pinleri
#define SDA_PIN 4
#define SCL_PIN 5

// Buton pinleri
#define BUTTON_RIGHT 13
#define BUTTON_LEFT 14
#define BUTTON_OK 12

// PN532 ayarları
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

void setup() {
  // Seri haberleşme başlat
  Serial.begin(115200);

  // I2C başlat
  Wire.begin(SDA_PIN, SCL_PIN);

  // OLED ekran başlat
  if (!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println(F("OLED ekran başlatılamadı!"));
    for (;;);
  }

  // Ekranı temizle ve açılış animasyonunu başlat
  display.clearDisplay();
  displayOpeningAnimation();

  // PN532 başlat
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println(F("PN532 başlatılamadı!"));
    while (1); // Hata durumunda dur
  }
  nfc.SAMConfig();

  // Buton pinlerini giriş olarak ayarla
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_OK, INPUT_PULLUP);
}

void loop() {
  // Ana menü
  displayMainMenu();
}

void displayOpeningAnimation() {
  // Açılış animasyonu
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor((SCREEN_WIDTH - 12 * 8) / 2, SCREEN_HEIGHT / 4);
  display.println(F("ShifterKey"));
  display.display();
  delay(1000);

  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - 10 * 6) / 2, SCREEN_HEIGHT / 2);
  display.println(F("SmartKraft"));
  display.display();
  delay(1000);
}

void displayMainMenu() {
  // Ana menü
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("1. Read"));
  display.println(F("2. Copy"));
  display.println(F("3. New UID"));
  display.println(F("4. Saved"));
  display.println(F("5. Setting"));
  display.display();
}

// RFID okuma işlevi
void readRFID() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Reading RFID..."));
  display.display();

  uint8_t success;
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("RFID UID:"));
    for (uint8_t i = 0; i < uidLength; i++) {
      display.print(uid[i], HEX);
      display.print(F(" "));
    }
    display.display();
    delay(2000);
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("RFID Okunamadı!"));
    display.display();
    delay(2000);
  }
}

// Menü navigasyonu
void navigateMenu() {
  int selectedOption = 1;
  while (true) {
    displayMainMenu();
    if (digitalRead(BUTTON_RIGHT) == LOW) {
      selectedOption++;
      if (selectedOption > 5) selectedOption = 1;
      delay(200);
    }
    if (digitalRead(BUTTON_LEFT) == LOW) {
      selectedOption--;
      if (selectedOption < 1) selectedOption = 5;
      delay(200);
    }
    if (digitalRead(BUTTON_OK) == LOW) {
      executeOption(selectedOption);
      break;
    }
  }
}

// Seçilen menü işlevini çalıştır
void executeOption(int option) {
  switch (option) {
    case 1:
      readRFID();
      break;
    case 2:
      // Kopyalama işlevi eklenecek
      break;
    case 3:
      // Yeni UID oluşturma işlevi eklenecek
      break;
    case 4:
      // Kaydedilen UID'ler işlevi eklenecek
      break;
    case 5:
      // Ayarlar işlevi eklenecek
      break;
  }
}