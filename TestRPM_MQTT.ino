
//18
//23
#include <PubSubClient.h>
#include <WiFi.h>      //ESP32 Core WiFi Library  

//const char* mqtt_server = "192.168.100.105";
//const char* mqtt_server = "192.168.0.13";
const char* mqtt_server = "192.168.100.114"; // 4.227.241.127
const char* mqtt_server2 = "192.168.100.114";
//http://4.227.241.127:1880/

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (10)
char msg[MSG_BUFFER_SIZE];
char msg2[MSG_BUFFER_SIZE];


#define GPIO_BOTAO 32
#define GPIO_BOTAO2 18

#define TEMPO_DEBOUNCE 10 //ms
#include <Crescer.h>

Tempora temp1;
Tempora temp2;
Tempora tempEnvio;
Tempora tempMqtt;

int leitura = 0;
int leituraOld = 0;
long TF = 0;
long TI = 0;
long delta = 0;
long pulsos = 0;
long pulsosCalc = 0;
float rpm = 0.000;
float rpmCorr = 0.000;
float rpmCorr2 = 0.000;

int media = 10;
int media2 = 10;

int leitura2 = 0;
int leituraOld2 = 0;
long TF2 = 0;
long TI2 = 0;
long delta2 = 0;
long pulsos2 = 0;
long pulsosCalc2 = 0;
float rpm2 = 0.000;


// pino 23
float RPMmedido = 0.00;
int cont = 0;
float acumRPM = 0.00;
float const1 = 1.225;
float const2 = 7.779;


// pino 18
float RPMmedido2 = 0.00;
int cont2 = 0;
float acumRPM2 = 0.00;
float const3 = 1.204;
float const4 = 0.542;

float velM1 = 0.00;
float velM2 = 0.00;

int seta = 0;

void IRAM_ATTR contRPM1()
{
  TF = micros();
  delta = TF - TI;
  TI = TF;
  seta = 1;
  //detachInterrupt(digitalPinToInterrupt(GPIO_BOTAO));
  // noInterrupts();
}


/*
  void IRAM_ATTR contRPM2()
  {
  TF2 = micros();
  delta2 = TF2 - TI2;
  TI2 = TF2;
  }
*/



#include <WiFi.h>

const char* ssid     = "crescer";
const char* password = "cpb32832";

WiFiServer server(80);




int selRPM = 1;
void setup()
{
  Serial.begin(115200);
 tempMqtt.defiSP(5000);
  /* Configura o GPIO do botão como entrada
    e configura interrupção externa no modo
    RISING para ele.
  */
  pinMode(GPIO_BOTAO, INPUT);

  attachInterrupt(digitalPinToInterrupt(GPIO_BOTAO), contRPM1, RISING);//FALLING    RISING
  temp1.defiSP(10);
  temp2.defiSP(30000);
  tempEnvio.defiSP(100);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  client.setServer(mqtt_server, 1883);
}


float RPM = 0;
float acum = 0;
int conta = 0;

void loop()
{

  //Serial.println(digitalRead(GPIO_BOTAO));




  if (seta)
  {

    acum = acum + (1 / (float(delta / 1000000.000000))) * 60;
    seta = 0;
    conta++;




    if (acum / conta < 1500 or conta == 7)
    {
  //    Serial.print(0);
  //    Serial.print(",");
  //    Serial.print(4000);
  //   Serial.print(",");
RPM = acum/ conta;
  //    Serial.println(RPM);

      acum = 0;
      conta = 0;
    }



  }




  if (WiFi.status() == 3)
  {


    if (tempEnvio.Saida(1))
    {

      snprintf (msg, MSG_BUFFER_SIZE, "%ld", int(RPM));

 Serial.println(msg);

      if (client.connected())
      {
        client.publish("topico/rpm/", msg);


      }
      tempEnvio.Saida(0);
    }






        if (WiFi.status() == 3)
        {
          if (tempMqtt.Saida(1))
          {
            if (!client.connected()) {
              reconnect();
            }
            tempMqtt.Saida(0);

            if (!client.connected()) {
                Serial.print("Desconectado  ");
           //   statusMqtt = "Desconectado";
            }

            else
            {
                Serial.print("Conectado  ");
             // statusMqtt = "Conectado";
            }
          }

          if (client.connected()) client.loop();
        }

    
      





  }

}
