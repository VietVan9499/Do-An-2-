ok4#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <DHT.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTPIN 10 // CHÂN SỐ MƯỜI CẢM BIẾN DHT22
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
#define LedX 16 // BẬT TẮT KHI TRỜI SÁNG VÀ TỐI
#define LedV 14 //RÚT ĐỒ  
#define LedD 12 //PHƠI ĐỒ RA 
#define LedSay 15 // SẤY ĐỒ 
#define Buttons 3 // CÀI ĐẶT THỜI GIAN SẤY
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "MsIXuYvu-TJBZ6ZcAZhLmA9DtjL28Wvs";
//char ssid[] = "CAFE HOANG ANH";
//char pass[] = "";
char ssid[] = "Do An 2";
char pass[] = "123456789";
int virtualPinR;
int virtualPinV;
int virtualPinD;
int virtualPinSay;
int virtualPinAutoSay;
int flagR = 0;
int flagV = 0;
int flagD = 0;
int flagSay = 0;
int flagAutoSay = 0;
int count = 0;
int check = 0;
int check1 = 0;
int check2 = 0;
int CheckSay = 0;
int CheckAutoSay = 0;
// WidgetLED led1(V1);
struct PhoiDo {
  int AnhSang;
  int Mua;
  int Button;
};
//Gửi dữ liệu trạng thái Button từ Blynk về Nodemcu
BLYNK_WRITE(V0)
{
  virtualPinR = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V0 Slider value is: ");
  Serial.println(virtualPinR);
  flagR = 1;
}
BLYNK_WRITE(V1)
{
  virtualPinV = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V1 Slider value is: ");
  Serial.println(virtualPinV);
  flagV = 1;
}
BLYNK_WRITE(V2)
{
  virtualPinD = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V2 Slider value is: ");
  Serial.println(virtualPinD);
  flagD = 1;
}
BLYNK_WRITE(V5)
{
  virtualPinSay = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V5 Slider value is: ");
  Serial.println(virtualPinSay);
  flagSay = 1;
}
BLYNK_WRITE(V6)
{
  virtualPinAutoSay = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V6 Slider value is: ");
  Serial.println(virtualPinAutoSay);
  flagAutoSay = 1;
}
//Đồng bộ lại trạng thái khi mất nguồn
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V5);
}



