#include "amDecagonDlogger.h"
#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}
RTC_DS1307 rtc;
void initSensor() {
  SDISerial connection(9, 1);
  connection.begin();
  Serial.println("Iniciando Sistema"); // startup string echo'd to our uart
  delay(3000);
  Serial.println(connection.sdi_query("1I!", 1000));
}
void getSensorData() {
  SDISerial connection(9, 1);
  char output_buffer[255], nomearquivo[20], dados[1][20];
  connection.begin();
  DateTime now = rtc.now();
  connection.sdi_query("1M!", 1000);
  connection.wait_for_response(1000);
  char* resposta = connection.sdi_query("1D0!", 1000);
  strcpy(dados[0], resposta);
  Serial.print("Sensor 0:");
  Serial.println(resposta);
  sprintf(nomearquivo, "%02d%02d%02d.CSV", now.day(), now.month(), now.hour());
  File arquivo = SD.open(nomearquivo, FILE_WRITE);
  if (!arquivo) {
    Serial.print("Problema no Arquivo");
    Serial.println(nomearquivo);
    while (1);
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
    sprintf(output_buffer,"%02d/%02d/%i;%02d:%02d:%02d;%s;\r\n",now.day(),now.month(),now.year(),now.hour(),now.minute(),now.second(),dados[0]);
    Serial.println("dados cartão!");
    Serial.println(output_buffer);
    arquivo.print(output_buffer);
    arquivo.close();
    wdt_reset();
  }
 

}

void setup() {
  Serial.begin(115200); // start our uart
  Serial.print("Testando Cartao...");
  pinMode(10, OUTPUT);
  if (!SD.begin(10)) {
    Serial.println("Falha no Cartao..");
    return;
  }
  Wire.begin();
  if (! rtc.isrunning()) {
    Serial.println("Testando RTC");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  initSensor();
}
void loop() {

  getSensorData();
  delay(1000);
}

