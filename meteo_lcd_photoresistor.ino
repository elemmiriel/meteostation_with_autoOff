#include <LiquidCrystal.h>
#include <DHT.h>
#include "Adafruit_CCS811.h"

#define DHTPIN 4 // номер пина, к которому подсоединен датчик

#define PIN_PHOTO_SENSOR A0 //пин фоторезистора
int backLight = 2; // цифровой пин ардуино, к которому подключен 15й пин дисплея, контролирует подсветку

//Анализатор воздуха CCS811
Adafruit_CCS811 ccs;

byte Celsum[8] =
{
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
};

byte charP[8] =
{
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
};

byte charL[8] =
{
  B00011,
  B00101,
  B00101,
  B01001,
  B01001,
  B10001,
  B10001,
};

byte charG[8] =
{
  B10001,
  B10101,
  B01110,
  B00100,
  B01110,
  B10101,
  B10001
};

byte percent[8] =
{
  B00000,
  B11001,
  B10010,
  B00100,
  B01001,
  B10011,
  B00000,
};

int co2 = 0;
int tvoc = 0;

// Инициируем датчик
DHT dht(DHTPIN, DHT11);
LiquidCrystal lcd(12, 11, 9, 8, 7, 6); /// REGISTER SELECT PIN,ENABLE PIN,D4 PIN,D5 PIN, D6 PIN, D7 PIN

void setup() {                
  Serial.begin(9600);
  dht.begin();
  ccs.begin();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH);
  lcd.createChar(1, Celsum);
  lcd.createChar(2, charP);
  lcd.createChar(3, charL);
  lcd.createChar(4, charG);
  lcd.createChar(5, percent);
}
void loop() {
  //получаем освещённость для дисплея
  int val = analogRead(PIN_PHOTO_SENSOR);
  Serial.println(val);
  if (val < 750) {
    digitalWrite(backLight, HIGH); //вкл
  } else {
    digitalWrite(backLight, LOW); //выкл
  }
  
  //получаем показания;
  int tem = getTemperature();   
  int hum = getHumidity();
  getCO();
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  
  lcd.setCursor(0, 0);
  lcd.print("TEM");
  lcd.setCursor(3, 0);
  lcd.print("\2");
  lcd.setCursor(4, 0);
  lcd.print("EPATYPA");
  lcd.setCursor(12, 0);
  lcd.print(tem);
  lcd.setCursor(14, 0);
  lcd.print("\1");
  lcd.setCursor(15, 0);
  lcd.print("C");
  
  lcd.setCursor(0, 1);
  lcd.print("B");
  lcd.setCursor(1, 1);
  lcd.print("\3");
  lcd.setCursor(2, 1);
  lcd.print("A");
  lcd.setCursor(3, 1);
  lcd.print("\4");
  lcd.setCursor(4, 1);
  lcd.print("HOCTb");
  lcd.setCursor(12, 1);
  lcd.print(hum);
  lcd.setCursor(15, 1);
  lcd.print("\5");
  delay (3000);
  lcd.clear();
  
  // CO2 and TVOC
  lcd.setCursor(0, 0);
  lcd.print("CO2");
  lcd.setCursor(6, 0);
  lcd.print(co2);
  lcd.setCursor(12, 0);
  lcd.print("PPM");

  lcd.setCursor(0, 1);
  lcd.print("TVOC");
  lcd.setCursor(6, 1);
  lcd.print(tvoc);
  lcd.setCursor(12, 1);
  lcd.print("PPB");
  delay (3000);
  lcd.clear();
}

float getHumidity(){
  //Считываем влажность
  float h = dht.readHumidity();
  
  // Проверка удачно прошло ли считывание.
  if (isnan(h)) {
   Serial.println("Error! Cant read");
   return 0;
  }
  
  //Сообщение в консоль
  Serial.print(" Hum: ");
  Serial.print(h);
  Serial.println();

  return h;
}

float getTemperature(){
 // Считываем температуру
  float t = dht.readTemperature();

  // Проверка удачно прошло ли считывание.
  if (isnan(t)) {
   Serial.println("Error! Cant read");
   return 0;
  }
  
  //Сообщение в консоль
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" C");

  return t;
}

void getCO(){
  // если пришли новые данные
  if(ccs.available()) {
    // считываем данные
    if(!ccs.readData()) {
      Serial.print("c");
      co2 = ccs.geteCO2();
      Serial.print(co2);
      Serial.print(" ");

      
      Serial.print("o");
      tvoc = ccs.getTVOC();
      Serial.print(tvoc);
      Serial.println();
    } else {
      Serial.print("c00 o00");
      Serial.println();
    }
  } else {
      Serial.print("c00 o00");
      Serial.println();
  }
  
}
