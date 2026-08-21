# 🏥 Smart Patient Health Monitoring System

An IoT-based Smart Patient Health Monitoring System designed for real-time monitoring of patient health parameters, local display, and remote health-data visualization.

---

## 📌 Project Overview

The Smart Patient Health Monitoring System is an embedded IoT project developed to continuously monitor important patient health parameters using sensors and a microcontroller.

The system collects physiological data, processes the sensor readings, displays the information locally, and can transmit the data to an IoT platform for remote monitoring.

The project demonstrates the integration of embedded systems, biomedical sensors, wireless communication, and IoT technologies for healthcare applications.

---

## 🎯 Objectives

- Monitor patient health parameters in real time.
- Acquire physiological data using sensors.
- Process and display sensor readings.
- Provide remote monitoring through an IoT platform.
- Generate alerts when monitored parameters exceed configured limits.
- Demonstrate embedded system and IoT applications in healthcare.

---

## ✨ Key Features

- ❤️ Real-time patient health monitoring
- 🌡️ Health parameter measurement
- 📊 Local data display
- 📱 IoT-based remote monitoring
- 🚨 Abnormal-condition alerts
- 📡 Wireless communication
- 🔄 Continuous sensor monitoring
- 💻 Embedded system implementation

---

## 🧠 System Architecture

```text
              ┌──────────────────────┐
              │   Health Sensors     │
              │                      │
              │  ┌────────────────┐  │
              │  │ MAX30102       │  │
              │  │ AD8232         │  │
              │  │ DS18B20        │  │
              │  │ MPU6050        │  │
              │  └────────────────┘  │
              └──────────┬───────────┘
                         │
                         ▼
              ┌──────────────────────┐
              │    Microcontroller   │
              │                      │
              │   Data Acquisition   │
              │   Data Processing    │
              └──────────┬───────────┘
                         │
                         │
                         │        
                         ▼         
                       OLED      
                      Display      
