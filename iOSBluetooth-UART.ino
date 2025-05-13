#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic* pTxCharacteristic;
bool deviceConnected = false;
int txValue = 0;

const int button = 0;      // button on PIN G0
const int readPin = 32;    // analog pin G32
const int LEDpin = 2;      // LED on pin G2
bool convert = false;
String rxString = "";
std::string rxValue;

// UART service UUIDs
#define SERVICE_UUID            "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" 
#define CHARACTERISTIC_UUID_RX  "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX  "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue().c_str();
    if (rxValue.length() > 0) {
      convert = true;
      rxString = String(rxValue.c_str()); // Clean reset and conversion
      Serial.println();
      Serial.print("Received data: ");
      Serial.println(rxString);
    }
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(LEDpin, OUTPUT);
  pinMode(button, INPUT);

  BLEDevice::init("Subu's ESP32 UART");

  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService* pService = pServer->createService(SERVICE_UUID);

  // TX characteristic
  pTxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pTxCharacteristic->addDescriptor(new BLE2902());

  // RX characteristic
  BLECharacteristic* pRxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_RX,
    BLECharacteristic::PROPERTY_WRITE
  );
  pRxCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();
  pServer->getAdvertising()->start();

  Serial.println("Waiting for a client to connect...");
}

void loop() {
  if (deviceConnected) {
    txValue = analogRead(readPin);
    char txString[8];
    dtostrf(txValue, 1, 2, txString); // Convert int to string format

    if (digitalRead(button) == LOW) {
      pTxCharacteristic->setValue(txString);
      pTxCharacteristic->notify();
      pTxCharacteristic->setValue(" ");
      pTxCharacteristic->notify();

      digitalWrite(LEDpin, HIGH);

      Serial.println();
      Serial.print("*** Peripheral sends: ");
      Serial.print(txString);
      Serial.println(" ***");
    } else {
      digitalWrite(LEDpin, LOW);
    }

    if (convert) {
      convertControlpad();
    }
  }

  delay(50);
}

void convertControlpad() {
  convert = false;
  Serial.print("      ");
  if (rxString == "!B11:") Serial.println("********** Start Action 1");
  else if (rxString == "!B219") Serial.println("********** Start Action 2");
  else if (rxString == "!B318") Serial.println("********** Start Action 3");
  else if (rxString == "!B417") Serial.println("********** Start Action 4");
  else if (rxString == "!B516") Serial.println("********** Start Action UP");
  else if (rxString == "!B615") Serial.println("********** Start Action DOWN");
  else if (rxString == "!B714") Serial.println("********** Start Action LEFT");
  else if (rxString == "!B813") Serial.println("********** Start Action RIGHT");
  else if (rxString == "!B10;") Serial.println("********** Stop Action 1");
  else if (rxString == "!B20:") Serial.println("********** Stop Action 2");
  else if (rxString == "!B309") Serial.println("********** Stop Action 3");
  else if (rxString == "!B408") Serial.println("********** Stop Action 4");
  else if (rxString == "!B507") Serial.println("********** Stop Action UP");
  else if (rxString == "!B606") Serial.println("********** Stop Action DOWN");
  else if (rxString == "!B705") Serial.println("********** Stop Action LEFT");
  else if (rxString == "!B804") Serial.println("********** Stop Action RIGHT");

  rxString = "";
}
