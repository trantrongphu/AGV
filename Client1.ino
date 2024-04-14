#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient client;
PubSubClient mqtt_client(client);

//const char *ssid = "HiepPhat";
const char *ssid = "HP Ultrasonic";
//const char *password = "qqqqqqqq";
const char *password = "wwwwwwww";
const char *server_mqtt = "192.168.0.132";
const int port_mqtt = 1883;
const char *mqtt_id = "Client 1";
const char *topic_subscribe = "ESP8266_AVG";
const char *topic_publish = "ESP8266_AVG";

String message_send = "";
int client1 = 0;
int client1_NG = 0;
int status = 0;
void SendToSever()
{
  if (digitalRead(4) == 0)
  {
    while(digitalRead(4) == 0)
    {
      delay(10);
      // trong lúc line 1 đang có sản phẩm kiểm tra xem line 2 có sản phẩm không
      if (digitalRead(14) == 0)
      {
        while(digitalRead(14) == 0)
        {
          delay(200);
        }
        if (mqtt_client.connected())
        {
          client1_NG++;
          // String payload = "";
          // payload += client2;
          String payload = "{\"client1_NG\":";
          payload += client1_NG;
          payload += "}";
          mqtt_client.publish(topic_publish, payload.c_str());
        }
      }
    }
    if (mqtt_client.connected())
    {
      client1++;
      // String payload = "";
      // payload += client2;
      String payload = "{\"Client1\":";
      payload += client1;
      payload += "}";
      mqtt_client.publish(topic_publish, payload.c_str());
    }
  }
}
void call_AVG ()
{
  if (digitalRead(12) == 0)
  {
    while (digitalRead(12) == 0)
    {
      delay(10);
    }
    if (mqtt_client.connected() && digitalRead(13) == LOW)
    {
      mqtt_client.publish(topic_publish, "2");
      digitalWrite(13,HIGH);
    }  
  }
}
void Send_NG()
{
  if (digitalRead(14) == 0)
  {
    while(digitalRead(14) == 0)
    {
      delay(20);
      if (digitalRead(4) == 0)
      {
        while(digitalRead(4) == 0)
        {
          delay(100);
        }
        if (mqtt_client.connected())
        {
          client1++;
          // String payload = "";
          // payload += client2;
          String payload = "{\"Client1\":";
          payload += client1;
          payload += "}";
          mqtt_client.publish(topic_publish, payload.c_str());
        }
      }
    }
    if (mqtt_client.connected())
    {
      client1_NG++;
      // String payload = "";
      // payload += client2;
      String payload = "{\"client1_NG\":";
      payload += client1_NG;
      payload += "}";
      mqtt_client.publish(topic_publish, payload.c_str());
    }
  }
}
void callback(char *topic, byte *payload, unsigned int length)
{
  String message = "";
  Serial.print("Recived from: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (size_t i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  Serial.println(message);
  if (message == "ON")
  {
    status = 1;
    digitalWrite(5, HIGH);
    // client1 = 0;
    // client1_NG = 0;
    // String payload = "{\"Client1\":";
    // payload += client1;
    // payload += ",\"Client1_NG\":";
    // payload += client1_NG;
    // payload += "}";
    //mqtt_client.publish(topic_publish, payload.c_str());
  }
  if (message == "OFF")
  {
    digitalWrite(5, LOW);
    digitalWrite(13, LOW);
    status = 0;
    client1 = 0;
    client1_NG = 0;
  }
  if (message == "done_2")
  {
    digitalWrite(13, LOW);
  }
}

void setup() 
{
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(14, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  Serial.println("");
  Serial.print("Dang ket noi toi WiFi...");
  WiFi.begin(ssid, password);
  WiFi.reconnect();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Da ket noi toi WiFi");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  delay(1000);

  mqtt_client.setServer(server_mqtt, port_mqtt);
  mqtt_client.setCallback(callback);

  Serial.println("Dang ket noi toi mqtt ");
  while (!mqtt_client.connect(mqtt_id))
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Da ke noi toi mqtt ");
  mqtt_client.publish(topic_publish, "Client 1 Hello Server");
  mqtt_client.subscribe(topic_subscribe);
}

void loop() {
  // put your main code here, to run repeatedly:
  mqtt_client.loop();
  //Receive();
  SendToSever();
  Send_NG();
  call_AVG ();
}
