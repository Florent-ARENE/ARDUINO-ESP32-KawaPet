/*
 * ============================================================================
 * KawaPet - Display Calibration Test
 * ============================================================================
 * 
 * Ce sketch affiche un pattern de calibration pour vérifier que l'écran
 * Round Display est correctement configuré et centré.
 * 
 * Matériel requis:
 *   - Seeed XIAO ESP32-S3
 *   - Round Display for XIAO (GC9A01, 240×240)
 * 
 * Bibliothèques requises:
 *   - GFX Library for Arduino (par moononournation)
 * 
 * Configuration Arduino IDE:
 *   - Carte: XIAO_ESP32S3
 *   - Flash Size: 8MB (64Mb)
 *   - PSRAM: OPI PSRAM
 * 
 * Auteur: KawaPet Project
 * Licence: MIT
 * ============================================================================
 */

#include <Arduino_GFX_Library.h>

// ============================================================================
// CONFIGURATION HARDWARE
// ============================================================================

// Pins GPIO pour le Round Display (utiliser GPIO réels, pas les alias Dx)
#define TFT_CS    2   // D1 = GPIO2
#define TFT_DC    4   // D3 = GPIO4
#define TFT_MOSI  9   // D10 = GPIO9
#define TFT_SCLK  7   // D8 = GPIO7
#define TFT_RST   -1  // Non connecté (reset géré par le XIAO)

// Centre de l'écran 240×240
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 240
#define CENTER_X      (SCREEN_WIDTH / 2)   // 120
#define CENTER_Y      (SCREEN_HEIGHT / 2)  // 120

// ============================================================================
// COULEURS RGB565
// ============================================================================

#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define YELLOW  0xFFE0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define ORANGE  0xFD20
#define GRAY    0x8410

// ============================================================================
// OBJETS DISPLAY
// ============================================================================

// Bus SPI sur HSPI (obligatoire pour ESP32-S3)
Arduino_DataBus *bus = new Arduino_ESP32SPI(
  TFT_DC,    // DC
  TFT_CS,    // CS  
  TFT_SCLK,  // SCK
  TFT_MOSI,  // MOSI
  -1,        // MISO (non utilisé)
  HSPI       // Utiliser HSPI
);

// Display GC9A01 (IPS = true pour couleurs correctes)
Arduino_GFX *gfx = new Arduino_GC9A01(
  bus,
  TFT_RST,   // Reset
  0,         // Rotation (0 = normal)
  true       // IPS panel
);

// ============================================================================
// FONCTIONS UTILITAIRES
// ============================================================================

/**
 * Affiche du texte centré horizontalement et verticalement
 * 
 * @param text     Texte à afficher
 * @param y        Position Y du centre du texte
 * @param textSize Taille du texte (1-4)
 * @param color    Couleur RGB565
 */
void drawCenteredText(const char* text, int16_t y, uint8_t textSize, uint16_t color) {
  gfx->setTextSize(textSize);
  gfx->setTextColor(color);
  
  // Calculer les dimensions du texte
  int16_t x1, y1;
  uint16_t textWidth, textHeight;
  gfx->getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);
  
  // Centrer horizontalement et verticalement
  int16_t x = CENTER_X - (textWidth / 2);
  int16_t adjustedY = y - (textHeight / 2);
  
  gfx->setCursor(x, adjustedY);
  gfx->print(text);
}

/**
 * Version simplifiée - centrée au milieu de l'écran
 */
void drawCenteredText(const char* text, uint8_t textSize, uint16_t color) {
  drawCenteredText(text, CENTER_Y, textSize, color);
}

// ============================================================================
// PATTERNS DE TEST
// ============================================================================

/**
 * Pattern de calibration avec cercles concentriques
 * 
 * Permet de vérifier:
 * - Le centrage de l'affichage
 * - La symétrie du rendu
 * - Les bords de l'écran rond
 */
