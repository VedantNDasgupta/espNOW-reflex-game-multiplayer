#include <esp_now.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

// Create the LCD object address 0x27 and 16 columns x 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define CHANNEL 1
#define BUTTON_1 4 // Player 1 Start/Stop
#define BUTTON_2 6 // Player 2 Start/Stop
#define BUTTON_3 7 // Reset/Switch Turn
#define BUTTON_4 8 // Display Winner

bool startCountdown = false;
bool calculateTime = false;
bool player1Turn = true;
unsigned long startTime;
unsigned long reactionTimeP1;
unsigned long reactionTimeP2;

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  } else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

// config AP SSID
void configDeviceAP() {
  const char *SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
    Serial.print("AP CHANNEL "); Serial.println(WiFi.channel());
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESPNow Reaction Game");

  // Initialize the LCD
  lcd.begin();
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Reflex Test!");

  // Set device in AP mode to begin with
  WiFi.mode(WIFI_AP);
  // configure device AP mode
  configDeviceAP();
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for recv CB to get recv packet info.
  esp_now_register_recv_cb(OnDataRecv);

  // Configure button pins
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);
}

// callback when data is received from Master
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);

  uint8_t button1 = data[0]; // Player 1 Start/Stop
  uint8_t button2 = data[1]; // Player 2 Start/Stop
  uint8_t button3 = data[2]; // Reset/Switch Turn
  uint8_t button4 = data[3]; // Display Winner

  if (player1Turn) {
    if (button1 == 1 && !startCountdown && !calculateTime) {
      startCountdown = true;
    } else if (button1 == 1 && calculateTime) {
      reactionTimeP1 = millis() - startTime;
      calculateTime = false;
      lcd.clear();
      lcd.print("P1 Reaction:");
      lcd.setCursor(0, 1);
      lcd.print(reactionTimeP1);
      lcd.print(" milliseconds");
    }
  } else {
    if (button2 == 1 && !startCountdown && !calculateTime) {
      startCountdown = true;
    } else if (button2 == 1 && calculateTime) {
      reactionTimeP2 = millis() - startTime;
      calculateTime = false;
      lcd.clear();
      lcd.print("P2 Reaction:");
      lcd.setCursor(0, 1);
      lcd.print(reactionTimeP2);
      lcd.print(" milliseconds");
    }
  }

  if (button3 == 1 && !startCountdown && !calculateTime) {
    switchTurn();
  }

  if (button4 == 1 && reactionTimeP1 != 0 && reactionTimeP2 != 0) {
    displayWinner();
  }
}

void switchTurn() {
  startCountdown = false;
  calculateTime = false;
  lcd.clear();
  player1Turn = !player1Turn;
  if (player1Turn) {
    lcd.print("P1: Press Btn1");
    lcd.setCursor(0, 1);
    lcd.print("to Start");
  } else {
    lcd.print("P2: Press Btn2");
    lcd.setCursor(0, 1);
    lcd.print("to Start");
  }
}

void displayWinner() {
  lcd.clear();
  lcd.print("Winner is:");
  lcd.setCursor(0, 1);
  if (reactionTimeP1 < reactionTimeP2) {
    lcd.print("Player 1");
  } else if (reactionTimeP2 < reactionTimeP1) {
    lcd.print("Player 2");
  } else {
    lcd.print("Draw!");
  }
}

void loop() {
  if (startCountdown) {
    for (int i = 5; i > 0; i--) {
      lcd.clear();
      lcd.print("Starting in: ");
      lcd.setCursor(0, 1);
      lcd.print(i);
      delay(1000); // 1-second countdown
    }
    lcd.clear();
    lcd.print("GO!");
    startCountdown = false;
    calculateTime = true;
    startTime = millis();
  }

  if (calculateTime) {
    lcd.setCursor(0, 1);
    lcd.print("Waiting for press");
  }
}