void PhoiRut ()
{
  struct PhoiDo Read;
  Read.AnhSang = digitalRead(5); // CẢM BIẾN ÁNH SÁNG
  Read.Mua     = digitalRead(4); // CẢM BIẾN MƯA
  Read.Button  = digitalRead(13);// NÚT NHẤN PHƠI VÀ RÚT
  // TRỜI SÁNG
  if (  ( (Read.AnhSang == LOW) && (Read.Mua == HIGH) && (Read.Button == HIGH)  ) && (check == 0)) {
    delay(50);
    digitalWrite(LedD, HIGH);
    delay (1700);
    digitalWrite(LedD, LOW);
    if (Read.AnhSang == LOW ) {
      if (check2 == 1) {
        digitalWrite(LedX, LOW);
        check2 = 0;
      }
      Blynk.virtualWrite(V2, 0); //Ghi dữ liệu từ Nodemcu lên Blynk
      Blynk.notify("TRỜI ĐÃ SÁNG PHƠI QUẦN ÁO");
    }
    if (Read.Mua == HIGH ) {
      Blynk.notify("TRỜI ĐÃ HẾT MƯA PHƠI QUẦN ÁO");
    }
    Blynk.virtualWrite(V0, 1); //Ghi dữ liệu từ Nodemcu lên Blynk
    Blynk.virtualWrite(V1, 0); //Ghi dữ liệu từ Nodemcu lên Blynk
    check = 1;
  }
  //Mưa và tối
  if ( (  (Read.AnhSang == HIGH) || (Read.Mua == LOW)  || (Read.Button == LOW) ) && (check == 1) ) {
    delay(50);
    digitalWrite(LedV, HIGH);
    delay(100);
    digitalWrite(LedV, LOW);
    delay(100);
    digitalWrite(LedV, HIGH);
    delay(100);
    digitalWrite(LedV, LOW);
    delay(100);
    digitalWrite(LedV, HIGH);
    delay(100);
    digitalWrite(LedV, LOW);
    delay(100);
    digitalWrite(LedV, HIGH);
    delay(70);
    digitalWrite(LedV, LOW);
    delay(100);
    digitalWrite(LedV, HIGH);
    delay(20);
    digitalWrite(LedV, LOW);
    if (Read.AnhSang == HIGH ) {
      if (check2 == 0) {
        digitalWrite(LedX, HIGH);
        check2 = 1;
      }
      Blynk.virtualWrite(V2, 1); //Ghi dữ liệu từ Nodemcu lên Blynk
      Blynk.notify("TRỜI ĐÃ TỐI RÚT QUẦN ÁO");
    }
    if (Read.Mua == LOW ) {
      count++;
      Blynk.virtualWrite(V5, count);
      if (count == 200) {
        count = 0;
      }
      Blynk.notify("TRỜI ĐÃ MƯA RÚT QUẦN ÁO");
    }
    Blynk.virtualWrite(V1, 1); //Ghi dữ liệu từ Nodemcu lên Blynk
    Blynk.virtualWrite(V0, 0); //Ghi dữ liệu từ Nodemcu lên Blynk
    check = 0;
  }
}
///////////////////////////////////
void Auto()
{
  float h = dht.readHumidity();
  if ((digitalRead(Buttons) == LOW) && (CheckSay == 0)) {
    lcd.setCursor(10, 0);
    lcd.print("CHE DO");
    lcd.setCursor(10, 1);
    lcd.print(" AUTO");
    if (h < 40) {
      Serial.println("AUTO 1");
      for (short int i = 5 ; i  >  -1 ; i-- ) {
        Blynk.virtualWrite(V8, i );
        Blynk.virtualWrite(V9, i );
        digitalWrite(LedSay, HIGH);
        delay(1000); // TƯƠNG ĐƯƠNG SẤY 10 PHÚT
      }
    }
    else if (40 < h < 60) {
      Serial.println("AUTO 2");
      for (short int i = 7 ; i  >  -1 ; i-- ) {
        Blynk.virtualWrite(V8, i );
        Blynk.virtualWrite(V9, i );
        digitalWrite(LedSay, HIGH);
        delay(1000); // TƯƠNG ĐƯƠNG SẤY 10 PHÚT
      }
    }
    else if (60 < h < 80) {
      Serial.println("AUTO 3");
      for (short int i = 10 ; i  >  -1 ; i-- ) {
        Blynk.virtualWrite(V8, i );
        Blynk.virtualWrite(V9, i );
        digitalWrite(LedSay, HIGH);
        delay(1000); // TƯƠNG ĐƯƠNG SẤY 10 PHÚT
      }
    }
    else if (80 < h < 100) {
      Serial.println("AUTO 4");
      for (short int i = 15 ; i  >  -1 ; i-- ) {
        Blynk.virtualWrite(V8, i );
        Blynk.virtualWrite(V9, i );
        digitalWrite(LedSay, HIGH);
        delay(1000); // TƯƠNG ĐƯƠNG SẤY 10 PHÚT
      }
    }
    else {
      digitalWrite(LedSay, LOW);
    }
    digitalWrite(LedSay, LOW);
    lcd.clear();
    CheckSay = 1;
  } else {
    if (digitalRead(Buttons) == HIGH) {
      CheckSay = 0;
    }
  }
}
//////////////////////////////////

