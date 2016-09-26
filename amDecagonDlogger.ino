#include "amDecagonDlogger.h"
RTC_DS1307 rtc;

void initSensor(){

    char output_buffer[255];
    SDISerial connection(DATALINE_PIN, INVERTED);
    connection.begin();
    Serial.println("Iniciando Sistema"); // startup string echo'd to our uart
    delay(3000); 
    char* sensor_info = connection.sdi_query("0I!",1000); // get sensor info
    sprintf(output_buffer,"Info Sensor: %s",sensor_info?sensor_info:"Sem resposta");
    Serial.println(output_buffer);
    delay(100);
    sensor_info = connection.sdi_query("1I!",1000); // get sensor info
    sprintf(output_buffer,"Info Sensor: %s",sensor_info?sensor_info:"Sem resposta");
    Serial.println(output_buffer);
    delay(100);
    delay(500);
  
  }
void saveOnSd(char output_buffer[], char nomearquivo[], char dados[1][20]){

	DateTime now = rtc.now();
	File arquivo = SD.open(nomearquivo, FILE_WRITE);
	if (!arquivo){
		Serial.print("Problema no Arquivo");
		Serial.println(nomearquivo);
		while(1);
	}
	else {
		for (int x=0;x<1;x++){
			for (int y=0;y<20;y++){
				if(dados[x][y]=='+') dados[x][y]=';';
				if(dados[x][y]=='-') dados[x][y]=';';
				if(dados[x][y]=='.') dados[x][y]=',';
				if(dados[x][y]=='\r') dados[x][y]=dados[x][y+1];
			}
		}
		sprintf(output_buffer,"%i/%02d/%02d;%02d:%02d:%02d;%s;\r\n",now.day(),now.month(),now.year(),now.month(),now.hour(),now.minute(),now.second(),dados[0]);
    Serial.println("dados cartão!");
    Serial.println(output_buffer);
		arquivo.print(output_buffer);
		arquivo.close();
	}

}

void getSensorData(){

	char output_buffer[255],nomearquivo[20],dados[1][20];
	SDISerial connection(DATALINE_PIN, INVERTED);
	connection.begin();
	DateTime now = rtc.now();
	connection.sdi_query("1M!",1000);
	connection.wait_for_response(1000);
	char* resposta = connection.sdi_query("1D0!",1000);
	strcpy(dados[0],resposta);
	Serial.print("Sensor 0:");
	Serial.println(resposta);
	sprintf(nomearquivo,"%02d%02d%02d.CSV",now.month(),now.day(),now.hour());
	saveOnSd(output_buffer, nomearquivo, dados);

}

void setup(){
    Serial.begin(115200); // start our uart
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }
    Serial.print("Testando Cartao...");
    pinMode(CS, OUTPUT);
    if (!SD.begin(CS)) {
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
void loop(){

    getSensorData();
    delay(500);
}
