#define led_red 5
#define led_blue 14
#define button_done 12
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

WiFiClient client;
PubSubClient mqtt_client(client);

const char *ssid = "HP Ultrasonic";
const char *password= "wwwwwwww";
const char *server_mqtt="192.168.0.132";  
const int port_mqtt=1883;
const char *mqtt_id="AVG";
const char *topic_subscribe="ESP8266_AVG";
const char *topic_publish="ESP8266_AVG";
char rx_data[50]="";
int AVG[50];

String message_send="";
String p= "";
String check="";
int client2_line1=0;
int client2_line2=0;
int client1=0;
int number=0;
int i=0;
int count=0;
void callback(char *topic,byte *payload, unsigned int length)
{
  Serial.print("receive from: ");
  Serial.println(topic);
  Serial.print("messeage: ");
  for (size_t i = 0; i < length; i++)
  {
     rx_data[i]=((char)payload[i]);
  }
  p=String(rx_data);
  number= p.toInt();
  Serial.println(number);
  Serial.print("-------------------------------");
  Serial.println();
  memset(rx_data,0,sizeof(rx_data));
  if (p=="ON")
  {
    digitalWrite(led_blue,1);
    
  }
  if (p=="OFF")
  {
    digitalWrite(led_blue,0);
    
  }
  
}  
void control_AVG()
{
  for (i;i<4;i++)
  {
    while (number==i && i!=0)
    {
      // a[count]=i;
      // count++;
      //delay(1000); 
      Serial.print(" AVG to client: ");
      Serial.println(i);
      digitalWrite(led_red,1);
      if (digitalRead(button_done)==0)
      {
        while (digitalRead(button_done)==0)
        {
          delay(10);
        }
        check="";
        //umber=0;
        digitalWrite(led_red,0);
        mqtt_client.publish(topic_publish, ("done_" + String(i)).c_str());
        Serial.println("AVG is available");
        break;
      } 
    }
  }
  p="";
  i=0;
  number=0;
}

void setup() 
{
  Serial.begin(9600);
  pinMode(led_blue,OUTPUT);
  pinMode(led_red,OUTPUT);
  pinMode(button_done,INPUT_PULLUP);
  Serial.println("connecting to wifi");

  WiFi.begin(ssid,password);
  // ket noi wifi
  while (WiFi.status()!=WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  } 
  Serial.println("connected to wifi");

  Serial.println("connecting to MQTT");
  mqtt_client.setServer(server_mqtt, port_mqtt);
  mqtt_client.setCallback(callback);
  // ket noi mqtt
  while (!mqtt_client.connect(mqtt_id))
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println("connected to MQTT");
  mqtt_client.publish(topic_publish," AVG hello Server");
  mqtt_client.subscribe(topic_subscribe);

 }
void loop() 
{
  mqtt_client.loop();
  control_AVG();
}