void Say()
{
  int h = dht.readHumidity();
  if (digitalRead(Buttons) == HIGH) {
    switch (virtualPinSay)
    {
      case 1: { // Item 1
          Serial.println("Item 1 selected");
          Blynk.notify("ĐANG SẤY 15 PHÚT");
          for (short int i = 15 ; i  >  -1 ; i-- ) {
            Blynk.virtualWrite(V7, i );
            Blynk.virtualWrite(V9, i );
            lcd.setCursor(10, 0);
            lcd.print("TIME:");
            digitalWrite(LedSay, HIGH);
            lcd.setCursor(10, 1);
            lcd.print(i);
            lcd.setCursor(12, 1);
            lcd.print("M...");
            if (i < 10) {
              lcd.setCursor(10, 1);
              lcd.print("0");
              lcd.setCursor(11, 1);
              lcd.print(i);
              lcd.setCursor(12, 1);
              lcd.print("M...");
            }
            delay(1000);
          }
          digitalWrite(LedSay, LOW);
          Blynk.notify("ĐÃ SẤY XONG");
          lcd.clear();
          break;
        }
      case 2: { // Item 1
          Serial.println("Item 2 selected");
          Blynk.notify("ĐANG SẤY 30 PHÚT");
          for (short int i = 30 ; i  >  -1 ; i-- ) {
            Blynk.virtualWrite(V7, i );
            Blynk.virtualWrite(V9, i );
            lcd.setCursor(10, 0);
            lcd.print("TIME:");
            digitalWrite(LedSay, HIGH);
            lcd.setCursor(10, 1);
            lcd.print(i);
            lcd.setCursor(12, 1);
            lcd.print("M...");
            if (i < 10) {
              lcd.setCursor(10, 1);
              lcd.print("0");
              lcd.setCursor(11, 1);
              lcd.print(i);
              lcd.setCursor(12, 1);
              lcd.print("M...");
            }
            delay(1000);
          }
          digitalWrite(LedSay, LOW);
          Blynk.notify("ĐÃ SẤY XONG");
          lcd.clear();
          break;
        }
      case 3: { // Item 1
          Blynk.notify("ĐANG SẤY 60 PHÚT");
          for (short int i = 60 ; i  >  -1 ; i-- ) {
            Blynk.virtualWrite(V7, i );
            Blynk.virtualWrite(V9, i );
            if (virtualPinSay == 5) {
              digitalWrite(LedSay, LOW);
            }
            lcd.setCursor(10, 0);
            lcd.print("TIME:");
            digitalWrite(LedSay, HIGH);
            lcd.setCursor(10, 1);
            lcd.print(i);
            lcd.setCursor(12, 1);
            lcd.print("M...");
            if (i < 10) {
              lcd.setCursor(10, 1);
              lcd.print("0");
              lcd.setCursor(11, 1);
              lcd.print(i);
              lcd.setCursor(12, 1);
              lcd.print("M...");
            }
            delay(1000);
          }
          digitalWrite(LedSay, LOW);
          Blynk.notify("ĐÃ SẤY XONG");
          lcd.clear();
          break;
        }
      case 4: { // Item 1
          Blynk.notify("ĐANG SẤY 90 PHÚT");
          for (short int i = 90 ; i  >  -1 ; i-- ) {
            Blynk.virtualWrite(V7, i );
            Blynk.virtualWrite(V9, i );
            lcd.setCursor(10, 0);
            lcd.print("TIME:");
            digitalWrite(LedSay, HIGH);
            lcd.setCursor(10, 1);
            lcd.print(i);
            lcd.setCursor(12, 1);
            lcd.print("M...");
            if (i < 10) {
              lcd.setCursor(10, 1);
              lcd.print("0");
              lcd.setCursor(11, 1);
              lcd.print(i);
              lcd.setCursor(12, 1);
              lcd.print("M...");
            }
            delay(1000);
          }
          digitalWrite(LedSay, LOW);
          Blynk.notify("ĐÃ SẤY XONG");
          lcd.clear();
          break;
        }
      case 5:
        {
          lcd.setCursor(10, 0);
          lcd.print("CHE DO");
          lcd.setCursor(10, 1);
          lcd.print(" AUTO");
          if (h < 40) {
            Serial.println("AUTO 1");
            for (short int i = 5 ; i  >  -1 ; i-- ) {
              Blynk.virtualWrite(V8, i );
              Blynk.virtualWrite(V9, i );
              digitalWrite(LedSay, HIGH);
              delay(1000); // TƯƠNG ĐƯƠNG SẤY 10 PHÚT
            }
          }
          else if (40 < h < 60) {
            Serial.println("AUTO 2");
            for (short int i = 7 ; i  >  -1 ; i-- ) {
              Blynk.virtualWrite(V8, i );
              Blynk.virtualWrite(V9, i );
              digitalWrite(LedSay, HIGH);
              delay(1000); // TƯƠNG ĐƯƠNG SẤY 10 PHÚT
            }
          }
          else if (60 < h < 80) {
            Serial.println("AUTO 3");
            for (short int i = 10 ; i  >  -1 ; i-- ) {
              Blynk.virtualWrite(V8, i );
              Blynk.virtualWrite(V9, i );
              digitalWrite(LedSay, HIGH);
              delay(1000); // TƯƠNG ĐƯƠNG SẤY 10 PHÚT
            }
          }
          else if (80 < h < 100) {
            Serial.println("AUTO 4");
            for (short int i = 15 ; i  >  -1 ; i-- ) {
              Blynk.virtualWrite(V8, i );
              Blynk.virtualWrite(V9, i );
              digitalWrite(LedSay, HIGH);
              delay(1000); // TƯƠNG ĐƯƠNG SẤY 10 PHÚT
            }
          }
          digitalWrite(LedSay, LOW);
          lcd.clear();
          break;
        }
      case 6:
        {
          digitalWrite(LedSay, LOW);
        }
    }
  }
}
void BlynkApp ()
{
  if (flagR == 1) {
    if (virtualPinR == 1 && check1 == 0) {
      digitalWrite(LedD, HIGH);
      delay(1700);
      digitalWrite(LedD, LOW);
      Blynk.notify("ĐÃ PHƠI QUẦN ÁO");
      Blynk.virtualWrite(V1, 0); //Ghi dữ liệu từ Nodemcu lên Blynk
      check1 = 1;
    }
  }

  if (flagV == 1) {
    if (virtualPinV == 1 && check1 == 1) {
    digitalWrite(LedV, HIGH);
    delay(100);
    digitalWrite(LedV, LOW);
    delay(100);
    digitalWrite(LedV, HIGH);
    delay(100);
    digitalWrite(LedV, LOW);
    delay(100);
    digitalWrite(LedV, HIGH);
    delay(100);
    digitalWrite(LedV, LOW);
    delay(100);
    digitalWrite(LedV, HIGH);
    delay(70);
    digitalWrite(LedV, LOW);
    delay(100);
    digitalWrite(LedV, HIGH);
    delay(20);
    digitalWrite(LedV, LOW);
      Blynk.notify("ĐÃ RÚT QUẦN ÁO");
      Blynk.virtualWrite(V0, 0); //Ghi dữ liệu từ Nodemcu lên Blynk
      check1 = 0;
    }
  }
  if (flagD == 1) {
    if (virtualPinD == 1 && check2 == 0) {
      digitalWrite(LedX, HIGH);
      Blynk.notify("ĐÃ BẬT ĐÈN");
      check2 = 1;
    }
    if (virtualPinD == 0 && check2 == 1)
    {
      digitalWrite(LedX, LOW);
      Blynk.notify("ĐÃ TẮT ĐÈN");
      check2 = 0;
    }
  }

}

void setup()
{
  Serial.begin(9600);
  Wire.begin(2, 0); // SDA SCL
  dht.begin();
  lcd.begin();
  lcd.home();
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 1, 100), 8080);
  pinMode(5, INPUT);
  pinMode(4, INPUT);
  pinMode(13, INPUT_PULLUP);
  pinMode(Buttons, INPUT_PULLUP);
  pinMode(LedX, OUTPUT);
  pinMode(LedV, OUTPUT);
  pinMode(LedD, OUTPUT);
  pinMode(LedSay, OUTPUT);
}
void NhietDo()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Blynk.virtualWrite(V3, t ); //Nhiệt độ với pin V10
  Blynk.virtualWrite(V4, h); // Độ ẩm với pin V11
  Serial.println(t);
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);
  lcd.print((char)0xDF);
  lcd.print("C|");
  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(h);
  lcd.print("% |");
  delay(500);

}
void loop()
{
  Serial.println("OUT");
  Blynk.run();
  Say();
  NhietDo();
  PhoiRut ();
  Auto();
  BlynkApp();
  virtualPinSay = 0;
  flagR = 0;
  flagV = 0;
  flagD = 0;
}
