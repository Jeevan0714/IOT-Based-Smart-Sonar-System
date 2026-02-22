# IoT-Based Smart Automated Sonar System

## 📖 Overview
The IoT Based Smart Automated SONAR System is designed to provide continuous, non-contact distance monitoring and transmit real-time data wirelessly [8]. Built to overcome the limitations of fragile wired sensors, this system executes automated, event-triggered alerts upon the breach of a predefined safety threshold [8]. By integrating a rugged, waterproof ultrasonic sensor with a Wi-Fi-enabled microcontroller, the system offers a scalable solution for remote fluid level sensing and security perimeter detection [159, 584].

## 🛑 The Problem It Solves
This project was engineered to address critical flaws in current distance monitoring methodologies [55, 60]:
* **Fragility of Entry-Level Systems**: Basic wired systems use cheap sensors (like the HC-SR04) that easily fail when exposed to moisture, humidity, or temperature variations [88, 127]. They also rely on a constant physical USB connection, restricting them to immobile indoor environments [87, 122].
* **Cost and Complexity of Industrial Setups**: High-end industrial systems utilizing Programmable Logic Controllers (PLCs) are prohibitively expensive, require specialized training, and often lack native integration with modern, user-friendly IoT cloud platforms [89, 105, 140].
* **Inefficiency of Camera-Only Surveillance**: Visual monitoring fails in low-light, fog, or when obscured by debris [91, 149]. Furthermore, extracting distance data from video streams requires power-hungry computer vision algorithms that drain batteries and network bandwidth [91, 150].

## 👥 The Team
Developed by 5th-semester Electronics and Communication Engineering students at Don Bosco Institute of Technology [546, 563]:
* **Jeevan R** [548]
* **Kushal C** [550]
* **Kushal NS** [552]
* **Pavan K** [554]

## 🛠️ Hardware Architecture



The physical build prioritizes durability and autonomy for off-grid deployment:
* **Microcontroller**: The ESP32 DevKit acts as the central processor and manages the Wi-Fi connection [9, 728]. It calculates the distance from the Time-of-Flight (ToF) data and executes the automation logic for threshold checking [728, 729]. 
* **Ultrasonic Sensor**: The JSN-SR04T is a ruggedized sensor featuring an integrated sealed cable probe, making it ideal for outdoor and wet conditions [236, 335]. It operates using a 40 kHz ultrasonic pulse [326]. 
* **Voltage Divider Circuit**: Crucial for hardware safety, this circuit uses a 1 kΩ resistor and a 2 kΩ resistor to step down the sensor's 5V ECHO output signal to a safe 3.3V level for the ESP32 input pins [196, 197, 722].
* **Power Management**: A 3.7V Li-ion battery, paired with a 5V Step-up (Boost) Converter, provides true autonomy and extended operational capability for off-grid deployment [179, 180, 247].

## 💻 Software & Dashboard
The system is programmed using the Arduino IDE [361]. It hosts a custom embedded website for real-time data visualization:
* **Web Server**: Utilizes the `ESPAsyncWebServer` and `AsyncTCP` libraries to serve an HTML/CSS/JavaScript dashboard directly from the microcontroller [1099, 1100, 1111].
* **Live Analytics**: Integrates Chart.js to plot distance measurements dynamically over a WebSocket connection [1113, 1126, 1284].
* **IoT Cloud Link**: Capable of transmitting live distance values to cloud platforms via MQTT or HTTP for remote monitoring and event-triggered alerts [161, 233, 476].

## 🌍 Real-World Applications
* **Marine Navigation**: Assists autonomous underwater vehicles (AUVs) with collision avoidance and operates effectively in low-visibility murky waters [454, 455].
* **Environmental Monitoring**: Continuously tracks reservoir water levels for flood prediction and management [457, 459].
* **Industrial Automation**: Monitors liquid levels in manufacturing tanks and inspects oil/gas pipelines for leaks [460, 461].
* **Defense & Security**: Used for border surveillance and intrusion detection by monitoring underwater or terrestrial environments [449].

## 🚀 Key Advantages
* **Cost-Effective**: The total Bill of Materials (BOM) is under ₹2500, making it 5-10 times cheaper than commercial sonar modules while matching performance for civilian applications [475].
* **High Efficiency**: Employs duty-cycled scanning at 500ms intervals; when combined with a Li-ion battery, it can achieve over 72 hours of autonomy [477].
* **Rapid Response**: Automated SMS/Telegram alerts via cloud dashboards reduce incident response times from minutes to mere seconds [476].

## ⚠️ Challenges & Limitations
* **Environmental Limitations**: Sonar accuracy and performance can be affected by water salinity, turbidity, or ambient noise interference [916].
* **Dependencies**: The system requires a stable power supply and reliable internet connectivity to ensure continuous remote monitoring [913].
* **Cybersecurity**: As with all IoT devices, the system is vulnerable to cyberattacks or unauthorized access to sensitive sonar data [915].

## 🔮 Future Scope
* **AI Integration**: Implementing AI-driven threat detection for faster identification of submarines, drones, or intruders [949].
* **3D Mapping**: Enabling real-time 3D mapping of seabeds for safe marine navigation in complex terrains [954].
* **Industry 4.0 Integration**: Advancing predictive maintenance in oil, gas, and shipping industries by integrating with broader automated factory operations [960].
* **Blockchain Security**: Using blockchain-secured IoT networks to ensure tamper-proof communication for military and defense applications [951].
