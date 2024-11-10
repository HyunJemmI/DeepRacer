#ifndef PWM_HPP
#define PWM_HPP

#include <filesystem>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

namespace PWM {
    class Servo
    {
    public:
        /// @param channel Servo channel for which to send the PWM signal's to.
        Servo(int channel);
        ~Servo() = default;
        /// Setter for the PWM period.
        /// @param period Desired period in ms.
        void setPeriod(int period);
        /// Setter for the duty cycle, this is what determines how much the servo actuates.
        /// @param Desired duty cycle.
        void setDuty(int duty);
        /// @returns Current value of the period.
        int getPeriod() const;
        /// @returns Current value of the duty cycle.
        int getDuty() const;

    private:
        /// Channel that the pwm is being written to.
        int channel_;
        /// Current value of the period in ms.
        int period_;
        /// Current value of the duty cycle.
        int duty_;
        /// Dynamically assigned syspath
        std::string syspath_;
    };
}
#endif
