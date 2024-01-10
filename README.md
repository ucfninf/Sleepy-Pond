# Sleep Environment Monitor

## Overview

Sleepy Pond is a sleep monitoring device designed to track and analyze your sleep patterns, providing valuable insights into the quality of your sleep. By using various sensors and advanced algorithms, Sleepy Pond aims to help users improve their sleep hygiene and overall well-being.
![IMG_0893](https://github.com/ucfninf/Sleepy-Pond/assets/146268411/b3a43c27-afd1-41b5-9c3e-3a8f9146c1a9)

## Features

- Integration of temperature, humidity, light intensity, and noise sensors
- Real-time feedback mechanism using Neo Pixel Strips
- Threshold alerts for optimal sleep conditions
- Aesthetic and calming enclosure inspired by Asian water ponds
  
## Material List
To build the Sleep Environment Detector, you will need the following materials:

Sensor: DHT22 (Temperature and Humidity)
Sensor: MAX4466 (Noise)
Sensor: Photoresistor (Light Intensity)
Board: Arduino Uno (Microcontroller)

## Design, Build and Installation

1. Clone this repository to your local machine.

```bash
git clone https://github.com/your-username/sleep-environment-detector.git
```

2. Ensure you have the required Arduino IDE installed on your computer.

3. Connect the sensors and Neo Pixel Strips to your Arduino Uno following the provided circuit diagrams.

4. Upload the `DIY_Monitor.ino` file to your Arduino Uno using the Arduino IDE. slight adjustment needed if your sensor is not MAX4466.
   ![circuit display](https://github.com/ucfninf/Sleepy-Pond/assets/146268411/93cda25c-6b24-444a-8331-b72fac1f32a4)
Attached wire diagram

## Usage

1. Power up your Sleep Environment Detector.

2. Observe the Neo Pixel Strips for real-time feedback on your sleep environment.

3. Refer to the [Feedback Mechanism](#feedback-mechanism) section for understanding the meaning of different lighting patterns.

## Feedback Mechanism

The Sleep Environment Detector utilizes Neo Pixel Strips to provide feedback on environmental conditions:

- **Noise Level:** Red lights indicate high noise levels.
- **Humidity Level:** Blue lights for low humidity, green lights for optimal humidity, and red lights for high humidity.
- **Temperature:** Blue lights for low temperatures, green lights for optimal temperatures, and red lights for high temperatures.
- **Light Intensity:** The brightness of the Neo Pixel Strips corresponds to the ambient light intensity.

## Enclosure Design

The enclosure design is inspired by Asian water ponds, providing a visually appealing and calming aesthetic to your sleep space.

1. Assemble the sensor system, LED lights, and sound module into the housing.

2. Ensure that all components work seamlessly together.

3. Organize the wiring using a twisting rod for a neat appearance.

## Future Improvements

1.Enhanced Sensor Sensitivity: Investigate and implement sensors with higher sensitivity, such as LM393, which detects sound intensity via a microphone. This will address the current limitations in detecting sudden sounds.

2. Expanded Detection Range: Modify the system to detect environmental conditions over a larger area, enhancing the versatility of the sleep monitor.

3. Advanced LED Light Effects: Introduce LED lights with adjustable color temperature and brightness to simulate the light of a sauna or Japanese garden, creating a more immersive and tranquil atmosphere.

4. Integrated Sound Effect System: Incorporate a sound effect system that plays natural sounds, such as running water, birdsong, and steaming sauna steam. Users can choose sound effects according to their preferences, enhancing the overall sleep environment.


Happy Sleeping with Sleepy Pond! 🌙💤✨
