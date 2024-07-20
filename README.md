Welcome to the **ESPNOW Reflex Training Game**! This is a two-player game designed to enhance and measure your reflexes. Utilizing the Nano ESP32 board and the ESP-NOW protocol, the game features a master-slave architecture where players compete for the fastest reflex time.

The master ESP32 is connected to the controllers for both players, while the slave ESP32 manages the game display through an I2C LCD. This setup ensures a seamless and fun gaming experience.

Table of Contents:
- Features
- Hardware Requirements
- Software Requirements
- Wiring and Connections
- Installation and Setup
- Usage

### Features
Two-Player Mode: Players compete head-to-head on separate controllers.
Reflex Time Measurement: Accurately records and displays the reflex time of each player.
Real-Time Feedback: Visual feedback through an I2C LCD display.
Wireless Communication: Uses the ESP-NOW protocol for robust and low-latency communication.

### Hardware Requirements
2x Nano ESP32 Boards
1x I2C LCD Display
2x Breadboards
2x Push Buttons per Player (Total 4)
1x Potentiometer
Jumper Wires
USB Cables for programming

### Software Requirements
Arduino IDE
ESP32 Board Support Package
Libraries: esp_now, Wire, LiquidCrystal_I2C

### Wiring and Connections
Master Board
Push Buttons:
Button 1: GPIO 4
Button 2: GPIO 5
Potentiometer: Analog Pin A0
Slave Board
I2C LCD Display:
SDA: GPIO 21
SCL: GPIO 22

### Installation and Setup
Clone the Repository:

bash
Copy code
```
git clone https://github.com/username/reflex-training-game.git
cd reflex-training-game
```
Open Arduino IDE:

Install the ESP32 Board Support: ESP32 Board Installation Guide
Install required libraries from the Library Manager: esp_now, Wire, LiquidCrystal_I2C
Upload Code:

Open master/master.ino and upload to the master ESP32 board.
Open slave/slave.ino and upload to the slave ESP32 board.

### Usage
Power On: Connect both ESP32 boards to your computer or power source.
Game Start: Press the start button on the master controller.
Gameplay: The game will display prompts on the LCD. Players must press their button as quickly as possible in response to the prompt.
Results: The LCD will display the reaction times of both players, indicating the winner.

Developed with ❤️ by  _Vedant N Dasgupta_

Feel free to reach out if you have any questions or feedback. Happy gaming!
