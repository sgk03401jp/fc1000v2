#include <WiFi.h>
#include <NetworkClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "MyWiFiSettings.h"
#include "MyWebserver.h"
#include "Adafruit_HDC1000.h"

//#define PIN_OUTPUT 5
//#define PIN_FAN 5
//#define PIN_LED 4
#define PIN_A0 0
#define PIN_A1 1
//#define IR_SENSOR_PIN 3  
//#define ULTRASONIC_TRIGGER_PIN 2  
//#define ULTRASONIC_ECHO_PIN 3     
//#define BUZZER_PIN 4              
#define DHT_PIN 3   
#define DHT_TYPE DHT11

/* Actual data stored in WiFiSettings.h */
//const char *ssid = "YOUR_SSID";
//const char *password = "YOUR_PASSWD";

WebServer server(80);

Adafruit_HDC1000 hdc = Adafruit_HDC1000();

TaskHandle_t mainTask;
TaskHandle_t serverTask;
TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL;
TaskHandle_t Task3Handle = NULL;

void ADC_Function(void *pvParameters);

void UpdateSlider();
void ProcessButton_1();
void ProcessButton_0();
void SendXML();
void SendWebsite();

//const int led = 13;
int BitsA0 = 0, BitsA1 = 0;
float VoltsA0 = 0, VoltsA1 = 0;
uint32_t SensorUpdate = 0;
int FanSpeed = 0;
bool LED0 = false, SomeOutput = false;
bool emergencyShutdownActive = false;
float temperatureDHT = 0.0;
float humidityDHT = 0.0;
int taskCount = 0;

char XML[2048];
char buf[32];

void handleRoot() {
//  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp32!");
//  digitalWrite(led, 0);
}

void handleNotFound() {
//  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
//  digitalWrite(led, 0);
}

void setup(void) {
//  pinMode(led, OUTPUT);
//  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  if (!hdc.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }

  server.onNotFound(handleNotFound);

  server.on("/", SendWebsite);
  server.on("/xml", SendXML);
  server.on("/UPDATE_SLIDER", UpdateSlider);
  server.on("/BUTTON_0", ProcessButton_0);
  server.on("/BUTTON_1", ProcessButton_1);

  server.begin();
  Serial.println("HTTP server started");

  xTaskCreateUniversal(ADC_Function,        "mainTask",   8192, NULL, 1, &mainTask, 0);
  // Create FreeRTOS tasks
  xTaskCreateUniversal(IR_Sensor_Function,  "Task1",      8192, NULL, 2, &Task1Handle, 0);
  xTaskCreateUniversal(DHT_Sensor_Function, "Task2",      8192, NULL, 2, &Task2Handle, 0);
  xTaskCreateUniversal(Emergency_Function,  "Task3",      8192, NULL, 3, &Task3Handle, 0);
  // Create a server task pinned to core 1
  xTaskCreateUniversal(ServerTask,          "serverTask", 4096, NULL, 3, &serverTask, 1);
}

void loop(void) {
//  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks
}


void ADC_Function(void *pvParameters) {
  taskCount++;
  while (true) {
    if ((millis() - SensorUpdate) >= 50) {
      SensorUpdate = millis();
      BitsA0 = analogRead(PIN_A0);
      BitsA1 = analogRead(PIN_A1);
      VoltsA0 = BitsA0 * 3.3 / 4096;
      VoltsA1 = BitsA1 * 3.3 / 4096;
    }
    vTaskDelay(pdMS_TO_TICKS(10));  // Adjust the delay as needed
  }
  
}

void UpdateSlider() {
  String t_state = server.arg("VALUE");
  FanSpeed = t_state.toInt();
  Serial.print("UpdateSlider ");
  Serial.println(FanSpeed);

  // Map FanSpeed to LED brightness
  int ledBrightness = map(FanSpeed, 0, 255, 0, 255);

  // Set LED brightness 
//  analogWrite(PIN_FAN, ledBrightness);

  // Respond with the updated RPM value
  server.send(200, "text/plain", String(FanSpeed));
}

void ProcessButton_0() {
  LED0 = !LED0;
//  digitalWrite(PIN_LED, LED0);
  Serial.print("Button 0 ");
  Serial.println(LED0);
  server.send(200, "text/plain", "");
}

void ProcessButton_1() {
  SomeOutput = !SomeOutput;
//  digitalWrite(PIN_OUTPUT, SomeOutput);
  Serial.print("Button 1 ");
  Serial.println(LED0);
  server.send(200, "text/plain", "");
}

void SendWebsite() {
  Serial.println("sending web page");
  server.send(200, "text/html", PAGE_MAIN);
}

