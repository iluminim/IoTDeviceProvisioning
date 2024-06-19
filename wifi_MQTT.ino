void publishmsg() {
  Serial.println("Publishing message: ");

  StaticJsonDocument<200> doc;
  doc["temperature"] = "34";
  doc["humidity"] = "35";
  doc["rainfall"] = "4";

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  Serial.print("Publishing message: ");
  Serial.println(jsonBuffer);
  client.publish(THINGSBOARD_TELEMETRY_TOPIC, jsonBuffer);
  }