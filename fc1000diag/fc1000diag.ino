/**
 * @file fc1000diag.ino
 * @brief fc1000 diagnostic mode
 * @author JJ1VQD
 * @date 25-10-12
 */
#include <WiFi.h>
#include <NetworkClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "MyWiFiSettings.h"
#include "MyWebserver.h"
#include "Adafruit_HDC1000.h"
#include <MCP23017.h>
#include <Wire.h>
#include <MCP23017.h>

/* Definition */
#define PIN_A0 0  //! Forward Power
#define PIN_A1 1  //! Refrect Power

#define MCP23017_ADDR_1 0x20
#define MCP23017_ADDR_2 0x21

/* Actual data stored in WiFiSettings.h */
//const char *ssid = "YOUR_SSID";
//const char *password = "YOUR_PASSWD";

/* Macros */
/* Type Definition */
/* Enumlation */
/* Structure Definition */
struct bf {
  uint8_t b0 : 1;
  uint8_t b1 : 1;
  uint8_t b2 : 1;
  uint8_t b3 : 1;
  uint8_t b4 : 1;
  uint8_t b5 : 1;
  uint8_t b6 : 1;
  uint8_t b7 : 1;
};

union bm {
  uint8_t   byte;
  bf  bits;
};

/* Prototypes */
void ADC_Function(void *pvParameters);
void UpdateSlider();
void SendXML();
void SendWebsite();
void ProcessButton_0();
void ProcessButton_1();
void ProcessButton_101();
void ProcessButton_103();
void ProcessButton_104();
void ProcessButton_105();
void ProcessButton_106();
void ProcessButton_107();
void ProcessButton_108();
void ProcessButton_109();
void ProcessButton_110();
void ProcessButton_111();
void SensorTest();
void SerCapTest();
void SerIndTest();
void ParIndTest();

/* Global Variables */
MCP23017 mcp1 = MCP23017(MCP23017_ADDR_1);
MCP23017 mcp2 = MCP23017(MCP23017_ADDR_2);
Adafruit_HDC1000 hdc = Adafruit_HDC1000();
WebServer server(80);

bm sercap;  //! C-SECTION
bm serind;  //! LOADING COIL
bm parind;  //! L-SECTION
bm sensor;  //! SENSOR
bm rytest;  //! Relay Test

TaskHandle_t mainTask;            //! main task
TaskHandle_t serverTask;          //! Web Server task
TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL;
TaskHandle_t Task3Handle = NULL;

int BitsA0 = 0, BitsA1 = 0;
float FwdPower = 0, RefPower = 0;
uint32_t SensorUpdate = 0;
int TestDelay = 1000;
bool LED0 = false, SomeOutput = false;
bool emergencyShutdownActive = false;
float temperature = 0.0;
float humidity = 0.0;
int taskCount = 0;
int idleCount = 0;

char XML[2048];
char buf[32];

/**
 * @fn
 * 404 File Not Found Handler
 * @brief 404 Handler
 * @param None
 * @return None
 * @detail File Not Found Handler
 */
void handleNotFound() {
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
}

/**
 * @fn
 * FreeRTOS Setup Function
 * @brief Setup Function
 * @param None
 * @return None
 * @detail FreeRTOS setup function
 */