void SendXML() {
  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");
  sprintf(buf, "<B0>%d</B0>\n", BitsA0);
  strcat(XML, buf);
  sprintf(buf, "<V0>%d.%d</V0>\n", (int)(VoltsA0), abs((int)(VoltsA0 * 10) - ((int)(VoltsA0) * 10)));
  strcat(XML, buf);
  sprintf(buf, "<B1>%d</B1>\n", BitsA1);
  strcat(XML, buf);
  sprintf(buf, "<V1>%d.%d</V1>\n", (int)(VoltsA1), abs((int)(VoltsA1 * 10) - ((int)(VoltsA1) * 10)));
  strcat(XML, buf);

  if (LED0) {
    strcat(XML, "<LED>1</LED>\n");
  } else {
    strcat(XML, "<LED>0</LED>\n");
  }

  if (SomeOutput) {
    strcat(XML, "<SWITCH>1</SWITCH>\n");
  } else {
    strcat(XML, "<SWITCH>0</SWITCH>\n");
  }

  strcat(XML, "<EMERGENCY_MODE>");
  if (emergencyShutdownActive) {
    strcat(XML, "1");
  } else {
    strcat(XML, "0");
  }
  strcat(XML, "</EMERGENCY_MODE>\n");
  strcat(XML, "<CORE0_STATUS>");
  if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING && uxTaskGetNumberOfTasks() > 0) {
    strcat(XML, "1");
  } else {
    strcat(XML, "0");
  }
  strcat(XML, "</CORE0_STATUS>\n");

  strcat(XML, "<CORE1_STATUS>");
  if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING && uxTaskGetNumberOfTasks() > 1) {
    // char a= Serial.read();
    // if(a == 'C'){
    //   strcat(XML, "0");
    // }
    strcat(XML, "1");
  } else {
    strcat(XML, "0");
  }
  strcat(XML, "</CORE1_STATUS>\n");
  // Append temperature and humidity data to XML
  strcat(XML, "<DHT_READINGS>\n");

  // Append temperature value
  strcat(XML, "<TEMP>");
  sprintf(buf, "%.2f", temperatureDHT);
  strcat(XML, buf);
  strcat(XML, "</TEMP>\n");

  // Append humidity value
  strcat(XML, "<HUMIDITY>");
  sprintf(buf, "%.2f", humidityDHT);
  strcat(XML, buf);
  strcat(XML, "</HUMIDITY>\n");

  strcat(XML, "</DHT_READINGS>\n");
  strcat(XML, "<TASK_COUNT>");
  strcat(XML, String(taskCount).c_str());
  strcat(XML, "</TASK_COUNT>\n");

  strcat(XML, "</Data>\n");
  Serial.println(XML);
  server.send(200, "text/xml", XML);
}

void ServerTask(void *pvParameters) {
  taskCount++;
  while (true) {
    // Update the server task core indicators
    server.handleClient();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}


void IR_Sensor_Function(void *pvParameters) {
  taskCount++;
//  pinMode(IR_SENSOR_PIN, INPUT);

  while (1) {
    // Read data from the analog IR sensor
    int irSensorValue ; //= analogRead(IR_SENSOR_PIN);

    // Process the sensor data
    if (irSensorValue < 500) {
      Serial.println("IR sensor detected an obstacle!");
    } else {
      Serial.println("No obstacle detected.");
    }
    vTaskDelay(pdMS_TO_TICKS(50));  // Adjust the delay as needed
  }
  
}

void DHT_Sensor_Function(void *pvParameters) {
  taskCount++;
  while (1) {
    temperatureDHT = hdc.readTemperature();
    humidityDHT = hdc.readHumidity();

    Serial.print("Task2 - Temperature: ");
    Serial.print(temperatureDHT);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidityDHT);
    Serial.println(" %");
    // Update the server with temperature and humidity values
    SendXML(); 
    Serial.println("Task2 - display is updated");
    vTaskDelay(pdMS_TO_TICKS(1000));  // Adjust the delay as needed
  }
}

void Emergency_Function(void *pvParameters) {
  taskCount++;
  //pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
//  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
//  pinMode(BUZZER_PIN, OUTPUT);

  while (1) {
    // Trigger ultrasonic sensor
//    digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
    delayMicroseconds(2);
//    digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
//    digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);

    // Read the ultrasonic sensor echo
    long duration; // = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
    // Calculate distance in centimeters
    float distance = duration * 0.034 / 2;
    Serial.print("Distance: ");
    Serial.println(distance);
    char a= Serial.read();
    // Check if an obstacle is detected (adjust the distance threshold as needed)
    if (distance < 20) {
    // if (a == 'E') {
      // Activate emergency mode only if not already active
      if (!emergencyShutdownActive) {
        emergencyShutdownActive = true;
        EmergencyShutdown();
//        digitalWrite(BUZZER_PIN, HIGH);
      }
    } else {
      // Deactivate emergency mode only if it was active
      if (emergencyShutdownActive) {
        emergencyShutdownActive = false;
//        digitalWrite(BUZZER_PIN, LOW);
        // Add any post-emergency code here if needed
      }
    }
    
    vTaskDelay(pdMS_TO_TICKS(100));  // Adjust the delay as needed
  }
}

void EmergencyShutdown() {
  // Execute emergency shutdown procedures
    Serial.println("Task3 - EMERGENCY protocol is being executed...");
  // Send XML data to update the server
  server.send(200, "text/plain", "");
}
