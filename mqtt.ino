
int setaMqtt = 0;

void reconnect() {
  // Loop until we're reconnected
  if (!client.connected() and setaMqtt == 0) {
    setaMqtt = 1;
    Serial.println("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-teste";
    // clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()), "Crescer", "Crescer@123") {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("topico/rpm/", 0);
  
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      //  delay(5000);
    }
  }

  if (temp2.Saida(setaMqtt))
  {
    setaMqtt = 0;
  }


}
