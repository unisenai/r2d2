/**
 * @file r2d2_display.c
 * @author Charly Batista (carlbsb@gmail.com)
 * @brief R2D2 is am autonomous guided vehicle (AGV) created as part of the Mechatronics undergraduate program in UniSenai Joinville/Brazil
 * @version 0.1
 * @date 2023-11-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif12pt7b.h>

#include "include/r2d2.h"

Adafruit_SSD1306 display(-1);

// 'R2D2', 128x64px
static const unsigned char PROGMEM epd_bitmap_R2D2[] = {
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8,
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf8,
    0x0f, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xf8,
    0x0f, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xf8,
    0x0f, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xf8, 0x0f, 0xff, 0xf8, 0x07, 0x78, 0x1f, 0xff, 0xf8,
    0x0f, 0xff, 0xf8, 0x04, 0x38, 0x1f, 0xff, 0xf8, 0x0f, 0xff, 0xf0, 0x04, 0x18, 0x0f, 0xff, 0xf8,
    0x0f, 0xff, 0xf0, 0x08, 0x1c, 0x0f, 0xff, 0xf8, 0x0f, 0xff, 0xf0, 0x0c, 0x18, 0x07, 0xff, 0xf8,
    0x0f, 0xff, 0xe0, 0x0c, 0x38, 0x07, 0xff, 0xf8, 0x0f, 0xff, 0xfa, 0xdf, 0xf0, 0xdf, 0xff, 0xf8,
    0x0f, 0xff, 0xfa, 0xc0, 0x00, 0xdf, 0xff, 0xf8, 0x0f, 0xff, 0xfa, 0xd9, 0xe4, 0xdf, 0xff, 0xf8,
    0x0f, 0xff, 0xfa, 0xd9, 0xe4, 0xdf, 0xff, 0xf8, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xf8,
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8,
    0x0f, 0xf8, 0x30, 0x00, 0x00, 0x0c, 0x1f, 0xf8, 0x0f, 0xf8, 0x30, 0xff, 0xff, 0x0c, 0x1f, 0xf8,
    0x0f, 0xf8, 0x30, 0xff, 0xff, 0x0c, 0x1f, 0xf8, 0x0f, 0xf8, 0x30, 0x00, 0x00, 0x0c, 0x1f, 0xf8,
    0x0f, 0xf8, 0x30, 0xff, 0xff, 0x0c, 0x1f, 0xf8, 0x0f, 0xf8, 0x30, 0xff, 0xff, 0x0c, 0x1f, 0xf8,
    0x0f, 0xf8, 0x30, 0xff, 0xff, 0x0c, 0x1f, 0xf8, 0x0f, 0xf8, 0x30, 0x00, 0x00, 0x0c, 0x1f, 0xf8,
    0x0f, 0xf8, 0x30, 0xff, 0xff, 0x0c, 0x1f, 0xf8, 0x0f, 0xf8, 0x30, 0xff, 0xff, 0x0c, 0x1f, 0xf8,
    0x0f, 0xf8, 0x30, 0xff, 0xff, 0x0c, 0x1f, 0xf8, 0x0f, 0xf8, 0x30, 0x00, 0x00, 0x0c, 0x1f, 0xf8,
    0x0f, 0xff, 0xf0, 0x03, 0xc0, 0x0f, 0xff, 0xf8, 0x0f, 0xff, 0xf0, 0x03, 0xc0, 0x0f, 0xff, 0xf8,
    0x0f, 0xfc, 0x30, 0x03, 0xc0, 0x0c, 0x3f, 0xf8, 0x0f, 0xfc, 0x30, 0x03, 0xc0, 0x0c, 0x3f, 0xf8,
    0x0f, 0xfc, 0x30, 0x03, 0xc0, 0x0c, 0x3f, 0xf8, 0x0f, 0xfc, 0x30, 0x03, 0xc0, 0x0c, 0x3f, 0xf8,
    0x0f, 0xfc, 0x30, 0x03, 0xc0, 0x0c, 0x3f, 0xf8, 0x0f, 0xfc, 0x30, 0x03, 0xc0, 0x0c, 0x3f, 0xf8,
    0x0f, 0xfc, 0x30, 0x00, 0x00, 0x0c, 0x3f, 0xf8, 0x0f, 0xfc, 0x30, 0x00, 0x00, 0x0c, 0x3f, 0xf8,
    0x0f, 0xfc, 0x30, 0x00, 0x00, 0x0c, 0x3f, 0xf8, 0x0f, 0xfc, 0x30, 0x00, 0x00, 0x0c, 0x3f, 0xf8,
    0x0f, 0xfc, 0x30, 0x7b, 0xc0, 0x0c, 0x3f, 0xf8, 0x0f, 0xfc, 0x30, 0x7b, 0xc0, 0x0c, 0x3f, 0xf8,
    0x0f, 0xfc, 0x30, 0x7b, 0xc0, 0x0c, 0x3f, 0xf8, 0x0f, 0xfc, 0x30, 0x00, 0x00, 0x0c, 0x3f, 0xf8,
    0x0f, 0xfc, 0x30, 0x00, 0x00, 0x0c, 0x3f, 0xf8, 0x0f, 0xfc, 0x3f, 0xff, 0xff, 0xfc, 0x3f, 0xf8,
    0x0f, 0xfc, 0x3f, 0xff, 0xff, 0xfc, 0x3f, 0xf8, 0x0f, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xf8,
    0x0f, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xf8, 0x0f, 0xfc, 0x3f, 0xf8, 0x1f, 0xfc, 0x1f, 0xf8,
    0x0f, 0xf8, 0x3f, 0xf8, 0x1f, 0xfc, 0x1f, 0xf8, 0x0f, 0xf8, 0x1f, 0xf8, 0x0f, 0xfc, 0x1f, 0xf8,
    0x0f, 0xf8, 0x1f, 0xf0, 0x0f, 0xf8, 0x1f, 0xf8, 0x0f, 0xf8, 0x1f, 0xf0, 0x0f, 0xf8, 0x1f, 0xf8,
    0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf8, 0x0f, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8,
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8};

/*
// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1040)
const int epd_bitmap_allArray_LEN = 1;
const unsigned char *epd_bitmap_allArray[1] = {
    epd_bitmap_R2D2};

void print_logo()
{
  // Inicializa o display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("Erro na inicialização!"));
    while (1)
      ;
  }

  display.clearDisplay();

  // Retângulo ao redor da tela
  display.drawRect(0, 0, display.width() - 1, display.height() - 1, 1);
  // Circulo na parte direita da tela
  display.fillCircle(display.width() - 12, 16, 7, 1);

  // Configura fonte do texto
  display.setTextSize(1);  // Tamanho
  display.setTextColor(1); // Cor
  display.setCursor(4, 8); // Cursor pode ir de 0,0 a 128,32
  display.print("Mundo Projetado");
  display.setCursor(4, 16);
  display.print("Teste");

  // Atualiza todas as mudanças
  display.display();

  // Espera 4 segundos e exibe a imagem
  delay(4000);
  display.clearDisplay();
  display.drawBitmap(64 - 16, 0, epd_bitmap_R2D2, 32, 32, 1);
  display.display();
}

*/

