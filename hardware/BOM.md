# 🛒 Bill of Materials (BOM) - KawaPet

## Composants Principaux

| Réf | Composant | Description | Qté | Prix estimé | Où acheter |
|-----|-----------|-------------|-----|-------------|------------|
| U1 | Seeed XIAO ESP32-S3 | MCU dual-core 240MHz, 8MB Flash, WiFi/BT | 1 | 8-12€ | [Seeed Studio](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html), [Mouser](https://www.mouser.fr/) |
| D1 | Round Display for XIAO | Écran rond 1.28" GC9A01, 240×240, tactile | 1 | 18-25€ | [Seeed Studio](https://www.seeedstudio.com/Seeed-Studio-Round-Display-for-XIAO-p-5638.html) |
| U2 | GY-BMI160 | Module IMU 6 axes (accéléromètre + gyroscope) | 1 | 5-10€ | Amazon, AliExpress |
| U3 | Trill Bar | Capteur tactile capacitif I2C (26 canaux) | 1 | 18-22€ | [Bela Shop](https://shop.bela.io/products/trill-bar) |
| U3-ALT | Trill Flex | Alternative flexible au Trill Bar | 1 | 15-18€ | [Bela Shop](https://shop.bela.io/products/trill-flex) |
| M1 | Servo FT90B | Micro servo digital 3-4.8V, 90° | 1 | 5-8€ | [Gotronic](https://www.gotronic.fr/), Amazon |
| BAT1 | Batterie LiPo 3.7V | 3000mAh avec BMS, connecteur JST 1.25mm | 1 | 10-15€ | Amazon, AliExpress |

## Connectique et Câbles

| Réf | Composant | Description | Qté | Prix estimé |
|-----|-----------|-------------|-----|-------------|
| C1 | Câbles Dupont femelle-femelle | Pour connexions I2C et servo | 10 | 2-3€ |
| C2 | Connecteur JST 1.25mm | Si besoin d'adapter la batterie | 2 | 1-2€ |
| C3 | Câble USB-C | Pour programmation et recharge | 1 | 3-5€ |

## Optionnel

| Réf | Composant | Description | Qté | Prix estimé |
|-----|-----------|-------------|-----|-------------|
| BAT2 | Batterie LiPo 10000mAh | Pour autonomie prolongée (~75h) | 1 | 20-30€ |
| ENC1 | Boîtier imprimé 3D | Fichiers STL fournis dans /hardware/enclosure | 1 | Variable |

## Total Estimé

| Configuration | Prix |
|---------------|------|
| Minimale (sans Trill, sans servo) | ~40-55€ |
| Standard (avec Trill Bar) | ~65-95€ |
| Complète (Trill + Servo + grosse batterie) | ~85-130€ |

---

## Notes d'Achat

### ⚠️ Points d'Attention

1. **XIAO ESP32-S3** : Bien vérifier qu'il s'agit du S3, pas du C3 ou C6
   - Le S3 a 8MB de Flash et PSRAM
   - Nécessaire pour les animations fluides

2. **Round Display** : Doit être le modèle "for XIAO"
   - Se branche directement sur le XIAO
   - Inclut le chip de charge et connecteur batterie

3. **Batterie** : 
   - Connecteur **JST 1.25mm** (pas 2.0mm PH)
   - Doit avoir une **protection BMS** intégrée
   - Polarité à vérifier avant connexion

4. **Servo FT90B** : 
   - Fonctionne de 3V à 4.8V (compatible LiPo directe)
   - Ne pas utiliser un SG90 standard (5V minimum)

5. **Trill** :
   - Le **Trill Bar** est plus rigide (idéal pour "dos" du pet)
   - Le **Trill Flex** est flexible mais moins précis

### 🔗 Liens Utiles

- [Seeed Studio XIAO ESP32-S3](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html)
- [Round Display for XIAO](https://www.seeedstudio.com/Seeed-Studio-Round-Display-for-XIAO-p-5638.html)
- [Trill by Bela](https://bela.io/products/trill/)
- [BMI160 Breakout Boards (Amazon)](https://www.amazon.fr/s?k=GY-BMI160)
