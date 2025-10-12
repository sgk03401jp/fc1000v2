// Controls an LED via an attached button.
#include "Wire.h"

#include <MCP23017.h>

#define ON  1
#define OFF 0

#define LED_PIN 0     // MCP23XXX pin LED is attached to
#define BUTTON_PIN 1  // MCP23XXX pin button is attached to

// only used for SPI
#define CS_PIN 6

//   b0 : RL105
//   b1 : RL106
//   b2 : RL107
//   b3 : RL108
//   b4 : RL109
//   b5 : RL110
//   b6 : RL111
//   b7 : 

//   b0 : RL118
//   b1 : RL119
//   b2 : RL120
//   b3 : RL121
//   b4 : RL122
//   b5 : 
//   b6 : RL103
//   b7 : RL104

//   b0 : RL112
//   b1 : RL113
//   b2 : RL114
//   b3 : RL115
//   b4 : RL116
//   b5 : RL117
//   b6 : 
//   b7 : RL101,RL102


// ビットフィールド構造体の定義
struct bitfield {
  uint8_t b0 : 1;
  uint8_t b1 : 1;
  uint8_t b2 : 1;
  uint8_t b3 : 1;
  uint8_t b4 : 1;
  uint8_t b5 : 1;
  uint8_t b6 : 1;
  uint8_t b7 : 1;
};

// ビットフィールドとバイトを共有するunion
union SERCAP {
  uint8_t   byte;
  bitfield  bits;
};

union SERIND {
  uint8_t   byte;
  bitfield  bits;
};

union PARIND {
  uint8_t   byte;
  bitfield  bits;
};

MCP23017 mcp1 = MCP23017(0x20);
MCP23017 mcp2 = MCP23017(0x21);

SERCAP sercap;
SERIND serind;
PARIND parind;

void scan();

void setup() {
  // put your setup code here, to run once:
  String board;
  int ret;
  
  delay(1000);

  Serial.begin(115200);
  Wire.begin();
  mcp1.init();
  mcp2.init();

  mcp1.portMode(MCP23017Port::A, 0);          //Port 1A as output
  mcp1.portMode(MCP23017Port::A, 0);          //Port 1B as output
  mcp2.portMode(MCP23017Port::A, 0);          //Port 2A as output
  mcp2.portMode(MCP23017Port::B, 0b11111111); //Port 2B as input
  
  while (!Serial); // Wait for starting serial monitor

#if CONFIG_IDF_TARGET_ESP32
  // ESP32
  board="ESP32";
#elif CONFIG_IDF_TARGET_ESP32S2
  // ESP32-S2
  board="ESP32S2";
#elif CONFIG_IDF_TARGET_ESP32S3
  // ESP32-S3
  board="ESP32S3";
#elif CONFIG_IDF_TARGET_ESP32C3
  // ESP32-C3
  board="ESP32C3";
#define I2C_SDA 8
#define I2C_SCL 9
#elif CONFIG_IDF_TARGET_ESP32H2
  // ESP32-H2
  board="ESP32H2";
#else
  // Other
  board="Other";
#endif
  Serial.print("ESP32 : ");
  Serial.println(board);

  Serial.println("\nI2C Scanner");

  ret = Wire.begin(I2C_SDA, I2C_SCL);
 
  Serial.print("I2C Init : ");
  Serial.println(ret);

  i2cScan();
  dumpRegisters(mcp1);
  dumpRegisters(mcp2);

  Serial.println("Looping...");

  mcp2.writeRegister(MCP23017Register::GPIO_A, 0x00);  //Reset port A 
  mcp2.writeRegister(MCP23017Register::GPIO_B, 0x00);  //Reset port B

  // GPIO_B reflects the same logic as the input pins state
  mcp2.writeRegister(MCP23017Register::IPOL_B, 0x00);
  // Uncomment this line to invert inputs (press a button to lit a led)
  //mcp.writeRegister(MCP23017Register::IPOL_B, 0xFF);
#if 0
  // 初期化：全ビットを0に
  sercap.byte = 0x00;

  // ビットに名前を付けて書き込み
  sercap.bits.b0 = 1;  // LSB
  sercap.bits.b3 = 1;
  sercap.bits.b7 = 1;  // MSB

  // 結果表示
  Serial.print("Byte value: 0b");
  for (int i = 7; i >= 0; i--) {
    Serial.print((sercap.byte >> i) & 1);
  }
  Serial.println();

  Serial.print("Hex value: 0x");
  Serial.println(sercap.byte);

  // 個別ビットの読み取り
  Serial.print("bit3 is ");
  Serial.println(sercap.bits.b3 ? "ON" : "OFF");
#endif
}

