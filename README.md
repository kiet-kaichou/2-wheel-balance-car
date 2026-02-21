# 2-Wheel Self-Balancing Robot

This repository contains the source code for a 2-wheel self-balancing robot, developed in C for the STM32F407VET6 microcontroller. The firmware utilizes the STM32 HAL library to efficiently manage hardware peripherals and implements a robust cascaded PID control system running precisely inside a 100Hz hardware timer interrupt. The outer control loop processes raw I2C data from an MPU6050 IMU through a complementary filter to determine the pitch angle, computing the required target speed to maintain balance. The inner loop then regulates the motors by comparing this target against real-time hardware encoder feedback, outputting deadzone-compensated PWM signals for highly responsive motor control. 

To get started, simply clone the repository, open it in your preferred STM32 toolchain (like STM32CubeIDE), build, and flash it to your board.

Just remember to keep the robot perfectly still on a flat surface upon power-up, allowing the IMU to calibrate its zero-point offsets before the balancing sequence begins.
