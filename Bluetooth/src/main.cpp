/*#include <Arduino.h>
#include <BluetoothSerial.h>

 BluetoothSerial BT;

 void setup() {
   Serial.begin(9600);
   Serial.setTimeout(5000);

   if (BT.begin("EspSlaveGrasi"))
   {
    Serial.println("Bluetooth iniciado com sucesso");
     Serial.print("Endereço bluetooth ESP:  ");
     Serial.println(BT.getBtAddressString());
     //F4:65:0B:54:58:AE

   }else
   {
     Serial.println("Erro ao iniciar o bluetooth");
   }
 }

 void loop() {
     if (BT.available())
     {
        String mensagemRecebida = BT.readStringUntil('\n');
         mensagemRecebida.trim();
         Serial.printf("Mensagem recebida: %s", mensagemRecebida);
     }

     if (Serial.available())
     {
         String mensagemEnviar = Serial.readStringUntil('\n');
         mensagemEnviar.trim();
         BT.println(mensagemEnviar);
     }
} */

/*#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial BT;
const int ledPin = 2; 

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  if (!BT.begin("espSlaveThalita0")) {
    Serial.println("Erro ao iniciar o Bluetooth slave");
    while (1) {} 
  }
  Serial.println("Bluetooth slave iniciado com sucesso");
}

void loop() {
  if (BT.available()) {
    String comando = BT.readStringUntil('\n');
    comando.trim();
    Serial.print("Comando recebido: ");
    Serial.println(comando);

    if (comando == "liga") {
      digitalWrite(ledPin, HIGH);
      BT.println("LED ligado");
    } else if (comando == "desliga") {
      digitalWrite(ledPin, LOW);
      BT.println("LED desligado");
    } else if (comando == "pisca") {
      BT.println("LED piscando");
      for (int i = 0; i = 5; i++) {
        digitalWrite(ledPin, HIGH);
        delay(300);
        digitalWrite(ledPin, LOW);
        delay(300);
      }
      BT.println("desliga");
    } else {
      BT.println("Comando inválido");
    }
  }
}*/
/*
#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial BT;

const int botaoPin = 0; 
const int ledPin = 2;   

enum EstadoLed { DESLIGADO, ACESO, PISCANDO };
EstadoLed estadoLed = DESLIGADO;

bool botaoAnterior = HIGH;
unsigned long tempoPressionado = 0;
unsigned long ultimoMillis = 0;
bool ledLigado = false;

void setup() {
  Serial.begin(115200);
  pinMode(botaoPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  if (!BT.begin("espSlavePietra0")) {
    Serial.println("Erro ao iniciar Bluetooth Slave");
    while (1);
  }
  Serial.println("Bluetooth Slave iniciado e aguardando conexão...");
}

void loop() {
  bool botaoAtual = digitalRead(botaoPin);

  
  if (botaoAnterior == HIGH && botaoAtual == LOW) tempoPressionado = millis();

  
  if (botaoAnterior == LOW && botaoAtual == HIGH) {
    unsigned long duracao = millis() - tempoPressionado;

    
    if (duracao < 2000) {
      BT.println("toggle");
      Serial.println("Comando enviado: toggle");
    } 
   
    else {
      BT.println("pisca");
      Serial.println("Comando enviado: pisca");
    }
  }
  botaoAnterior = botaoAtual;

  
  if (BT.available()) {
    String comando = BT.readStringUntil('\n');
    comando.trim();
    Serial.printf("Comando recebido: %s\n", comando.c_str());

    if (comando.equalsIgnoreCase("toggle")) {
      if (estadoLed == DESLIGADO) estadoLed = ACESO;
      else if (estadoLed == ACESO || estadoLed == PISCANDO) estadoLed = DESLIGADO;
    } 
    else if (comando.equalsIgnoreCase("pisca")) {
      estadoLed = PISCANDO;
      ledLigado = false;
      ultimoMillis = millis();
    }
  }

  
  if (estadoLed == DESLIGADO) {
    digitalWrite(ledPin, LOW);
  } 
  else if (estadoLed == ACESO) {
    digitalWrite(ledPin, HIGH);
  } 
  else if (estadoLed == PISCANDO) {
    unsigned long agora = millis();
    if (agora - ultimoMillis >= 500) {
      ledLigado = !ledLigado;
      digitalWrite(ledPin, ledLigado ? HIGH : LOW);
      ultimoMillis = agora;
    }
  }
}*/

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

BluetoothSerial BT;
LiquidCrystal_I2C lcd(0x27, 16, 2); 

unsigned long ultimoUpdate = 0;
const unsigned long intervalo = 1000; 

String umid = "";
String tempC = "";
String tempF = "";
bool dadosNovos = false;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  if (!BT.begin("ESP32_Slave")) {
    Serial.println(" Erro ao iniciar Bluetooth!");
    while (true);
  }

  Serial.println(" Bluetooth Slave pronto!");
  lcd.setCursor(0, 0);
  lcd.print("Aguardando dados...");
}

void loop() {
 
  if (BT.available()) {
    String recebido = BT.readStringUntil('\n');
    recebido.trim();
    Serial.println(" Recebido: " + recebido);

    
    int p1 = recebido.indexOf(',');
    int p2 = recebido.lastIndexOf(',');

    if (p1 > 0 && p2 > p1) {
      umid = recebido.substring(0, p1);
      tempC = recebido.substring(p1 + 1, p2);
      tempF = recebido.substring(p2 + 1);
      dadosNovos = true;
    }
  }

  
  unsigned long agora = millis();
  if (agora - ultimoUpdate >= intervalo && dadosNovos) {
    ultimoUpdate = agora;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Umi: ");
    lcd.print(umid);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("T: ");
    lcd.print(tempC);
    lcd.print("C ");
    lcd.print(tempF);
    lcd.print("F");
  }
}
