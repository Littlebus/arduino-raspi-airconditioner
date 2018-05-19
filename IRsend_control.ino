#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

IRsend irsend;
char sep = ' ';
char eof = '\n';
String cur_element = "";
int sig[231];
int sub = 0;
void setup() {
  Serial.begin(115200); 
}

void loop() {

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
}