void drawCalibrationPattern() {
  gfx->fillScreen(BLACK);
  
  // Cercles concentriques (du plus grand au plus petit)
  // Rayon max = 119 pour laisser 1px de marge sur un écran 240×240
  
  gfx->drawCircle(CENTER_X, CENTER_Y, 119, WHITE);   // Bord extérieur
  gfx->drawCircle(CENTER_X, CENTER_Y, 118, WHITE);   // Double trait
  gfx->drawCircle(CENTER_X, CENTER_Y, 100, BLUE);    // Cercle bleu
  gfx->drawCircle(CENTER_X, CENTER_Y, 99, BLUE);
  gfx->drawCircle(CENTER_X, CENTER_Y, 80, CYAN);     // Cercle cyan
  gfx->drawCircle(CENTER_X, CENTER_Y, 60, GREEN);    // Cercle vert
  gfx->drawCircle(CENTER_X, CENTER_Y, 40, YELLOW);   // Cercle jaune
  gfx->drawCircle(CENTER_X, CENTER_Y, 20, ORANGE);   // Cercle orange
  gfx->fillCircle(CENTER_X, CENTER_Y, 10, RED);      // Centre rouge plein
  
  // Croix de centrage
  gfx->drawLine(CENTER_X - 115, CENTER_Y, CENTER_X + 115, CENTER_Y, GRAY);  // Horizontale
  gfx->drawLine(CENTER_X, CENTER_Y - 115, CENTER_X, CENTER_Y + 115, GRAY);  // Verticale
  
  // Point central blanc (vérification précise du centre)
  gfx->fillCircle(CENTER_X, CENTER_Y, 2, WHITE);
  
  // 4 points cardinaux pour vérifier la symétrie
  gfx->fillCircle(CENTER_X, 10, 5, MAGENTA);         // Haut
  gfx->fillCircle(CENTER_X, SCREEN_HEIGHT - 10, 5, MAGENTA);  // Bas
  gfx->fillCircle(10, CENTER_Y, 5, MAGENTA);         // Gauche
  gfx->fillCircle(SCREEN_WIDTH - 10, CENTER_Y, 5, MAGENTA);   // Droite
}

/**
 * Test de remplissage d'écran avec texte centré
 */
void drawColorTest(const char* colorName, uint16_t bgColor, uint16_t textColor) {
  gfx->fillScreen(bgColor);
  gfx->drawCircle(CENTER_X, CENTER_Y, 119, WHITE);
  drawCenteredText(colorName, 3, textColor);
}

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  // Initialiser le port série pour debug
  Serial.begin(115200);
  delay(1000);
  
  Serial.println();
  Serial.println("============================================");
  Serial.println("   KawaPet - Display Calibration Test");
  Serial.println("============================================");
  Serial.println();
  Serial.println("Hardware: XIAO ESP32-S3 + Round Display");
  Serial.println("Display:  GC9A01 240x240 (IPS)");
  Serial.println();

  // Initialiser l'écran
  Serial.print("Initialisation ecran... ");
  if (!gfx->begin()) {
    Serial.println("ERREUR!");
    Serial.println("Verifier les connexions et la configuration.");
    while (1) {
      delay(100);
    }
  }
  Serial.println("OK!");
  
  // Afficher le pattern de calibration
  Serial.println();
  Serial.println("Affichage du pattern de calibration...");
  Serial.println();
  Serial.println("Verifier:");
  Serial.println("  - Cercles bien centres");
  Serial.println("  - 4 points magenta visibles aux bords");
  Serial.println("  - Croix passant par le centre rouge");
  Serial.println("  - Point blanc au centre exact");
  Serial.println();
  
  drawCalibrationPattern();
}

// ============================================================================
// LOOP
// ============================================================================

void loop() {
  static int testNum = 0;
  static unsigned long lastChange = 0;
  const unsigned long interval = 4000;  // 4 secondes par test
  
  if (millis() - lastChange >= interval) {
    lastChange = millis();
    testNum++;
    
    switch (testNum % 6) {
      case 0:
        Serial.println(">>> CALIBRATION <<<");
        drawCalibrationPattern();
        break;
        
      case 1:
        Serial.println(">>> ROUGE <<<");
        drawColorTest("ROUGE", RED, WHITE);
        break;
        
      case 2:
        Serial.println(">>> VERT <<<");
        drawColorTest("VERT", GREEN, BLACK);
        break;
        
      case 3:
        Serial.println(">>> BLEU <<<");
        drawColorTest("BLEU", BLUE, WHITE);
        break;
        
      case 4:
        Serial.println(">>> JAUNE <<<");
        drawColorTest("JAUNE", YELLOW, BLACK);
        break;
        
      case 5:
        Serial.println(">>> CENTRE <<<");
        gfx->fillScreen(BLACK);
        gfx->drawCircle(CENTER_X, CENTER_Y, 119, WHITE);
        drawCenteredText("CENTRE", 3, CYAN);
        drawCenteredText("Parfait!", CENTER_Y + 40, 2, YELLOW);
        break;
    }
  }
}
