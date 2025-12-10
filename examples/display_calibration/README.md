# 🎯 Display Calibration Test

Test de calibration pour vérifier le bon fonctionnement de l'écran Round Display avec le XIAO ESP32-S3.

## Description

Ce sketch affiche un pattern de calibration avec :

- **Cercles concentriques** colorés (blanc → bleu → cyan → vert → jaune → orange → rouge)
- **Croix de centrage** grise
- **4 points magenta** aux bords cardinaux
- **Point blanc** au centre exact

Puis alterne avec des écrans de couleurs pleines pour vérifier :
- L'absence de balayage visible
- Le centrage du texte
- La qualité des couleurs

## Matériel Requis

- Seeed XIAO ESP32-S3
- Round Display for XIAO (GC9A01, 240×240)

## Installation

1. **Bibliothèque requise** : Installer "GFX Library for Arduino" par moononournation
   - Arduino IDE → Outils → Gérer les bibliothèques
   - Rechercher "GFX Library for Arduino"
   - Installer

2. **Configuration Arduino IDE** :
   ```
   Carte       : XIAO_ESP32S3
   Flash Size  : 8MB (64Mb)
   PSRAM       : OPI PSRAM
   ```

3. **Upload** le sketch

## Vérifications

### Pattern de Calibration

✅ **Correct** si :
- Le cercle rouge est au centre exact
- Les 4 points magenta sont équidistants des bords
- La croix passe par le centre du cercle rouge
- Tous les cercles sont concentriques

❌ **Problème** si :
- Les cercles sont décalés → Vérifier les paramètres de rotation
- Des pixels manquent sur un bord → Vérifier l'offset d'affichage

### Test de Couleurs

✅ **Correct** si :
- Les couleurs changent instantanément (pas de balayage)
- Le texte est centré horizontalement et verticalement
- Les couleurs sont vives et fidèles

❌ **Problème** si :
- Balayage visible → La bibliothèque n'utilise pas le DMA
- Couleurs inversées → Modifier le paramètre IPS du constructeur

## Sortie Série

Le sketch affiche les informations de debug sur le port série (115200 bauds) :

```
============================================
   KawaPet - Display Calibration Test
============================================

Hardware: XIAO ESP32-S3 + Round Display
Display:  GC9A01 240x240 (IPS)

Initialisation ecran... OK!

Affichage du pattern de calibration...

Verifier:
  - Cercles bien centres
  - 4 points magenta visibles aux bords
  - Croix passant par le centre rouge
  - Point blanc au centre exact

>>> CALIBRATION <<<
>>> ROUGE <<<
>>> VERT <<<
...
```

## Fonction drawCenteredText

Ce sketch inclut une fonction utilitaire pour centrer le texte :

```cpp
void drawCenteredText(const char* text, int16_t y, uint8_t textSize, uint16_t color);
```

Cette fonction utilise `getTextBounds()` pour calculer automatiquement la position X permettant de centrer le texte horizontalement, et ajuste Y pour centrer verticalement.

## Capture d'Écran

```
      ╭────────────────────╮
     ╱  ●                   ╲    ● = Point magenta (haut)
    │    ○ ○ ○ ○ ○ ●        │    ○ = Cercles concentriques
    │  ○               ○    │    + = Croix de centrage
   ● │○      ───+───     ○│ ●    ● = Centre rouge
    │  ○       │       ○    │
    │    ○ ○ ○ ○ ○          │
     ╲                  ●  ╱
      ╰────────────────────╯
```

## Licence

MIT - Voir le fichier LICENSE à la racine du projet.