void print_logo()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000); // Pausa por 2 segundos

  // Limpa o display
  display.clearDisplay();

  // Retângulo ao redor da tela
  // display.drawRect(0, 0, display.width() - 1, display.height() - 1, 1);

  // Desenha um bitmap na tela
  // Coordenada X, coordenada Y, nome da imagem, tamanho na coordenada x, tamanho na coordenada y, nome do arquivo bitmap (sempre 1)
  display.drawBitmap(0, 0, epd_bitmap_R2D2, 64, 64, 1);

  // Configura fonte do texto
  r2d2_print(78, 1, 1, 1, false, "R2D2");
  r2d2_print(70, 12, 0.5, 1, false, "ADVANCED");
  r2d2_print(80, 22, 0.5, 1, true, "AGV");
}

/**
 * Mostra a mensagem no display OLED
 * O display tem dimensoes de 128x48
 * @pos_x Posicao horizontal aceitando valores entre 0 e 128
 * @pos_y Posicao vertical aceitando valores entre 0 e 32
 * @size Tamanho da fonte
 * @color Codigo para a cor
 * @render Booleano pra indicar se e para renderizar a mensagem na tela ou apenas "bufferizar"
 * @msg Mensagem de texto a ser exibida no display
 */
void r2d2_print(const uint8_t pos_x, const uint8_t pos_y,
                const uint8_t size, const uint8_t color,
                bool render, char *msg)
{
  // Tamanho
  display.setTextSize(size);

  // Cor
  display.setTextColor(color);

  // Move o cursor pra posicao indicada
  display.setCursor(pos_x, pos_y);

  // Print the msg
  display.print(msg);

  if (render)
    display.display();
}