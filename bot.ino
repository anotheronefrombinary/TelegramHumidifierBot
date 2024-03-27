#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <SPI.h>
#include <LittleFS.h>

// Replace with your network credentials
const char* ssid = "Leonidas";
const char* password = "#";

// Replace with your Telegram BOT token
const char* botToken = "#";

WiFiClientSecure secured_client;
UniversalTelegramBot bot(botToken, secured_client);
unsigned long bot_last_time_checked; // Timestamp of last time messages were checked

// Global variables for timing humidity checks
unsigned long lastHumidityCheck = 0; // Stores the last time humidity was checked
const long interval = 40000;  // Interval at which to check humidity (milliseconds)
unsigned long lastStatsSent = 0; // Stores the last time stats were sent
const long statsInterval = 900000; // Interval at which to send stats (milliseconds)


// DHT Sensor Setup
#define DHTPIN D4     // What digital pin the DHT22 is connected to
#define DHTTYPE DHT22   // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);
// Assuming DHTPIN and relayPin are defined as in previous examples
float minHumidityThreshold = 40.0;
float maxHumidityThreshold = 60.0;
String systemMode = "Auto"; // Possible values: Auto, On, Off

// Relay Setup
const int relayPin = D3                ;  // Relay is connected to D0

// Function declarations
void handleNewMessages(int numNewMessages);

