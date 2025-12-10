# 📖 Documentation Technique - KawaPet

## Table des Matières

1. [Architecture Matérielle](#architecture-matérielle)
2. [Configuration de l'Écran](#configuration-de-lécran)
3. [Correspondance des Pins](#correspondance-des-pins)
4. [Bus de Communication](#bus-de-communication)
5. [Gestion de l'Alimentation](#gestion-de-lalimentation)
6. [Bibliothèque Graphique](#bibliothèque-graphique)
7. [Résolution des Problèmes](#résolution-des-problèmes)

---

## Architecture Matérielle

### Vue d'Ensemble

```
┌─────────────────────────────────────────────────────────────┐
│                        KawaPet                               │
│                                                              │
│  ┌──────────────┐    SPI     ┌──────────────────────────┐   │
│  │              │◄──────────►│     Round Display        │   │
│  │    XIAO      │            │      GC9A01 240×240      │   │
│  │  ESP32-S3    │            │    + Touch CST816S       │   │
│  │              │    I2C     │    + RTC BM8563          │   │
│  │  (8MB Flash) │◄──────────►│    + Charge TP4056       │   │
│  │  (8MB PSRAM) │            └──────────────────────────┘   │
│  │              │                      │                     │
│  │              │    I2C     ┌─────────┴─────────┐          │
│  │              │◄──────────►│    GY-BMI160      │          │
│  │              │            │  (Accel + Gyro)   │          │
│  │              │            └───────────────────┘          │
│  │              │    I2C     ┌───────────────────┐          │
│  │              │◄──────────►│    Trill Bar      │          │
│  │              │            │ (Capteur tactile) │          │
│  │              │            └───────────────────┘          │
│  │              │    PWM     ┌───────────────────┐          │
│  │              │───────────►│    Servo FT90B    │          │
│  └──────────────┘            │  (Tête rotative)  │          │
│                              └───────────────────┘          │
└─────────────────────────────────────────────────────────────┘
```

### Composants Détaillés

#### XIAO ESP32-S3

| Spécification | Valeur |
|---------------|--------|
| MCU | ESP32-S3R8 (Xtensa LX7 dual-core) |
| Fréquence | 240 MHz |
| Flash | 8 MB |
| PSRAM | 8 MB (OPI) |
| RAM | 512 KB SRAM |
| WiFi | 2.4 GHz 802.11 b/g/n |
| Bluetooth | BLE 5.0 |
| Tension de fonctionnement | 3.3V |
| Tension d'entrée | 5V (USB-C) |
| Dimensions | 21 × 17.8 mm |

#### Round Display for XIAO

| Spécification | Valeur |
|---------------|--------|
| Contrôleur LCD | GC9A01 |
| Résolution | 240 × 240 pixels |
| Taille | 1.28" (32.4 mm) diamètre |
| Interface | SPI (4-wire) |
| Couleurs | 65K (RGB565) |
| Contrôleur tactile | CST816S (I2C) |
| RTC | BM8563 (I2C) |
| Chip de charge | Intégré |
| Connecteur batterie | JST 1.25mm |

---

## Configuration de l'Écran

### Bibliothèque Recommandée : Arduino_GFX

Après de nombreux tests, **Arduino_GFX** (GFX Library for Arduino) s'est révélée être la meilleure solution pour le XIAO ESP32-S3 + Round Display :

**Avantages :**
- Support DMA natif → Pas de balayage visible
- Fonctionne sans modifier TFT_eSPI existant
- API simple et bien documentée
- Support natif du GC9A01

**Alternatives testées :**

| Bibliothèque | Résultat | Problème |
|--------------|----------|----------|
| TFT_eSPI (standard) | ❌ Crash | Conflit de configuration, alias Dx non définis |
| Seeed_GFX | ⚠️ Complexe | Nécessite de supprimer TFT_eSPI, driver.h obligatoire |
| SPI brut | ⚠️ Balayage | Pas de DMA, transfert visible |
| **Arduino_GFX** | ✅ Parfait | Fonctionne immédiatement |

### Initialisation de l'Écran

```cpp
#include <Arduino_GFX_Library.h>

// Pins GPIO (pas les alias Dx !)
#define TFT_CS    2   // D1 = GPIO2
#define TFT_DC    4   // D3 = GPIO4
#define TFT_MOSI  9   // D10 = GPIO9
#define TFT_SCLK  7   // D8 = GPIO7
#define TFT_RST   -1  // Non connecté (reset par le XIAO)

// Créer le bus SPI sur HSPI
Arduino_DataBus *bus = new Arduino_ESP32SPI(
  TFT_DC,    // DC
  TFT_CS,    // CS
  TFT_SCLK,  // SCK
  TFT_MOSI,  // MOSI
  -1,        // MISO (non utilisé)
  HSPI       // Utiliser HSPI (obligatoire pour S3)
);

// Créer l'objet display
Arduino_GFX *gfx = new Arduino_GC9A01(
  bus,
  TFT_RST,   // Reset
  0,         // Rotation (0-3)
  true       // IPS = true
);

void setup() {
  if (!gfx->begin()) {
    // Erreur d'initialisation
    while(1);
  }
  gfx->fillScreen(BLACK);
}
```

### Fonction de Texte Centré

Arduino_GFX n'a pas de `drawCentreString()`. Utiliser cette fonction :

```cpp
#define CENTER_X 120
#define CENTER_Y 120

void drawCenteredText(const char* text, int16_t y, uint8_t textSize, uint16_t color) {
  gfx->setTextSize(textSize);
  gfx->setTextColor(color);
  
  int16_t x1, y1;
  uint16_t textWidth, textHeight;
  gfx->getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);
  
  int16_t x = CENTER_X - (textWidth / 2);
  int16_t adjustedY = y - (textHeight / 2);
  
  gfx->setCursor(x, adjustedY);
  gfx->print(text);
}

// Version centrée horizontalement et verticalement
void drawCenteredText(const char* text, uint8_t textSize, uint16_t color) {
  drawCenteredText(text, CENTER_Y, textSize, color);
}
```

---

## Correspondance des Pins

### XIAO ESP32-S3 Pinout

```
                 ┌─────────────────┐
                 │    USB-C        │
                 │   ┌───────┐     │
            D0 ──┤ 1 │       │ 14 ├── 5V
            D1 ──┤ 2 │       │ 13 ├── GND
            D2 ──┤ 3 │       │ 12 ├── 3V3
            D3 ──┤ 4 │       │ 11 ├── D10
            D4 ──┤ 5 │       │ 10 ├── D9
            D5 ──┤ 6 │       │  9 ├── D8
            D6 ──┤ 7 │       │  8 ├── D7
                 │   └───────┘     │
                 └─────────────────┘
```

### Correspondance Alias → GPIO

| Alias Arduino | GPIO ESP32-S3 | Fonction KawaPet |
|---------------|---------------|------------------|
| D0 | GPIO1 | Servo PWM |
| **D1** | **GPIO2** | **TFT_CS** |
| D2 | GPIO3 | (Libre) |
| **D3** | **GPIO4** | **TFT_DC** |
| **D4** | **GPIO5** | **I2C SDA** |
| **D5** | **GPIO6** | **I2C SCL** |
| D6 | GPIO43 | (Libre) |
| D7 | GPIO44 | (Libre) |
| **D8** | **GPIO7** | **TFT_SCLK** |
| D9 | GPIO8 | (Libre) |
| **D10** | **GPIO9** | **TFT_MOSI** |

### ⚠️ Important : Alias non définis au compile-time

Les alias `D1`, `D3`, etc. ne sont **pas disponibles** dans les `#define` car ils sont définis après `#include <Arduino.h>`.

**❌ Ne fonctionne PAS :**
```cpp
#define TFT_CS D1  // Erreur: 'D1' was not declared
```

**✅ Correct :**
```cpp
#define TFT_CS 2   // GPIO2 (équivalent de D1)
```

---

## Bus de Communication

### SPI (Écran)

| Signal | GPIO | Description |
|--------|------|-------------|
| CS | GPIO2 | Chip Select |
| DC | GPIO4 | Data/Command |
| SCLK | GPIO7 | Clock (jusqu'à 80MHz) |
| MOSI | GPIO9 | Data Out |
| MISO | - | Non utilisé |

**Fréquence SPI :** 40-80 MHz (testé stable à 40MHz)

### I2C (Capteurs)

| Signal | GPIO | Description |
|--------|------|-------------|
| SDA | GPIO5 (D4) | Data |
| SCL | GPIO6 (D5) | Clock |

**Périphériques I2C :**

| Périphérique | Adresse | Fonction |
|--------------|---------|----------|
| CST816S | 0x15 | Touch |
| BM8563 | 0x51 | RTC |
| GY-BMI160 | 0x68 ou 0x69 | IMU |
| Trill Bar | 0x20-0x28 | Tactile capacitif |

### PWM (Servo)

| Signal | GPIO | Description |
|--------|------|-------------|
| PWM | GPIO1 (D0) | Signal servo |

**Configuration servo :**
```cpp
#include <ESP32Servo.h>

Servo headServo;
#define SERVO_PIN 1  // GPIO1 = D0

void setup() {
  headServo.attach(SERVO_PIN, 500, 2500);
  headServo.write(90);  // Position neutre
}
```

---

## Gestion de l'Alimentation

### Architecture d'Alimentation

```
                              ┌─────────────┐
  USB-C ──────────────────────┤    XIAO     │
    │                         │  ESP32-S3   ├──── 3.3V (interne)
    │                         └──────┬──────┘
    │                                │
    │    ┌───────────────────────────┘
    │    │
    │    ▼
    │  ┌─────────────────────────────┐
    │  │       Round Display         │
    │  │                             │
    └──┤  TP4056 (Charge) ◄── USB    │
       │         │                   │
       │         ▼                   │
       │  ┌─────────────┐            │
       │  │   LiPo      │ JST 1.25   │
       │  │  3.7V       │◄───────────┤
       │  │  3000mAh    │            │
       │  └─────────────┘            │
       │         │                   │
       │         ▼                   │
       │  Régulateur 3.3V ──────────►│ Alim Display + XIAO
       │                             │
       └─────────────────────────────┘
```

### Points Clés

1. **Une seule batterie** connectée au Round Display (pas au XIAO directement)
2. **Recharge automatique** via USB-C du XIAO
3. **Protection BMS** intégrée à la batterie
4. **Tension LiPo** : 4.2V (plein) → 3.0V (vide)

### Consommation par Composant

| Composant | Mode actif | Mode veille |
|-----------|------------|-------------|
| XIAO ESP32-S3 | 80 mA | 14 µA (deep sleep) |
| Round Display | 35 mA | ~5 mA (backlight off) |
| GY-BMI160 | 0.9 mA | 3 µA |
| Trill Bar | 10 mA | ~1 mA |
| Servo FT90B | 150 mA (mouvement) | 10 mA (repos) |
| **Total (actif)** | **~135 mA** | - |

---

## Bibliothèque Graphique

### Couleurs RGB565

```cpp
#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define YELLOW  0xFFE0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define ORANGE  0xFD20
#define PINK    0xFE19
#define GRAY    0x8410
```

### Conversion RGB → RGB565

```cpp
uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
```

### Fonctions Graphiques Utiles

```cpp
// Cercle plein
gfx->fillCircle(x, y, radius, color);

// Cercle vide
gfx->drawCircle(x, y, radius, color);

// Rectangle
gfx->fillRect(x, y, width, height, color);
gfx->drawRect(x, y, width, height, color);

// Ligne
gfx->drawLine(x1, y1, x2, y2, color);

// Triangle
gfx->fillTriangle(x1, y1, x2, y2, x3, y3, color);

// Arc (pour les sourcils, bouches)
gfx->drawArc(x, y, r1, r2, startAngle, endAngle, color);

// Texte
gfx->setTextColor(color);
gfx->setTextSize(size);  // 1-4
gfx->setCursor(x, y);
gfx->print("Texte");
```

---

## Résolution des Problèmes

### L'écran ne s'allume pas

1. ✅ Vérifier que l'interrupteur du Round Display est sur **ON**
2. ✅ Vérifier que le XIAO est bien enfoncé (USB-C vers l'extérieur)
3. ✅ Vérifier la sélection de carte : **XIAO_ESP32S3**
4. ✅ Vérifier Flash Size : **8MB**

### Crash au démarrage (StoreProhibited)

1. ✅ Utiliser les **GPIO réels** (2, 4, 7, 9) pas les alias (D1, D3...)
2. ✅ Utiliser **HSPI** dans la config SPI
3. ✅ Mettre `IPS = true` dans le constructeur GC9A01

### Balayage visible lors des changements de couleur

- Normal avec du SPI brut (pas de DMA)
- ✅ Utiliser **Arduino_GFX** qui supporte le DMA

### Conflits avec TFT_eSPI existant

- Arduino_GFX fonctionne **sans** TFT_eSPI
- Pas besoin de renommer ou supprimer TFT_eSPI

### Erreur de compilation "Dx not declared"

```cpp
// ❌ Erreur
#define TFT_CS D1

// ✅ Solution
#define TFT_CS 2  // GPIO2
```

---

## Annexes

### Configuration Arduino IDE Complète

```
Carte               : XIAO_ESP32S3
USB CDC On Boot     : Enabled
CPU Frequency       : 240MHz (WiFi)
Core Debug Level    : None
USB DFU On Boot     : Disabled
Erase All Flash     : Disabled
Events Run On       : Core 1
Flash Mode          : QIO 80MHz
Flash Size          : 8MB (64Mb)
JTAG Adapter        : Disabled
Arduino Runs On     : Core 1
USB Firmware MSC    : Disabled
Partition Scheme    : Default 4MB with spiffs
PSRAM               : OPI PSRAM
Upload Mode         : UART0 / Hardware CDC
Upload Speed        : 921600
USB Mode            : Hardware CDC and JTAG
```

### Ressources

- [Seeed XIAO ESP32S3 Wiki](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/)
- [Round Display Wiki](https://wiki.seeedstudio.com/get_start_round_display/)
- [Arduino_GFX GitHub](https://github.com/moononournation/Arduino_GFX)
- [Trill Documentation](https://learn.bela.io/products/trill/)
- [BMI160 Datasheet](https://www.bosch-sensortec.com/products/motion-sensors/imus/bmi160/)
