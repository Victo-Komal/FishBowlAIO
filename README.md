
# FishBowlAIO



FishBowlAIO for Study Room Analytics


## Authors


- [@Victo-Komal](https://www.github.com/Victo-Komal)
- @Little-Devil
- [@Hestur](https://github.com/Hestur)
## Overview of files

#### FishBowlAIO/firmware/DB-generator.py:
Python file that generates the mqtt_entries.db database file.

#### FishBowlAIO/firmware/mqtt_entries.db:
Database file used for storing incoming values from the FishBowlAIO product.

#### FishBowlAIO/firmware/tt.html
HTML file for testing.

#### FishBowlAIO/firmware/firmware.ino
Firmware file for the FishBowlAIO.

#### FishBowlAIO/webserver.py
Python file for starting the web server locally.

#### FishBowlAIO/updater.sh
Shell script used for updating and fetching files.

## Installation

1. Download the repository

From here on there's two different installation methods:


Method 1:

Download the updater.sh script and run in the terminal. updater.sh will install and update the dependencies used by the FishBowlAIO product as well as the firmware.
Ignore step 2, 3 and 4 if this is choosen.

Method 2:

Open the terminal and paste in the following command:
```
sudo apt-get install git
git clone http://github.com/Victo-Komal/FishBowlAIO.git
```

2. Install Python3 
```
sudo apt install python3-pip
pip3 install --upgrade pip
pip3 install paho-mqtt
```
3. Install Snap repository (Ignore if updater script is used)
```
sudo apt install snapd
```
4. Install PyCharm Community Edition. (Ingore if updater script is used)
```
snap install pycharm-community --classic
```
5. Install Arduino IDE to handle the C files. (Ignore if updater script is used)
```
sudo apt-get install arduino
```

6. Installing ESP32 Add-on in Arduino IDE

In your Arduino IDE, go to File> Preferences


Installing ESP32 Add-on in Arduino IDE open preferences
Enter the following into the “Additional Board Manager URLs” field:

https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

7. Install ppaho-mqtt in PyCharm terminal if not installed automatic

In your PyCharm IDE, Click the terminal in the bottom of the window and type in:
```
pip3 install paho-mqtt
```
## Updating firmware

If the product needs to be updated, the following must be done:

1. Get the updated files (see "Installation" section)

2. Firmware upgrade for ESP32-Wrover:
Upload the updated firmware.ino file to the ESP32 through Arduino IDE.

3. Template upgrade for tt.html, webserver.py, DB-generator.py:
Replace the files in the folder with the updated files.
## Dependencies

Here we listed the dependencies for the project:

```bash
sudo apt-get update
sudo apt-get install git
sudo apt install python3-pip
sudo apt-get install arduino
pip3 install --upgrade pip
pip3 install paho-mqtt
sudo apt install snapd
snap install pycharm-community --classic
git clone http://github.com/Victo-Komal/FishBowlAIO.git
```
    
