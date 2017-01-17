#include "amDecagonDlogger.h"
#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}
RTC_DS1307 rtc;

void checkHour(){
   DateTime d = rtc.now();
   uint8_t h=d.hour();
   if(h==23){
    Serial.println("resetando..");
    Reset_AVR();
    }
  }
void initSensor() {
  SDISerial connection(9, 1);
  connection.begin();
  Serial.println("Iniciando Sistema"); // startup string echo'd to our uart
  delay(3000);
  Serial.println(connection.sdi_query("0I!", 1000));
}
void getSensorData() {
  checkHour();
  SDISerial connection(9, 1);
  char output_buffer[255], nomearquivo[20], dados[1][20];
  connection.begin();
  DateTime d = rtc.now();
  connection.sdi_query("0M!", 1000);
  connection.wait_for_response(1000);
  char* resposta = connection.sdi_query("0D0!", 1000);
  strcpy(dados[0], resposta);
  Serial.print("Sensor 0:");
  Serial.println(resposta);
  sprintf(nomearquivo, "%02d%02d%02d.CSV", d.day(), d.month(), d.hour());
  File arquivo = SD.open(nomearquivo, FILE_WRITE);
  if (!arquivo) {
    Serial.print("Problema no Arquivo");
    Serial.println(nomearquivo);
    Reset_AVR()
  }
  else {
    for (uint8_t x = 0; x < 1; x++) {
      for (uint8_t y = 0; y < 20; y++) {
        if (dados[x][y] == '+') dados[x][y] = ';';
        if (dados[x][y] == '-') dados[x][y] = ';';
        if (dados[x][y] == '.') dados[x][y] = ',';
        if (dados[x][y] == '\r') dados[x][y] = dados[x][y + 1];
      }
    }
    sprintf(output_buffer,"%02d/%02d/%i;%02d:%02d:%02d;%s;\r\n",d.day(),d.month(),d.year(),d.hour(),d.minute(),d.second(),dados[0]);
    Serial.println("dados cartão!");
    Serial.println(output_buffer);
    arquivo.print(output_buffer);
    arquivo.close();
  }
}

void setup() {
  
  Serial.begin(115200); // start our uart
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
  #ifdef AVR
  Wire.begin();
  #else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
  #endif
  rtc.begin();
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  Serial.print("Testando Cartao...");
  pinMode(10, OUTPUT);
  if (!SD.begin(10)) {
    Serial.println("Falha no Cartao..");
    return;
  }
  initSensor();
}
void loop() {
  getSensorData();
  delay(1000);
}

