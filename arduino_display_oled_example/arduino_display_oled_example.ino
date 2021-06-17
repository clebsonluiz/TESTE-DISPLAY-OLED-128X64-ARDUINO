/**
 * Autor: Clébson Luiz 
 * Descrição: Projeto muito simples para monitoramento temperatura e humidade do ar com DHT11
 * e da temperatura do DS18B20 (Sensor a prova de d'agua), mostrando as informações no display
 * oled SSD1306 128x64 0,96" I2C (Azul e Amarelo). Para isso foi usada a biblioteca u8glib
 **/
 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <U8glib.h>

#define DHT11_PINO A0
#define DS18B20_PINO 3

//Sensor DS18B20
OneWire pino(DS18B20_PINO);
DallasTemperature ds18b20(&pino);
DeviceAddress sensor;

//Sensor DHT11
DHT dht(DHT11_PINO, DHT11);

//Display OLED SSD1306 128x64 pixels
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC

void setup(void) 
{
//  Serial.begin(9600);
 
  // Configurações inicias do display
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  } 
  
  dht.begin();
  
  ds18b20.begin();
  ds18b20.getAddress(sensor, 0); 
}
 
void loop(void) 
{
  u8g.firstPage(); // Colocando o ponteiro de desenho no inicio da tela do display
  do
  {
    draw_dht(); // Desenha informações do DHT11
  } while( u8g.nextPage() );
  u8g.firstPage(); // Colocando o ponteiro de desenho no inicio da tela do display
  delay(3000);
  ds18b20.requestTemperatures(); // Atualiza informações de temperatura
  do
  {
    draw_ds18(); // Desenha informações do DS18B20
  } while( u8g.nextPage() );
  delay(3000);
}

/**
  * Função usada para para estilizar uma string de temperatura do DHT
  */
String dht_medicao(String msg, float valor, String escala, int casas_decimais)
{
  if (isnan(valor)) return msg + "N/D";
  return msg + String(valor, casas_decimais) + escala;
}

/**
  * Função usada para desenhar no corpo principal azul do display
  */
void draw_temperatura_box(String msg, float valor, String escala){
  if (isnan(valor)) valor = 0.0; 
  u8g.setFont(u8g_font_5x7);
  u8g.drawStr( 8, 30, msg.c_str());
  u8g.setFont(u8g_font_fub20);
  u8g.drawStr( 10, 57, String(valor, 1).c_str());
  u8g.setFont(u8g_font_6x10);
  if(valor < 0)
    u8g.drawCircle( 75, 38, 3);
  else
    u8g.drawCircle( 70, 38, 3);
  u8g.drawStr( 75, 57, escala.c_str());
  u8g.drawRFrame(0,18, 128, 46, 4);
}

/**
  * Função usada para desenhar no topo amarelo do display
  */
void draw_topo_msg(String msg){
  u8g.setFont(u8g_font_6x13);
  u8g.drawStr( 5, 15, msg.c_str());
}

/**
  * Função usada para desenhar as informações de temperatura e humidade do DHT11 no display
  */
void draw_dht() 
{
  draw_topo_msg(dht_medicao("Umidade do AR: ", dht.readHumidity(), "%", 0));
  draw_temperatura_box("S. TEMPERATURA DHT11:", dht.readTemperature(), "Celsius");
}
/**
  * Função usada para desenhar as informaões de temperatura do sensor a prova d'agua DS18B20 no display
  */
void draw_ds18() 
{ 
  draw_temperatura_box("S. TEMPERATURA DS18B20:", ds18b20.getTempC(sensor), "Celsius");
}
