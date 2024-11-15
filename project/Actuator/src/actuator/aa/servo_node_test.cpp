#include "servo_mgr.hpp"
#include "led_mgr.hpp"

#include <iostream>

/// Node entry point
int main(int argc, char **argv) {    
    auto servoMgr = std::make_unique<PWM::ServoMgr>();
    auto ledMgr = std::make_unique<PWM::LedMgr>();
   
   //g++ -o servo_node_test bios_version.cpp led_mgr.cpp pwm.cpp servo_mgr.cpp servo_node_test.cpp utility.cpp jsoncpp.cpp -lstdc++fs -std=c++17

    // Test Code for Servo Calibration Data
    {
        int cal_type = 0;
        int servo_min;
        int servo_mid;
        int servo_max;
        int servo_polarity;
        
        // Print current calibration value
        servoMgr->getCalibrationValue(cal_type, &servo_min, &servo_mid, &servo_max, &servo_polarity);
        printf("Current Servo calibration value: min: %d, mid: %d, max: %d, polarity: %d\n", servo_min, servo_mid, servo_max, servo_polarity);

        // Set New calibration value
        servoMgr->setCalibrationValue(cal_type, servo_min - 10, servo_mid - 10, servo_max - 10, servo_polarity == 1 ? -1 : 1);
        
        // Print updated calibration value
        servoMgr->getCalibrationValue(cal_type, &servo_min, &servo_mid, &servo_max, &servo_polarity);
        printf("New Servo calibration value(-10): min: %d, mid: %d, max: %d, polarity: %d\n", servo_min, servo_mid, servo_max, servo_polarity);

        // Recover calibration value
        servoMgr->setCalibrationValue(cal_type, servo_min + 10, servo_mid + 10, servo_max + 10, servo_polarity == 1 ? -1 : 1);

        // Print recovered calibration value
        servoMgr->getCalibrationValue(cal_type, &servo_min, &servo_mid, &servo_max, &servo_polarity);
        printf("Recovered Current Servo calibration value: min: %d, mid: %d, max: %d, polarity: %d\n", servo_min, servo_mid, servo_max, servo_polarity);
    }

    // Test Code for Motor Calibration Data
    {
        int cal_type = 1;
        int motor_min;
        int motor_mid;
        int motor_max;
        int motor_polarity; 
    
        // Print current calibration value
        servoMgr->getCalibrationValue(cal_type, &motor_min, &motor_mid, &motor_max, &motor_polarity);
        printf("Current Motor calibration value: min: %d, mid: %d, max: %d, polarity: %d\n", motor_min, motor_mid, motor_max, motor_polarity);

        // Set New calibration value
        servoMgr->setCalibrationValue(cal_type, motor_min - 10, motor_mid - 10, motor_max - 10, motor_polarity == 1 ? -1 : 1);

        // Print updated calibration value
        servoMgr->getCalibrationValue(cal_type, &motor_min, &motor_mid, &motor_max, &motor_polarity);
        printf("New Servo calibration value(-10): min: %d, mid: %d, max: %d, polarity: %d\n", motor_min, motor_mid, motor_max, motor_polarity);

        // Recover calibration value
        servoMgr->setCalibrationValue(cal_type, motor_min + 10, motor_mid + 10, motor_max + 10, motor_polarity == 1 ? -1 : 1);

        // Print recovered calibration value
        servoMgr->getCalibrationValue(cal_type, &motor_min, &motor_mid, &motor_max, &motor_polarity);
        printf("Recovered Current Servo calibration value: min: %d, mid: %d, max: %d, polarity: %d\n", motor_min, motor_mid, motor_max, motor_polarity);
    }

    float cur_motor = 0;
    float cur_servo = 0;

    while(1) {
        std::cout << "input cur motor value: ";
        std::cin >> cur_motor;
        std::cout << "\nInput cur servo value: ";
        std::cin >> cur_servo;
        std::cout << "\ncur_motor: " << cur_motor << ", cur_servo: " << cur_servo << std::endl;

        servoMgr->servoSubscriber(cur_motor, cur_servo); 
    }

    servoMgr->servoSubscriber(0, 0); 

    return 0;
}