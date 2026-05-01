# Automated Object Sorting Conveyor System (TM4C123)

## Overview
This project presents a fully functional automated conveyor-based object sorting system developed using the TM4C123GH6PM microcontroller. The system integrates embedded control with mechanical design to detect, count, and sort objects based on their position and timing on a moving conveyor belt.

Infrared sensors are used for object detection and tracking, while a stepper motor drives the conveyor mechanism. A servo-based mechanical sorting structure directs objects into different paths. The system also includes overload detection and an emergency stop mechanism, making it suitable as a prototype for industrial automation applications.

---

## Key Features
- Multi-point object detection using IR sensors  
- Real-time object counting and tracking  
- Conveyor motion control using NEMA 17 stepper motor  
- TB6600 driver-based high-current motor control  
- Mechanical sorting using angled structure and servo actuation  
- Overload detection using dedicated IR sensor  
- Emergency stop functionality  
- LCD-based real-time system status display  
- Register-level embedded programming approach  

---

## Hardware Components

### Core Electronics
- TM4C123GH6PM (Tiva C Series Microcontroller)  
- Infrared Sensors (x4)  
- 16x2 LCD Display  
- Push Button (Emergency Stop)  
- Potentiometer  

### Actuation & Power
- NEMA 17 Stepper Motor  
- TB6600 Stepper Motor Driver  
- SG90 Servo Motors (x2)  
- 12V Power Supply  

### Mechanical System
- Conveyor Belt  
- Wooden Frame Structure  
- Bearings and Shafts  
- Sorting Mechanism (Angled Divider)  

---

## Pin Configuration

### Stepper Motor (TB6600 Driver)
- STEP (PUL+) → PD1  
- DIR → PD0  

### IR Sensors
- Sensor A → PE0  
- Sensor B → PE1  
- Sensor C → PE2  
- Overload Sensor → PE3  

### Emergency Stop
- Push Button → PC4  

### LCD (4-bit Mode)
- RS → PB0  
- EN → PB1  
- D4 → PB2  
- D5 → PB3  
- D6 → PB4  
- D7 → PB5  

### Additional
- Servo Motors → PD2, PD3  
- On-board LEDs → PF1 (Red), PF2 (Blue)  

---

## System Working

1. Objects are placed on the conveyor belt  
2. Stepper motor drives the belt via TB6600 driver  
3. IR sensors detect objects at multiple checkpoints  
4. Microcontroller processes detection signals  
5. Objects are counted and tracked in real time  
6. Sorting is performed using mechanical structure and servo control  
7. Overload condition is detected if an object blocks the end sensor  
8. Emergency stop button halts the system instantly  
9. LCD continuously displays object counts and system status  

---

## System Architecture

- **Input Layer:** IR sensors, push button  
- **Processing Layer:** TM4C123 microcontroller  
- **Output Layer:** Stepper motor driver, servo motors, LCD  

All control logic is implemented using low-level register-based programming for efficient and deterministic behavior.

---

## Software Implementation

- Language: Embedded C  
- Platform: Keil uVision / Energia  
- Programming Style: Register-level configuration  

### Key Modules
- GPIO configuration  
- Stepper motor pulse generation  
- Sensor polling and debounce logic  
- Overload detection algorithm  
- LCD interfacing  
- Main control loop for sorting logic  

---

## How to Run

1. Connect all hardware components as per pin configuration  
2. Open the project in **Keil uVision** or **Energia**  
3. Select microcontroller: **TM4C123GH6PM**  
4. Compile the code  
5. Upload to Tiva C LaunchPad  
6. Power the system and observe operation  

---

## Bill of Materials (Summary)
- Total Estimated Cost: **~18,500 PKR**  
- Includes microcontroller, sensors, motor, driver, mechanical setup, and power modules  

---

## Applications
- Industrial automation systems  
- Packaging and sorting lines  
- Warehouse material handling  
- Educational embedded systems projects  

---

## Future Improvements
- Vision-based sorting using camera modules  
- IoT-based remote monitoring and control  
- PID-based speed control for conveyor optimization  
- Industrial-grade sensor integration  
- Real-time data logging and analytics  

---

## License
This project is licensed under the MIT License.