void loop() {
  uint8_t currentB;

  // Copy Port B to Port A
  currentB = mcp2.readPort(MCP23017Port::B);
  mcp2.writePort(MCP23017Port::A, currentB);

  mcp1aTest();
  mcp1bTest();
  mcp2aTest();
  mcp2bTest();

  delay(5000); 
}

void i2cScan() {
  // put your main code here, to run repeatedly:
  byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
      delay(500);
    } else if (error==4) {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  //delay(5000); // Wait 5 seconds for next scan
}

void dumpRegisters(MCP23017 mcp) {
	uint8_t conf = mcp.readRegister(MCP23017Register::IODIR_A);
	Serial.print("IODIR_A : ");
	Serial.print(conf, BIN);
	Serial.println();
	
	conf = mcp.readRegister(MCP23017Register::IODIR_B);
	Serial.print("IODIR_B : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::IPOL_A);
	Serial.print("IPOL_A : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::IPOL_B);
	Serial.print("IPOL_B : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::GPINTEN_A);
	Serial.print("GPINTEN_A : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::GPINTEN_B);
	Serial.print("GPINTEN_B : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::DEFVAL_A);
	Serial.print("DEFVAL_A : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::DEFVAL_B);
	Serial.print("DEFVAL_B : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::INTCON_A);
	Serial.print("INTCON_A : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::INTCON_B);
	Serial.print("INTCON_B : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::IOCON);
	Serial.print("IOCON : ");
	Serial.print(conf, BIN);
	Serial.println();

	//conf = mcp.readRegister(IOCONB);
	//Serial.print("IOCONB : ");
	//Serial.print(conf, BIN);
	//Serial.println();

	conf = mcp.readRegister(MCP23017Register::GPPU_A);
	Serial.print("GPPU_A : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::GPPU_B);
	Serial.print("GPPU_B : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::INTF_A);
	Serial.print("INTF_A : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::INTF_B);
	Serial.print("INTF_B : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::INTCAP_A);
	Serial.print("INTCAP_A : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::INTCAP_B);
	Serial.print("INTCAP_B : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::GPIO_A);
	Serial.print("GPIO_A : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::GPIO_B);
	Serial.print("GPIO_B : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::OLAT_A);
	Serial.print("OLAT_A : ");
	Serial.print(conf, BIN);
	Serial.println();

	conf = mcp.readRegister(MCP23017Register::OLAT_B);
	Serial.print("OLAT_B : ");
	Serial.print(conf, BIN);
	Serial.println();
}

// SERCAP
void mcp1aTest() {
  int b = 0;
  for (int i = 0; i <= 7; i++) {
    b = 2 ^ i;
    mcp1.writePort(MCP23017Port::A, b);
    delay(2000); 
  }
}

// SERIND 0-4,67
void mcp1bTest() {
  int b = 0;
  for (int i = 0; i <= 4; i++) {
    b = 2 ^ i;
    mcp1.writePort(MCP23017Port::B, b);
    delay(2000); 
  }
  mcp1.writePort(MCP23017Port::B, b);
  delay(2000); 
}

// PARIND 0-5,7
void mcp2aTest() {
  for (int i = 0; i <= 5; i++) {
    b = 2 ^ i;
    mcp2.writePort(MCP23017Port::A, b);
    delay(2000); 
  }
  mcp2.writePort(MCP23017Port::A, b & parind.bits.b7);
  delay(2000); 
}

// DETECT
void mcp2bTest() {
  uint8_t b;
  b = mcp2.readRegister(MCP23017Register::IODIR_B);
  Serial.printf("Read : %x", b)
}