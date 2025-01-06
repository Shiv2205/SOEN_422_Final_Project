# Smart Attendance System

## Demo Video
[![Demo Video](https://i9.ytimg.com/vi_webp/8dqWZcBLxaw/mq1.webp?sqp=CLiT8LsG-oaymwEmCMACELQB8quKqQMa8AEB-AG-B4AC0AWKAgwIABABGGUgZShlMA8=&rs=AOn4CLAo0L088eflJgKO5pX7yjahypalIQ)](https://www.youtube.com/watch?v=8dqWZcBLxaw)

## Overview

The **Smart Attendance System** is an innovative solution designed to streamline and enhance attendance management in academic settings. By leveraging RFID and BLE technology, this system minimizes classroom disruptions, improves reliability, and ensures secure handling of attendance data.

This project is part of the **Smart Campus Embedded Solutions** initiative and was developed by **Shivam Veerabudren**.

## Problem Statement

Traditional attendance methods, such as name-calling or passing a sheet of paper, are time-consuming and prone to errors:

1. **Name-calling** consumes valuable lecture time.
2. **Sign-in sheets** often result in unreliable data due to students signing for absent peers.

### Objective

The Smart Attendance System aims to:
- Seamlessly integrate attendance-taking into lecture time to avoid schedule delays.
- Ensure physical presence by requiring students to use their RFID cards for authentication.
- Provide reliable and secure attendance records.

## Features

- **RFID Integration**: Students use RFID cards to mark their attendance.
- **Secure BLE Configuration**: Configured BLE connections are disabled after setup to prevent tampering.
- **Duplicate Check**: Avoids duplicate attendance entries for the same student.
- **Cloud-Enabled**: Utilizes a free Google Cloud Run instance for backend processing.
- **Cost-Effective Design**:
  - ESP32 microcontroller: ~$10
  - RFID IC cards: ~$1.37 each

## Functional Block Diagram

> ![Functional Block Diagram](https://drive.google.com/uc?export=view&id=14KNTPFyttNsLfE55MiaEw0hcpwXexxa0)
> 
## Design Considerations

### Safety & Security
- Ensures student information is protected with robust encryption and access controls.
- Minimal attack surface by using Docker containers with essential libraries.

### Reliability
- Verifies that students are registered before marking attendance.
- Conducts checks to prevent duplicate records.

### Cost-Effectiveness
- Leverages affordable components and free cloud services for deployment.

## Getting Started

### Prerequisites
- ESP32 development board
- RFID IC cards and reader
- Docker
- Google Cloud account (for Cloud Run)

### Usage
- Power the ESP32 and scan the RFID card to mark attendance.
- Monitor attendance records via the cloud-based dashboard.

---

## Contributors
- **Shivam Veerabudren**  
  Student ID: 40121035

## Acknowledgments
This project was developed as part of the final for SOEN 422 Embedded Systems.
