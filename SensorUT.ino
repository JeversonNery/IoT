#define BLYNK_TEMPLATE_ID "..."
#define BLYNK_TEMPLATE_NAME "..."
#define BLYNK_AUTH_TOKEN "..."

#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h> // Adicione a biblioteca do Blynk

#define DHTPIN A2     // Pino digital do sensor DHT
#define DHTTYPE DHT11 // Tipo de sensor DHT11

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Substitua esses valores pelo seu endereço MAC real
IPAddress ip(192, 168, x, xxx); // Endereço IP do Arduino na rede local
IPAddress gateway(192, 168, x, X); // Gateway da rede
IPAddress subnet(255, 255, 255, 0); // Máscara de sub-rede

DHT dht(DHTPIN, DHTTYPE); // Inicializar o sensor DHT11

// Definir o endereço do LCD para 0x27 para um display de 16 caracteres e 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx teste!"));

  dht.begin();
  
  Ethernet.init(10); // Inicializar a interface Ethernet com o pino CS 10
  Ethernet.begin(mac, ip, gateway, gateway, subnet);

  lcd.begin(16, 2); // Inicializar o LCD apenas uma vez na função setup()
  Blynk.begin(BLYNK_AUTH_TOKEN); // Iniciar a comunicação com o Blynk

}

void loop() {
  Blynk.run(); // Executar as operações do Blynk

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Falha de leitura do sensor DHT!"));
    return;
  }

  Serial.print(F("Umidade: "));
  Serial.print(h);
  Serial.print(F("%  Temperatura: "));
  Serial.print(t);
  Serial.println(F("°C "));

  Blynk.virtualWrite(V0, h); // Enviar umidade para o widget V0 no aplicativo Blynk
  Blynk.virtualWrite(V1, t); // Enviar temperatura para o widget V1 no aplicativo Blynk

  lcd.setBacklight(HIGH);

  lcd.setCursor(0, 0);
  lcd.print(F("Humidade: "));
  lcd.setCursor(10, 0);
  lcd.print(round(h));
  lcd.setCursor(12, 0);
  lcd.print(F(" %"));
  delay(3000);

  lcd.setCursor(0, 1);
  lcd.print(F("Tempo: "));
  lcd.setCursor(7, 1);
  lcd.print(round(t));
  lcd.setCursor(9, 1);
  lcd.write(32);  // Caracter espaço
  lcd.write(223); // Caracter °
  lcd.print(F("C"));
  delay(3000);
}
