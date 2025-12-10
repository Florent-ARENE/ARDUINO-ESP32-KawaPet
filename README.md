# 🐾 KawaPet - Compagnon Électronique Interactif

<p align="center">
  <img src="docs/images/banner.png" alt="KawaPet Banner" width="600">
</p>

<p align="center">
  <strong>Un animal de compagnie virtuel avec 20 expressions animées, réactif au toucher et au mouvement</strong>
</p>

<p align="center">
  <a href="#-caractéristiques">Caractéristiques</a> •
  <a href="#-matériel">Matériel</a> •
  <a href="#-installation">Installation</a> •
  <a href="#-expressions">Expressions</a> •
  <a href="#-documentation">Documentation</a>
</p>

<p align="center">
  <a href="https://github.com/Florent-ARENE/ARDUINO-ESP32-KawaPet">
    <img src="https://img.shields.io/github/stars/Florent-ARENE/ARDUINO-ESP32-KawaPet?style=social" alt="GitHub Stars">
  </a>
  <a href="https://github.com/Florent-ARENE/ARDUINO-ESP32-KawaPet/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/Florent-ARENE/ARDUINO-ESP32-KawaPet" alt="License">
  </a>
</p>

---

## ✨ Caractéristiques

- **🎭 20 expressions faciales** avec animations fluides
- **👆 Détection tactile capacitive** - Caresses sur le dos (Trill Bar/Flex)
- **📱 Écran tactile rond** - Interactions sur le visage (1.28" 240×240)
- **🔄 Capteur 6 axes** - Réactions selon l'orientation et les mouvements
- **🤖 Servo moteur** - Mouvements de tête expressifs
- **🔋 Batterie LiPo** - Portable et rechargeable via USB-C
- **⚡ Faible consommation** - ~130mA, autonomie ~20h avec 3000mAh

---

## 🛠 Matériel

### Composants Principaux

| Composant | Description | Rôle |
|-----------|-------------|------|
| **Seeed XIAO ESP32-S3** | MCU dual-core 240MHz, 8MB Flash, WiFi/BT | Cerveau |
| **Round Display for XIAO** | Écran rond 1.28" GC9A01, 240×240, tactile | Visage |
| **GY-BMI160** | IMU 6 axes (accéléromètre + gyroscope) | Orientation |
| **Trill Bar / Flex** | Capteur tactile capacitif I2C | Caresses (dos) |
| **Servo FT90B** | Micro servo 3-4.8V, 90° | Mouvements tête |
| **Batterie LiPo 3.7V** | 3000mAh avec BMS, JST 1.25mm | Alimentation |

### Schéma de Connexion

```
                    ┌─────────────────┐
                    │  XIAO ESP32-S3  │
                    │                 │
    ┌───────────────┤ GPIO5 (D4/SDA)  │◄────┬──── Trill Bar (I2C)
    │               │ GPIO6 (D5/SCL)  │◄────┤     GY-BMI160 (I2C)
    │               │                 │     │
    │  Round        │ GPIO2 (D1/CS)   │     │
    │  Display      │ GPIO4 (D3/DC)   │     │
    │  (SPI)        │ GPIO7 (D8/SCLK) │     │
    │               │ GPIO9 (D10/MOSI)│     │
    │               │                 │     │
    │               │ GPIO1 (D0/PWM)  │────────── Servo FT90B
    │               └─────────────────┘
    │                       │
    │    ┌──────────────────┘
    │    │ USB-C
    │    ▼
    │  ┌───┐
    └──┤BAT├──── LiPo 3.7V 3000mAh (JST 1.25)
       └───┘
       Round Display
```

---

## 📦 Installation

### 1. Prérequis

- [Arduino IDE 2.x](https://www.arduino.cc/en/software)
- Support ESP32 pour Arduino

### 2. Configuration Arduino IDE

**Ajouter l'URL du gestionnaire de cartes :**
```
https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json
```

**Installer la carte :**
- Outils → Gestionnaire de cartes → Rechercher "esp32" → Installer "esp32 by Espressif"
- Sélectionner : **XIAO_ESP32S3**

### 3. Bibliothèques Requises

Installer via le Gestionnaire de bibliothèques :

| Bibliothèque | Auteur | Usage |
|--------------|--------|-------|
| **GFX Library for Arduino** | moononournation | Écran GC9A01 |
| **Trill Arduino Library** | Bela | Capteur tactile |
| **BMI160-Arduino** | hanyazou | IMU 6 axes |
| **ESP32Servo** | Kevin Harrington | Servo moteur |

### 4. Configuration de la Carte

```
Type de carte    : XIAO_ESP32S3
USB CDC On Boot  : Enabled
Flash Size       : 8MB (64Mb)
PSRAM            : OPI PSRAM
```

### 5. Premier Test

1. Cloner ce repository
2. Ouvrir `examples/display_calibration/display_calibration.ino`
3. Upload et vérifier que l'écran affiche le pattern de calibration

---

## 🎭 Expressions

### Les 20 Expressions

| # | Nom | Déclencheur | Animation |
|---|-----|-------------|-----------|
| 0 | NEUTRAL | État par défaut | Clignement périodique |
| 1 | HAPPY | Caresse douce | Yeux souriants + joues |
| 2 | SAD | Inactivité prolongée | Sourcils bas + larme |
| 3 | SLEEPING | 35s d'inactivité | Yeux fermés + ZZZ |
| 4 | BORED | Pas d'interaction | Paupières mi-closes |
| 5 | CURIOUS | Penché sur le côté | Sourcil levé + "?" |
| 6 | DIZZY | Retourné | Spirales tournantes |
| 7 | SURPRISED | Soulevé rapidement | Grands yeux + "O" |
| 8 | SICK | À l'envers longtemps | Teint vert + sueur |
| 9 | LOVE | Bercé doucement | Cœurs pulsants |
| 10 | ANGRY | Secoué fort | Sourcils V + veine |
| 11 | PLEASED | Caresse simple | Yeux fermés plaisir |
| 12 | EXCITED | Appuis rapides | Étoiles + rebond |
| 13 | KISS | Appui long doux | Bouche bisou + cœur |
| 14 | ANNOYED | Trop d'appuis | Regard agacé |
| 15 | THINKING | Aléatoire | Yeux haut + bulles |
| 16 | WINKING | Aléatoire | Clin d'œil |
| 17 | PLAYFUL | Bonne humeur | Langue tirée |
| 18 | SHY | Premier contact | Regard côté + rougeur |
| 19 | YAWN | Après SLEEPING | Bâillement |

### Déclencheurs IMU

| Orientation | Mouvement | Expression |
|-------------|-----------|------------|
| Penché 30°+ | - | CURIOUS |
| À l'envers | - | DIZZY → SICK |
| Normal | Soulevé vite | SURPRISED |
| Normal | Secoué fort | ANGRY |
| Normal | Bercé doucement | LOVE |

### Déclencheurs Tactiles

| Action | Expression |
|--------|------------|
| Caresse simple | PLEASED |
| Plusieurs caresses | HAPPY |
| Caresse rapide | EXCITED |
| Tapotement | ANNOYED |
| Pas de toucher 35s | SLEEPING |

---

## 📁 Structure du Projet

```
KawaPet/
├── README.md                 # Ce fichier
├── LICENSE
├── docs/
│   ├── TECHNICAL.md          # Documentation technique détaillée
│   ├── WIRING.md             # Schémas de câblage
│   ├── EXPRESSIONS.md        # Détails des expressions
│   └── images/
├── examples/
│   ├── display_calibration/  # Test de calibration écran
│   ├── imu_test/             # Test du capteur IMU
│   ├── trill_test/           # Test du capteur tactile
│   └── servo_test/           # Test du servo
├── src/
│   ├── KawaPet.ino           # Programme principal
│   ├── expressions.h         # Définitions des expressions
│   ├── expressions.cpp       # Animations des expressions
│   ├── sensors.h             # Gestion des capteurs
│   ├── sensors.cpp
│   ├── display.h             # Gestion de l'affichage
│   └── display.cpp
└── hardware/
    ├── BOM.md                # Liste des composants
    └── enclosure/            # Fichiers boîtier 3D
```

---

## 🔋 Autonomie

| Composant | Consommation |
|-----------|--------------|
| XIAO ESP32-S3 | ~80mA |
| Round Display | ~35mA |
| Trill | ~10mA |
| GY-BMI160 | <1mA |
| Servo (repos) | ~10mA |
| **Total** | **~135mA** |

| Batterie | Autonomie |
|----------|-----------|
| 3000mAh | ~22 heures |
| 10000mAh | ~74 heures |

---

## 📚 Documentation

- [📖 Documentation Technique](docs/TECHNICAL.md)
- [🔌 Schémas de Câblage](docs/WIRING.md)
- [🎭 Guide des Expressions](docs/EXPRESSIONS.md)

---

## 🚀 Roadmap

- [x] Test de l'écran Round Display
- [x] Calibration et centrage
- [x] Fonction de texte centré
- [ ] Implémentation des 20 expressions
- [ ] Intégration IMU (GY-BMI160)
- [ ] Intégration Trill Bar
- [ ] Servo pour mouvements de tête
- [ ] Mode démo (appui long 3s)
- [ ] Gestion de la batterie
- [ ] Boîtier imprimé 3D

---

## 🤝 Contribution

Les contributions sont les bienvenues ! 

- 🐛 [Signaler un bug](https://github.com/Florent-ARENE/ARDUINO-ESP32-KawaPet/issues)
- 💡 [Proposer une amélioration](https://github.com/Florent-ARENE/ARDUINO-ESP32-KawaPet/issues)
- 🔀 [Soumettre une Pull Request](https://github.com/Florent-ARENE/ARDUINO-ESP32-KawaPet/pulls)

---

## 📄 Licence

Ce projet est sous licence MIT - voir le fichier [LICENSE](LICENSE) pour plus de détails.

---

## 🙏 Remerciements

- [Seeed Studio](https://www.seeedstudio.com/) pour le XIAO ESP32-S3 et le Round Display
- [Bela](https://bela.io/) pour les capteurs Trill
- [moononournation](https://github.com/moononournation) pour Arduino_GFX
- La communauté Arduino et ESP32

---

<p align="center">
  Made with ❤️ and lots of ☕
</p>
