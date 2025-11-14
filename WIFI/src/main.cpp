#include <Arduino.h>
#include <WiFi.h>

// prototipar as funcoes
void conexaoWiFi ();
void reconexaoWiFi ();

const char *SSID = "SALA 25";
const char *SENHA = "info@134";
const unsigned long tempoEsperaConexao =  5000;
const unsigned long tempoEsperaReconexao = 5000;



void setup() {
  Serial.begin (115200);

  conexaoWiFi ();
  

}

void loop() {

  reconexaoWiFi ();
}

void conexaoWiFi () {


Serial.printf ("Conectando ao Wifi: %s", SSID);

  WiFi.begin (SSID, SENHA);

  unsigned long tempoInicialWifi = millis();

  while (WiFi.status () != WL_CONNECTED && millis() - tempoInicialWifi <  tempoEsperaConexao)
  {
    Serial.print (".");
    delay (500);
  }

  if (WiFi.status () == WL_CONNECTED)
  {
    Serial.println ("Wifi conectado com sucesso!");
    Serial.print("Endereço de IP: ");
    Serial.println (WiFi.localIP());
  }
  else {
    Serial.println ("Falha ao se conectar");
  }
}

void reconexaoWiFi () {
unsigned long tempoAtual = millis ();
unsigned long tempoUltimaConexao = 0;

if (tempoAtual - tempoUltimaConexao > tempoEsperaReconexao)
{
   if (WiFi.status () != WL_CONNECTED)

   {
    Serial.println ("Conexao wifi perdida");
    conexaoWiFi ();
   }


tempoUltimaConexao = tempoAtual;
}

}