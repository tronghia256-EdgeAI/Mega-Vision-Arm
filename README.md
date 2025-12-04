# 🦾 Mega-Vision-Arm: 5-Axis AI Robotic System

![Python](https://img.shields.io/badge/Python-3.8%2B-blue?style=for-the-badge&logo=python)
![Arduino](https://img.shields.io/badge/Hardware-Arduino_Mega_2560-teal?style=for-the-badge&logo=arduino)
![AI](https://img.shields.io/badge/AI-YOLOv8-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

> A comprehensive **5-Axis Robotic System** featuring a linear rail and 4-DOF arm, capable of real-time object detection and manipulation. Powered by **Arduino Mega 2560** for multi-axis synchronization and **YOLOv8** for intelligent decision-making.

---

## 📸 Demo & Showcase

[![Watch the video](https://youtube.com/shorts/ch8xM2GATeg?feature=share/maxresdefault.jpg)](https://youtube.com/shorts/ch8xM2GATeg?feature=share)

---

## 🚀 System Architecture

This project implements a **Cloud-to-Edge** workflow to optimize performance:

1.  **Training (Cloud):** Heavy lifting (Model Training) is performed on **Kaggle** using GPUs.
2.  **Inference (Edge):** The trained model (YOLOv8 Nano) runs locally on the laptop/PC.
3.  **Control (Embedded):** Coordinates are converted to steps and sent via **Serial (USB)** to the Arduino Mega controlling 5 independent axes.

```mermaid
graph LR
    A[Webcam] -->|Images| B(Laptop / Python AI)
    B -->|Coordinates| C{Inverse Kinematics}
    C -->|G-Code/Steps| D[Arduino Mega]
    D -->|Pulse/Dir| E[5x Stepper Drivers]
    E -->|Motion| F[Linear Rail + Robot Arm]