void setup(void) {
  Wire.begin();
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

  mcp1.init();
  mcp2.init();

  mcp1.portMode(MCP23017Port::A, 0);    //! Dev0 Port A as output
  mcp1.portMode(MCP23017Port::B, 0);    //! Dev0 Port B as output
  mcp2.portMode(MCP23017Port::A, 0);    //! Dev1 Port A as output
  mcp2.portMode(MCP23017Port::B, 0xff); //! Dev1 Port B as input

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

  server.on("/BUTTON_105", ProcessButton_105);  
  server.on("/BUTTON_106", ProcessButton_106);  
  server.on("/BUTTON_107", ProcessButton_107);  
  server.on("/BUTTON_108", ProcessButton_108);  
  server.on("/BUTTON_109", ProcessButton_109);  
  server.on("/BUTTON_110", ProcessButton_110);  
  server.on("/BUTTON_111", ProcessButton_111);  

  server.on("/BUTTON_118", ProcessButton_118);  
  server.on("/BUTTON_119", ProcessButton_119);  
  server.on("/BUTTON_120", ProcessButton_120);  
  server.on("/BUTTON_121", ProcessButton_121);  
  server.on("/BUTTON_122", ProcessButton_122);  
  server.on("/BUTTON_103", ProcessButton_103);  
  server.on("/BUTTON_104", ProcessButton_104);  

  server.on("/BUTTON_112", ProcessButton_112);  
  server.on("/BUTTON_113", ProcessButton_113);  
  server.on("/BUTTON_114", ProcessButton_114);  
  server.on("/BUTTON_115", ProcessButton_115);  
  server.on("/BUTTON_116", ProcessButton_116);  
  server.on("/BUTTON_117", ProcessButton_117);  
  server.on("/BUTTON_101", ProcessButton_101);  

  server.on("/BUTTON_SENSOR_TEST", SensorTest);  
  server.on("/BUTTON_SERCAP_TEST", SerCapTest);  
  server.on("/BUTTON_SERIND_TEST", SerIndTest);  
  server.on("/BUTTON_PARIND_TEST", ParIndTest);  

  server.begin();
  Serial.println("HTTP server started");

  xTaskCreateUniversal(ADC_Function,            "mainTask",   4096, NULL, 1, &mainTask, 0);
  // Create FreeRTOS tasks
  xTaskCreateUniversal(Test_Interval_Function,  "Task1",      4096, NULL, 2, &Task1Handle, 0);
  xTaskCreateUniversal(HDC_Sensor_Function,     "Task2",      4096, NULL, 2, &Task2Handle, 0);
  xTaskCreateUniversal(RelayControl_Function,   "Task3",      4096, NULL, 3, &Task3Handle, 0);
  // Create a server task pinned to core 1
  xTaskCreateUniversal(ServerTask,              "serverTask", 8192, NULL, 3, &serverTask, 1);
}

/**
 * @fn
 * FreeRTOS loop Function
 * @brief loop Function
 * @param None
 * @return None
 * @detail FreeRTOS loop function
 */
void loop(void) {
  idleCount++;
//  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks
}

float getSwr(float fwdpower, float refpower) {
  float swr;
  swr = (sqrt(fwdpower) + sqrt(refpower)) / (sqrt(fwdpower) - sqrt(refpower));
  return swr;
}

/**
 * @fn
 * Get FWD/REF Power
 * @brief get fwd/ref power Function
 * @param None
 * @return None
 * @detail
 */
void ADC_Function(void *pvParameters) {
  taskCount++;
  while (true) {
    if ((millis() - SensorUpdate) >= 50) {
      SensorUpdate = millis();
      BitsA0 = analogRead(PIN_A0);
      BitsA1 = analogRead(PIN_A1);
      FwdPower = BitsA0 * 3.3 / 4096;
      RefPower = BitsA1 * 3.3 / 4096;
    }
    vTaskDelay(pdMS_TO_TICKS(1000));  // Adjust the delay as needed
  }
}

void UpdateSlider() {
  String t_state = server.arg("VALUE");
  TestDelay = t_state.toInt();
  Serial.print("UpdateSlider ");
  Serial.println(TestDelay);

  // Map TestDelay to LED brightness
  int ledBrightness = map(TestDelay, 100, 5000, 100, 5000);

  // Respond with the updated RPM value
  server.send(200, "text/plain", String(TestDelay));
}

void ProcessButton_0() {
  Serial.print("Button 0 ");
  server.send(200, "text/plain", "");
}