void setup() {
  Serial.begin(9600);
  readThresholds();
  // Initialize DHT sensor.
  dht.begin();

  // Setup relay pin
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Make sure relay is off on startup

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Set time for secure connection
  configTime(0, 0, "pool.ntp.org");  
  secured_client.setInsecure(); // Not recommended for production, but okay for testing
}
void saveThresholds() {
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  File file = LittleFS.open("/thresholds.txt", "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <SPI.h>
#include <LittleFS.h>

// Replace with your network credentials
const char* ssid = "Leonidas";
const char* password = "77777778";

// Replace with your Telegram BOT token
const char* botToken = "6338325983:AAHkzRH41mm9AvmIrh5fLAxpZ51rKIs5KUY";

WiFiClientSecure secured_client;
UniversalTelegramBot bot(botToken, secured_client);
unsigned long bot_last_time_checked; // Timestamp of last time messages were checked

// Global variables for timing humidity checks
unsigned long lastHumidityCheck = 0; // Stores the last time humidity was checked
const long interval = 40000;  // Interval at which to check humidity (milliseconds)
unsigned long lastStatsSent = 0; // Stores the last time stats were sent
const long statsInterval = 900000; // Interval at which to send stats (milliseconds)


// DHT Sensor Setup
#define DHTPIN D4     // What digital pin the DHT22 is connected to
#define DHTTYPE DHT22   // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);
// Assuming DHTPIN and relayPin are defined as in previous examples
float minHumidityThreshold = 40.0;
float maxHumidityThreshold = 60.0;
String systemMode = "Auto"; // Possible values: Auto, On, Off

// Relay Setup
const int relayPin = D3                ;  // Relay is connected to D0

// Function declarations
void handleNewMessages(int numNewMessages);

void setup() {
  Serial.begin(9600);
  readThresholds();
  // Initialize DHT sensor.
  dht.begin();

  // Setup relay pin
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Make sure relay is off on startup

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Set time for secure connection
  configTime(0, 0, "pool.ntp.org");  
  secured_client.setInsecure(); // Not recommended for production, but okay for testing
}
void saveThresholds() {
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  File file = LittleFS.open("/thresholds.txt", "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  file.println(minHumidityThreshold);
  file.println(maxHumidityThreshold);
  file.close();
}
void readThresholds() {
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
Serial.print("Free Heap: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("Heap Fragmentation: ");
  Serial.println(ESP.getHeapFragmentation());
  File file = LittleFS.open("/thresholds.txt", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  minHumidityThreshold = file.parseFloat();
  maxHumidityThreshold = file.parseFloat();
  file.close();
}
void sendStats(const String& chat_id) {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  String relayState = digitalRead(relayPin) == HIGH ? "On" : "Off";

  String message = "(აუტო) მიმდინარე მონაცემები:\n";
  message += "ტენიანობა: " + String(humidity) + "%\n";
  message += "ტემპერატურა: " + String(temperature) + "°C\n";
  message += "სისტემის რეჟიმი: " + systemMode + "\n";
  message += "დამატენიანებელი: " + relayState;

  bot.sendMessage(chat_id, message, "");
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    if (text == "/start") {
      String welcome = "გამარჯობა!.\nარსებული ბრძანებები:\n /info - მიმდინარე მონაცემების ნახვა\n /setmode On | Off | Auto - რეჟიმის დაყენება\n /setmin - ტენიანობის მინიმალური დასაშვები დონის დაყენება\n /setmax - ტენიანობის მაქსიმალური დასაშვები დონის დაყენება";
      bot.sendMessage(chat_id, welcome, "");
    } 
    else if (text == "/info") {
      float humidity = dht.readHumidity();
      float temperature = dht.readTemperature();
      // Add temperature or other readings if necessary
      String message = "მიმდინარე მონაცემები\n";
      message += "ტენიანობა: " + String(humidity) + "%\n";
      message += "ტემპერატურა: " + String(temperature) + " °C\n";
      message += "სისტემის რეჟიმი: " + systemMode + "\n";
      message += "დამატენიანებელი: ";
      message += (digitalRead(relayPin) == HIGH ? "On" : "Off");
      message += "\nმაქს. დაშვებული ტენიანობა: " + String(maxHumidityThreshold) + "\nმინ. დაშვებული ტენიანობა: " + String(minHumidityThreshold);

      bot.sendMessage(chat_id, message, "");
      controlRelayBasedOnHumidity();
    }
    // Implement other commands (/setmode, /settings, /viewlogs)
    else if (text.startsWith("/setmode")) {
    String mode = text.substring(9); // Extract mode from command
    mode.trim(); // Remove any leading or trailing whitespace

    if (mode == "Auto" || mode == "On" || mode == "Off") {
        systemMode = mode;
        String reply = "დამატენიანებელი: " + mode;
        Serial.println(mode);

        // Immediately apply the new mode to adjust relay state
        controlRelayBasedOnHumidity(); // Evaluate and adjust relay state based on the new mode

        bot.sendMessage(chat_id, reply, "");
    } else {
        bot.sendMessage(chat_id, "ასეთი რეჟიმი არ არსებობს. გამოიყენეთ Auto, On, Off.", "");
      }
    }
    else if (text.startsWith("/setmin")) {
      String valueString = text.substring(7); // Extract the numeric part
      valueString.trim(); // Remove any leading or trailing whitespace
      float newValue = valueString.toFloat(); // Convert string to float
      if (newValue > 0 && newValue < maxHumidityThreshold) { // Basic validation
        minHumidityThreshold = newValue;
        String reply = "მინიმალური ტენიანობის დასაშვები დონე გახდა: " + String(newValue) + "%";
        bot.sendMessage(chat_id, reply, "");
        saveThresholds();
        controlRelayBasedOnHumidity();
      } else {
        bot.sendMessage(chat_id, "არასწორი მნიშვნელობა. გამოიყენეთ დადებითი და ტენიანობის დაშვებულ მაქსიმუმზე ნაკლები რიცხვი.", "");
      }
    }
    else if (text.startsWith("/setmax")) {
      String valueString = text.substring(7); // Extract the numeric part
      valueString.trim(); // Remove any leading or trailing whitespace
      float newValue = valueString.toFloat(); // Convert string to float
      if (newValue > 0 && newValue > minHumidityThreshold) { // Basic validation
        maxHumidityThreshold = newValue;
        String reply = "მაქსიმალური ტენიანობის დასაშვები დონე გახდა: " + String(newValue) + "%";
        bot.sendMessage(chat_id, reply, "");
        saveThresholds();
        controlRelayBasedOnHumidity();
      } else {
        bot.sendMessage(chat_id, "არასწორი მნიშვნელობა. გამოიყენეთ დადებითი რიცხვი და ტენიანობის დაშვებულ მინიმუმზე მეტი რიცხვი.", "");
      }
    }
  }
}
void controlRelayBasedOnHumidity() {
    float currentHumidity = dht.readHumidity();
    bool relayShouldBeOn = false;
    
    if (isnan(currentHumidity)) {
        Serial.println("Failed to read from DHT sensor! Can't control relay.");
        return; // Exit if failed to read humidity
    }

    if (systemMode == "Auto") {
        if (currentHumidity < minHumidityThreshold) {
            relayShouldBeOn = true;
        } else if (currentHumidity > maxHumidityThreshold) {
            relayShouldBeOn = false;
        }
        // Optionally, handle the "within threshold" scenario here
    } else if (systemMode == "On") {
        relayShouldBeOn = true;
    } else if (systemMode == "Off") {
        relayShouldBeOn = false;
    }
    
    // Check for a change in state and actuate relay accordingly
    if (relayShouldBeOn != (digitalRead(relayPin) == HIGH)) {
        digitalWrite(relayPin, relayShouldBeOn ? HIGH : LOW);
        
        // Notify via Telegram
        String message = "დამატენიანებელის ახალი მდგომარეობა:" + String(relayShouldBeOn ? "ON" : "OFF");
        sendNotification(message);
        
        Serial.println(message);
    }
}

void sendNotification(String message) {
    // Placeholder for your chat ID or an array of IDs
    String chat_id = "835640898";

    // Send the message
    bot.sendMessage(chat_id, message, "");
}

void loop() {
  unsigned long currentMillis = millis();
 // Returns fragmentation in percentage
  // Check if it's time to read the humidity and control the relay
  if (currentMillis - lastHumidityCheck >= interval) {
    lastHumidityCheck = currentMillis; // Save the last time you checked humidity

    // Read humidity and control the relay
    controlRelayBasedOnHumidity();
    Serial.println("HUMCHECK");
  }

  // Existing code to check for new bot messages
  if (millis() - bot_last_time_checked > 1000) {  // Only check for new bot messages every second
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_last_time_checked = millis();
  }
  // Check if it's time to send stats
  if (currentMillis - lastStatsSent >= statsInterval) {
    lastStatsSent = currentMillis; // Update the last stats sent time

    // Send stats to a predetermined chat ID or modify as needed for dynamic IDs
    sendStats("835640898");
  }
}

  }

  file.println(minHumidityThreshold);
  file.println(maxHumidityThreshold);
  file.close();
}
void readThresholds() {
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
Serial.print("Free Heap: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("Heap Fragmentation: ");
  Serial.println(ESP.getHeapFragmentation());
  File file = LittleFS.open("/thresholds.txt", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  minHumidityThreshold = file.parseFloat();
  maxHumidityThreshold = file.parseFloat();
  file.close();
}
void sendStats(const String& chat_id) {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  String relayState = digitalRead(relayPin) == HIGH ? "On" : "Off";

  String message = "(აუტო) მიმდინარე მონაცემები:\n";
  message += "ტენიანობა: " + String(humidity) + "%\n";
  message += "ტემპერატურა: " + String(temperature) + "°C\n";
  message += "სისტემის რეჟიმი: " + systemMode + "\n";
  message += "დამატენიანებელი: " + relayState;

  bot.sendMessage(chat_id, message, "");
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    if (text == "/start") {
      String welcome = "გამარჯობა!.\nარსებული ბრძანებები:\n /info - მიმდინარე მონაცემების ნახვა\n /setmode On | Off | Auto - რეჟიმის დაყენება\n /setmin - ტენიანობის მინიმალური დასაშვები დონის დაყენება\n /setmax - ტენიანობის მაქსიმალური დასაშვები დონის დაყენება";
      bot.sendMessage(chat_id, welcome, "");
    } 
    else if (text == "/info") {
      float humidity = dht.readHumidity();
      float temperature = dht.readTemperature();
      // Add temperature or other readings if necessary
      String message = "მიმდინარე მონაცემები\n";
      message += "ტენიანობა: " + String(humidity) + "%\n";
      message += "ტემპერატურა: " + String(temperature) + " °C\n";
      message += "სისტემის რეჟიმი: " + systemMode + "\n";
      message += "დამატენიანებელი: ";
      message += (digitalRead(relayPin) == HIGH ? "On" : "Off");
      message += "\nმაქს. დაშვებული ტენიანობა: " + String(maxHumidityThreshold) + "\nმინ. დაშვებული ტენიანობა: " + String(minHumidityThreshold);

      bot.sendMessage(chat_id, message, "");
      controlRelayBasedOnHumidity();
    }
    // Implement other commands (/setmode, /settings, /viewlogs)
    else if (text.startsWith("/setmode")) {
    String mode = text.substring(9); // Extract mode from command
    mode.trim(); // Remove any leading or trailing whitespace

    if (mode == "Auto" || mode == "On" || mode == "Off") {
        systemMode = mode;
        String reply = "დამატენიანებელი: " + mode;
        Serial.println(mode);

        // Immediately apply the new mode to adjust relay state
        controlRelayBasedOnHumidity(); // Evaluate and adjust relay state based on the new mode

        bot.sendMessage(chat_id, reply, "");
    } else {
        bot.sendMessage(chat_id, "ასეთი რეჟიმი არ არსებობს. გამოიყენეთ Auto, On, Off.", "");
      }
    }
    else if (text.startsWith("/setmin")) {
      String valueString = text.substring(7); // Extract the numeric part
      valueString.trim(); // Remove any leading or trailing whitespace
      float newValue = valueString.toFloat(); // Convert string to float
      if (newValue > 0 && newValue < maxHumidityThreshold) { // Basic validation
        minHumidityThreshold = newValue;
        String reply = "მინიმალური ტენიანობის დასაშვები დონე გახდა: " + String(newValue) + "%";
        bot.sendMessage(chat_id, reply, "");
        saveThresholds();
        controlRelayBasedOnHumidity();
      } else {
        bot.sendMessage(chat_id, "არასწორი მნიშვნელობა. გამოიყენეთ დადებითი და ტენიანობის დაშვებულ მაქსიმუმზე ნაკლები რიცხვი.", "");
      }
    }
    else if (text.startsWith("/setmax")) {
      String valueString = text.substring(7); // Extract the numeric part
      valueString.trim(); // Remove any leading or trailing whitespace
      float newValue = valueString.toFloat(); // Convert string to float
      if (newValue > 0 && newValue > minHumidityThreshold) { // Basic validation
        maxHumidityThreshold = newValue;
        String reply = "მაქსიმალური ტენიანობის დასაშვები დონე გახდა: " + String(newValue) + "%";
        bot.sendMessage(chat_id, reply, "");
        saveThresholds();
        controlRelayBasedOnHumidity();
      } else {
        bot.sendMessage(chat_id, "არასწორი მნიშვნელობა. გამოიყენეთ დადებითი რიცხვი და ტენიანობის დაშვებულ მინიმუმზე მეტი რიცხვი.", "");
      }
    }
  }
}
void controlRelayBasedOnHumidity() {
    float currentHumidity = dht.readHumidity();
    bool relayShouldBeOn = false;
    
    if (isnan(currentHumidity)) {
        Serial.println("Failed to read from DHT sensor! Can't control relay.");
        return; // Exit if failed to read humidity
    }

    if (systemMode == "Auto") {
        if (currentHumidity < minHumidityThreshold) {
            relayShouldBeOn = true;
        } else if (currentHumidity > maxHumidityThreshold) {
            relayShouldBeOn = false;
        }
        // Optionally, handle the "within threshold" scenario here
    } else if (systemMode == "On") {
        relayShouldBeOn = true;
    } else if (systemMode == "Off") {
        relayShouldBeOn = false;
    }
    
    // Check for a change in state and actuate relay accordingly
    if (relayShouldBeOn != (digitalRead(relayPin) == HIGH)) {
        digitalWrite(relayPin, relayShouldBeOn ? HIGH : LOW);
        
        // Notify via Telegram
        String message = "დამატენიანებელის ახალი მდგომარეობა:" + String(relayShouldBeOn ? "ON" : "OFF");
        sendNotification(message);
        
        Serial.println(message);
    }
}

void sendNotification(String message) {
    // Placeholder for your chat ID or an array of IDs
    String chat_id = "835640898";

    // Send the message
    bot.sendMessage(chat_id, message, "");
}

void loop() {
  unsigned long currentMillis = millis();
 // Returns fragmentation in percentage
  // Check if it's time to read the humidity and control the relay
  if (currentMillis - lastHumidityCheck >= interval) {
    lastHumidityCheck = currentMillis; // Save the last time you checked humidity

    // Read humidity and control the relay
    controlRelayBasedOnHumidity();
    Serial.println("HUMCHECK");
  }

  // Existing code to check for new bot messages
  if (millis() - bot_last_time_checked > 1000) {  // Only check for new bot messages every second
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_last_time_checked = millis();
  }
  // Check if it's time to send stats
  if (currentMillis - lastStatsSent >= statsInterval) {
    lastStatsSent = currentMillis; // Update the last stats sent time

    // Send stats to a predetermined chat ID or modify as needed for dynamic IDs
    sendStats("835640898");
  }
}
