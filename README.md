# Fotopu-apka-ESP32-CAM
Fotopułapka ESP32-CAM z czujnikiem obecności LD2410C

Schemat Połączeń:
Komponenty:
ESP32-CAM
Czujnik obecności LD2410C
Tranzystor NPN (np. 2N2222)
Bateria 18650
Rezystory (np. 1kΩ)
Moduł ładowania baterii 18650 (np. TP4056)
Karta microSD i czytnik kart SD
Połączenia:
ESP32-CAM:

VCC -> Kolektor tranzystora NPN
GND -> GND
GPIO 12 (PWR_PIN) -> Baza tranzystora NPN przez rezystor (1kΩ)
GPIO 13 (MOTION_PIN) -> Wyjście sygnału z czujnika LD2410C
SD Card Pins:
GPIO 14 (CS)
GPIO 15 (SCK)
GPIO 2 (MOSI)
GPIO 4 (MISO)
Czujnik LD2410C:

VCC -> VCC
GND -> GND
Wyjście sygnału -> GPIO 13 (MOTION_PIN) ESP32-CAM
Tranzystor NPN (np. 2N2222):

Emiter -> GND
Kolektor -> VCC ESP32-CAM
Baza -> GPIO 12 (PWR_PIN) ESP32-CAM przez rezystor (1kΩ)
Bateria 18650:

Plus -> VCC ESP32-CAM (przez moduł ładowania TP4056)
Minus -> GND
Moduł ładowania TP4056:

B+ -> Plus Baterii 18650
B- -> Minus Baterii 18650
OUT+ -> VCC ESP32-CAM
OUT- -> GND

Bateria 18650
  +----------------+
  |                |
  +---(+)--------->+ VCC ESP32-CAM
  |                |
  |                +-------> Kolektor tranzystora NPN
  |                |
  |                +-(B+)->
  |                | TP4056
  |                +-(B-)->
  |                |
  +---(-)---------> GND ESP32-CAM
                   |
                   +-------> Emiter tranzystora NPN
                   |
                   +-------> GND czujnika LD2410C
                   |
                   +-------> GND ESP32-CAM
                   |
                   |
VCC czujnika LD2410C --> VCC
Wyjście sygnału czujnika LD2410C --> GPIO 13 (MOTION_PIN) ESP32-CAM

ESP32-CAM:
  GPIO 12 (PWR_PIN) --> Baza tranzystora NPN przez rezystor (1kΩ)
  GPIO 13 (MOTION_PIN) --> Wyjście sygnału czujnika LD2410C
  GPIO 14 (CS) --> CS SD Card
  GPIO 15 (SCK) --> SCK SD Card
  GPIO 2 (MOSI) --> MOSI SD Card
  GPIO 4 (MISO) --> MISO SD Card