void ProcessButton_1() {
  rytest.bits.b5 = !rytest.bits.b5;
  Serial.print("Button 1 ");
  server.send(200, "text/plain", "");
}

void ProcessButton_105() {
  sercap.bits.b0 = !sercap.bits.b0;
  Serial.print("Button 105 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_106() {
  sercap.bits.b1 = !sercap.bits.b1;
  Serial.print("Button 106 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_107() {
  sercap.bits.b2 = !sercap.bits.b2;
  Serial.print("Button 107 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_108() {
  sercap.bits.b3 = !sercap.bits.b3;
  Serial.print("Button 108 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_109() {
  sercap.bits.b4 = !sercap.bits.b4;
  Serial.print("Button 109 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_110() {
  sercap.bits.b5 = !sercap.bits.b5;
  Serial.print("Button 110 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_111() {
  sercap.bits.b6 = !sercap.bits.b6;
  Serial.print("Button 111 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}

void ProcessButton_118() {
  serind.bits.b0 = !serind.bits.b0;
  Serial.print("Button 118 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_119() {
  serind.bits.b1 = !serind.bits.b1;
  Serial.print("Button 119 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_120() {
  serind.bits.b2 = !serind.bits.b2;
  Serial.print("Button 120 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_121() {
  serind.bits.b3 = !serind.bits.b3;
  Serial.print("Button 121 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_122() {
  serind.bits.b4 = !serind.bits.b4;
  Serial.print("Button 122 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_103() {
  serind.bits.b6 = !serind.bits.b6;
  Serial.print("Button 103 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_104() {
  serind.bits.b7 = !serind.bits.b7;
  Serial.print("Button 1104 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}

void ProcessButton_112() {
  parind.bits.b0 = !parind.bits.b0;
  Serial.print("Button 112 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_113() {
  parind.bits.b1 = !parind.bits.b1;
  Serial.print("Button 113 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_114() {
  parind.bits.b2 = !parind.bits.b2;
  Serial.print("Button 114 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_115() {
  parind.bits.b3 = !parind.bits.b3;
  Serial.print("Button 115 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_116() {
  parind.bits.b4 = !parind.bits.b4;
  Serial.print("Button 116 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_117() {
  parind.bits.b5 = !parind.bits.b5;
  Serial.print("Button 117 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}
void ProcessButton_101() {
  parind.bits.b7 = !parind.bits.b7;
  Serial.print("Button 101 ");
  Serial.println(sercap.byte);
  server.send(200, "text/plain", "");
}

void SensorTest() {
  rytest.bits.b0 = !rytest.bits.b0;
  Serial.print("Button Sensor Test ");
  Serial.println(rytest.byte);
  server.send(200, "text/plain", "");
}
void SerCapTest() {
  rytest.bits.b1 = !rytest.bits.b1;
  Serial.print("Button SerCap Test ");
  Serial.println(rytest.byte);
  server.send(200, "text/plain", "");
}
void SerIndTest() {
  rytest.bits.b2 = !rytest.bits.b2;
  Serial.print("Button SerInd Test ");
  Serial.println(rytest.byte);
  server.send(200, "text/plain", "");
}
void ParIndTest() {
  rytest.bits.b3 = !rytest.bits.b3;
  Serial.print("Button ParInd Test ");
  Serial.println(rytest.byte);
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
  sprintf(buf, "<V0>%d.%d</V0>\n", (int)(FwdPower), abs((int)(FwdPower * 10) - ((int)(FwdPower) * 10)));
  strcat(XML, buf);
  sprintf(buf, "<B1>%d</B1>\n", BitsA1);
  strcat(XML, buf);
  sprintf(buf, "<V1>%d.%d</V1>\n", (int)(RefPower), abs((int)(RefPower * 10) - ((int)(RefPower) * 10)));
  strcat(XML, buf);

  if (LED0) {
    strcat(XML, "<LED>1</LED>\n");
  } else {
    strcat(XML, "<LED>0</LED>\n");
  }

  sprintf(buf, "<SENSOR>00%0d%0d%0d%0d%0d%0d</SENSOR>\n", 
    sensor.bits.b5, sensor.bits.b4, sensor.bits.b3, sensor.bits.b2,
    sensor.bits.b1, sensor.bits.b0);
  strcat(XML, buf);

  sprintf(buf, "<SERCAP>0%0d%0d%0d%0d%0d%0d%0d</SERCAP>\n", 
    sercap.bits.b6, sercap.bits.b5, sercap.bits.b4, sercap.bits.b3,
    sercap.bits.b2, sercap.bits.b1, sercap.bits.b0);
  strcat(XML, buf);

  sprintf(buf, "<SERIND>%0d%0d0%0d%0d%0d%0d%0d</SERIND>\n", 
    serind.bits.b7, serind.bits.b6, serind.bits.b4, serind.bits.b3,
    serind.bits.b2, serind.bits.b1, serind.bits.b0);
  strcat(XML, buf);

  sprintf(buf, "<PARIND>%0d0%0d%0d%0d%0d%0d%0d</PARIND>\n", 
    parind.bits.b7, parind.bits.b5, parind.bits.b4, parind.bits.b3,
    parind.bits.b2, parind.bits.b1, parind.bits.b0);
  strcat(XML, buf);

  sprintf(buf, "<TEST>0000%0d%0d%0d%0d</TEST>\n", 
    rytest.bits.b3, rytest.bits.b2, rytest.bits.b1, rytest.bits.b0);
  strcat(XML, buf);

  if (rytest.bits.b5 == 1) {
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
    // if(a == 'C') {
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
  sprintf(buf, "%.2f", temperature);
  strcat(XML, buf);
  strcat(XML, "</TEMP>\n");

  // Append humidity value
  strcat(XML, "<HUMIDITY>");
  sprintf(buf, "%.2f", humidity);
  strcat(XML, buf);
  strcat(XML, "</HUMIDITY>\n");

  strcat(XML, "</DHT_READINGS>\n");

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

void Test_Interval_Function(void *pvParameters) {
  taskCount++;
//  pinMode(IR_SENSOR_PIN, INPUT);

  while (1) {
    if (rytest.bits.b0 == 1) {
      sensor.byte++;
    }
    if (rytest.bits.b1 == 1) {
      sercap.byte++;
    }
    if (rytest.bits.b2 == 1) {
      serind.byte++;
    }
    if (rytest.bits.b3 == 1) {
      parind.byte++;
    }

    vTaskDelay(pdMS_TO_TICKS(TestDelay));  // Adjust the delay as needed
  }
}

void HDC_Sensor_Function(void *pvParameters) {
  taskCount++;
  while (1) {
    temperature = hdc.readTemperature();
    humidity = hdc.readHumidity();

    Serial.print("Task2 - Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    // Update the server with temperature and humidity values
    SendXML(); 
    Serial.println("Task2 - display is updated");

    vTaskDelay(pdMS_TO_TICKS(1000));  // Adjust the delay as needed
  }
}

void RelayControl_Function(void *pvParameters) {
  taskCount++;

  while (1) {
    mcp1.writeRegister(MCP23017Register::GPIO_A, sercap.byte);  //Reset port A 
    mcp1.writeRegister(MCP23017Register::GPIO_B, serind.byte);  //Reset port B
    mcp2.writeRegister(MCP23017Register::GPIO_A, parind.byte);  //Reset port A 
    sensor.byte = mcp2.readPort(MCP23017Port::B);

    vTaskDelay(pdMS_TO_TICKS(200));  // Adjust the delay as needed
  }
}

void EmergencyShutdown() {
  // Execute emergency shutdown procedures
    Serial.println("Task3 - EMERGENCY protocol is being executed...");
  // Send XML data to update the server
  server.send(200, "text/plain", "");
}
