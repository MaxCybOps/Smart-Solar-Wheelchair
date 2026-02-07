# Smart-Solar-Wheelchair
Solar-powered intelligent wheelchair with Arduino-based obstacle avoidance and multiple control modes.
# Smart Solar-Powered Wheelchair

## Overview
This project was developed as part of my undergraduate studies in Mechatronics Engineering at the Federal University of Technology, Owerri (FUTO).  
The goal was to design and construct a **solar-powered intelligent wheelchair** to promote inclusion and independence for physically impaired individuals, using sustainable energy and smart control systems.

## Features
- **Sustainable Power:** Runs off-grid using a 22V, 80W solar panel and a 24V battery.
- **Intelligent Control:** Arduino Nano core with IR sensors for obstacle avoidance.
- **Multiple Control Modes:**
  - **Voice Control** (hands-free operation via speech recognition module)
  - **Remote Control** (IR remote or Bluetooth navigation)
  - **Push-Button Control** (manual use)
- **Safety:** Integrated IR obstacle detection prevents collisions.

## System Architecture
- **Hardware:** Solar panel, battery, Arduino Nano, IR sensors, motor drivers, wheelchair frame.
- **Software:** Arduino C++ code for sensor integration and control logic.
- **Control Flow:** Sensor input → Arduino processing → Motor driver → Wheelchair movement.

## Control Modes
1. **Push-Button Mode:** Simple manual control with obstacle avoidance.  
2. **Voice Mode:** Commands like *Forward, Backward, Left, Right, Stop* processed by the Voice Recognition Module.  
3. **Remote Mode:** IR remote mapped to movement directions (Up, Down, Left, Right, OK).  
4. **Obstacle Avoidance:** IR sensor prevents forward motion if an obstacle is detected, ensuring user safety.

## Challenges & Solutions
- **Power Management:** Designed efficient solar charging and battery regulation.  
- **Control Sync Issues:** Debugged Arduino code to ensure smooth switching between modes.  
- **Budget Constraints:** Optimized component selection for affordability.  

## Team
Developed by a team of 11 Mechatronics Engineering students under the supervision of **Engr. Dr. V. O. Aniugo**.

## Impact
This project demonstrates purposeful engineering driven by empathy, resilience, and innovation.  
It highlights my ability to integrate **robotics, renewable energy, and embedded systems** into real-world solutions.

## Media
Here are images of the prototype and testing phases:

![Wheelchair Prototype](images/wheelchair-prototype.jpeg)  
*The completed smart solar-powered wheelchair prototype.*

![Solar Panel Integration](images/solar-panel-integration.jpeg)  
*Solar panel and battery integration for sustainable power.*

![Control System Setup](images/control-system-setup.jpeg)  
*Arduino Nano, IR sensors, and wiring for intelligent control.*

![Final Defense Presentation](images/final-defense-presentation.jpeg)  
*Team presentation and successful defense of the project.*


## Future Improvements
- Integration with ROS 2 for advanced robotic control.  
- UI dashboard for live monitoring of wheelchair performance.  
- Enhanced AI-based obstacle detection.  

## How to Run
1. **Install Arduino IDE** from [arduino.cc](https://www.arduino.cc/en/software).  
2. **Clone this repository** or download the `.zip` file.  
   ```bash
   git clone https://github.com/MaxCybOps/Smart-Solar-Wheelchair.git


