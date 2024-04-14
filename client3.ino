#define led_blue 5
#define led_green 13
#define button_line_1 4
#define button_line_2 14
#define button_call_AVG 12
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

WiFiClient client;
PubSubClient mqtt_client(client);

const char *ssid = "HP Ultrasonic";
const char *password= "wwwwwwww";
const char *server_mqtt="192.168.0.132";  
const int port_mqtt=1883;
const char *mqtt_id="client 3";
const char *topic_subscribe="ESP8266_AVG";
const char *topic_publish="ESP8266_AVG";
char rx_data[50]="";

String message_send="";
String p= "";
int client3_line1=0;
int client3_line2=0;
int client1=0;

void call_AVG()
{
  if (digitalRead(button_call_AVG)==0)
  {
    while (digitalRead(button_call_AVG)==0)
    {
      delay(10);
    }
    digitalWrite(led_green,1);
    if (mqtt_client.connected())
    {
      mqtt_client.publish(topic_publish,"3");
    }
  }
}
void line_1()
{
  if (digitalRead(button_line_1)==0)
  {
    while (digitalRead(button_line_1)==0)
    {
      delay(10);
      if (digitalRead(button_line_2)==0)
      {
        while (digitalRead(button_line_2)==0)
        {
          delay(10);
        }
        if (mqtt_client.connected())
        {
          client3_line2++;
          // String payload = "";
          // payload += client2;
          String payload = "{\"client3_line2\":";
          payload += client3_line2;
          payload += "}\0";
          mqtt_client.publish(topic_publish, payload.c_str());
        }
      }
    }
    if (mqtt_client.connected())
    {
      client3_line1++;
      // String payload = "";
      // payload += client2;
      String payload = "{\"client3_line1\":";
      payload += client3_line1;
      payload += "}\0";
      mqtt_client.publish(topic_publish, payload.c_str());
    }
  }
}
void line_2()
{
  if (digitalRead(button_line_2)==0)
  {
    while (digitalRead(button_line_2)==0)
    {
      delay(10);
      if (digitalRead(button_line_1)==0)
      {
        while (digitalRead(button_line_1)==0)
        {
          delay(10);
        }
        if (mqtt_client.connected())
        {
          client3_line1++;
          // String payload = "";
          // payload += client2;
          String payload = "{\"client3_line1\":";
          payload += client3_line1;
          payload += "}\0";
          mqtt_client.publish(topic_publish, payload.c_str());
        }
      }
    }
    if (mqtt_client.connected())
    {
      client3_line2++;
      // String payload = "";
      // payload += client2;
      String payload = "{\"client3_line2\":";
      payload += client3_line2;
      payload += "}\0";
      mqtt_client.publish(topic_publish, payload.c_str());
    }
  }
}
 // read data
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
  Serial.println(p);
  Serial.print("-------------------------------");
  Serial.println();
  memset(rx_data,0,sizeof(rx_data));
  // khi AGV on, các client đều sẵn sàng
  if (p=="ON")
  {
    digitalWrite(led_blue,HIGH);
  }
  if (p=="OFF")
  {
    digitalWrite(led_blue,LOW);
  }
  if (p=="done_3")
  {
    digitalWrite(led_green,0);
  }
}
void setup() 
{
  Serial.begin(9600);
  pinMode(led_blue,OUTPUT);
  pinMode(led_green,OUTPUT);
  pinMode(button_line_1,INPUT_PULLUP);
  pinMode(button_line_2,INPUT_PULLUP);
  pinMode(button_call_AVG,INPUT_PULLUP);
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
  mqtt_client.publish(topic_publish," client 3  hello Server");
  mqtt_client.subscribe(topic_subscribe);

}
void loop() 
{
  mqtt_client.loop();
  line_1();
  line_2();
  call_AVG();
  //Serial.println(p);
}
