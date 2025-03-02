# Mbed Automated Medication Delivery System

## Overview
This project automates medication delivery in a hospital using an Mbed-based system. The system utilizes ultrasonic sensors, color tracking, servo motors, and an LCD interface to navigate, detect patient beds, and dispense medication efficiently.

## Getting Started
### Prerequisites
- Mbed-compatible microcontroller
- Mbed OS installed
- Required components: LCD, ultrasonic sensors, servo motors, keypad, color sensor

### Cloning the Repository
To get the latest version of the project, run the following command in your Mbed workspace:
```sh
git clone https://github.com/tech-ky/MAPP_project_medical_train.git
```
If you've already cloned the repository and want to update it, run:
```sh
git pull origin main
```

## How to Use
1. **Compile & Flash**: Build the project in Mbed Studio or another Mbed-compatible environment and flash it onto your microcontroller.
2. **Start the System**: Power on the Mbed board. The LCD will prompt you to schedule a delivery.
3. **User Input**: Use the keypad to confirm a delivery.
4. **Automated Delivery**: The system will navigate, detect patient beds, and dispense medication automatically.
5. **Completion**: The system will return to the starting position and await the next schedule.

## Video Demonstration
Watch the system in action:
- [Car Movement Video 1](https://youtu.be/4zVeG6fx4vY)
- [Car Movement Video 2](https://youtu.be/THQ6W-iNHxQ)
- [Medication Drop Alert Video](https://youtu.be/hvq-RzdkRa0)

## More Information
For a detailed explanation of the project, check out the project documentation on Canva:
[Project Documentation](https://www.canva.com/design/DAGUv7co6F4/CFF_JfXvHBZeTQvfcVGxFA/edit?utm_content=DAGUv7co6F4&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton)

## Contributing
If you'd like to contribute, fork the repository, make your changes, and submit a pull request.

## License
This project is open-source under the MIT License.

