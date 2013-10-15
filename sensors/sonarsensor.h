#ifndef SONARSENSOR_H
#define SONARSENSOR_H

#include "isonarsensor.h"

/** @brief Class to represent standard one or two pin echo timing sonar sensors.
 *
 *  This class represents any of a number of basic one or two pin sonar sensors
 *  that measure distance using an output trigger pulse and then listen for an
 *  echo that is timed using an input pulse.  This object can handle either
 *  one or two pin types.  It will only operate sensors that use the common
 *  positive pulse logic.  Other types of sensors or those requiring more 
 *  interaction should be implemented in a more specific class.
 */

class SonarSensor : public ISonarSensor
{
public:
    SonarSensor();                  
    SonarSensor(int t_pin, int e_pin, int t_len, int t_out);
    void init(int t_pin, int e_pin, int t_len, int t_out);

    // Interface overrides: See interface defintions for documentation
    long trigger();
    long getRange_in();
    long getRange_cm();
    
private:
    SonarSensor(const SonarSensor& s);
    
protected:
    int actPin;         // Activity pin
    int trigPin;        // Trigger pin to start ranging cycle
    int echoPin;        // Echo pin to capture time-of-flight
    int trigLen;        // Length in usecs for the trigger pulse duration
    int usTimeout;      // Maximum expected echo length in microseconds
    int echoTimeout;    // Maximum expected echo length in clock pulses
    long tof;           // Last time of flight in usecs
};

#endif // SONARSENSOR_H
