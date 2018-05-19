#include <LiquidCrystal.h>
#include <DHT.h>
#include <DHT_U.h>
#include <IRremote.h>

#define DHTTYPE           DHT11     // DHT 11 
#define DHTPIN            8         // Pin which is connected to the DHT sensor.

int valSensor = 1;
int maxtime = 100;
uint32_t delayMS;
char sep = ' ';
char eof = '\n';
String cur_element = "";
int sig[231];
int sub = 0;

LiquidCrystal lcd(12, 11, 5, 4, 7, 2);
IRsend irsend;
DHT_Unified dht(DHTPIN, DHTTYPE);

void setup_hum() {
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
}

int* get_temp_and_hum(){
  delay(delayMS);
  sensors_event_t event;
  static int r[2]={0};
  dht.temperature().getEvent(&event);
  r[0] = int(event.temperature);
  dht.humidity().getEvent(&event);
  r[1] = int(event.relative_humidity);
  return r;
}
  
void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);     //初始化LCD1602
  lcd.print("Welcome to use!"); //液晶显示Welcome to use！
  lcd.clear();          //液晶清屏
  setup_hum();
}

void loop() {
  int * info = get_temp_and_hum();
  lcd.setCursor(0, 0);          //设置液晶开始显示的指针位置，0列0行，人类理解的第一行
  char str[100];
  sprintf(str,"T:%d*C H:%d%",info[0],info[1]);
  lcd.print(str);
//  lcd.setCursor(0, 1);          //0列，1行，人类理解的第二行
  while (Serial.available()>0){
    char data = Serial.read();
    if(data == sep){
      sig[sub++] = atoi(cur_element.c_str());
      cur_element = "";
    }
    else if(data == eof){
      irsend.sendRaw(sig,230,38);
      sub = 0;
    }
    else{
      cur_element += data;
    }
  }
  sprintf(str,"upload:{\"humidity\":%d}",info[1]);
  Serial.println(str);
}
