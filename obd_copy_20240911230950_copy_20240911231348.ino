#define BLYNK_TEMPLATE_ID "TMPL3_ra5pkNO"
#define BLYNK_TEMPLATE_NAME "strain"
#define BLYNK_AUTH_TOKEN "_GCumzvc5d2yjaG0gyHWfvoptHnqR-vm"
#include <Wire.h>
#include <SPI.h>
#include <mcp2515.h>
#include <BluetoothSerial.h>

// Replace with your Blynk authentication token
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";

// Set up MCP2515 CAN controller
#define MCP2515_CS_PIN 5  // Define the Chip Select pin
#define MCP2515_INT_PIN 4 // Define the Interrupt pin (optional)

MCP2515 mcp2515(MCP2515_CS_PIN); // Create MCP2515 object with CS pin

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_OBD"); // Bluetooth name

  if (!mcp2515.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ)) {
    Serial.println("MCP2515 initialization failed");
    while (1);
  }

  Serial.println("MCP2515 initialized");
}

void loop() {
  Blynk.run(); // Keep Blynk running

  // Check if a new CAN message is available
  if (mcp2515.readMessage()) {
    CAN_Message message = mcp2515.getLastMessage(); // Get the last received message

    if (message.id == 0x123) { // Example ID for RPM
      int rpm = (message.data[0] << 8) | message.data[1];
      SerialBT.print("RPM: ");
      SerialBT.println(rpm);
    }

    if (message.id == 0x456) { // Example ID for Speed
      int speed = message.data[0];
      SerialBT.print("Speed: ");
      SerialBT.println(speed);
    }
  }

  delay(100); // Small delay
}