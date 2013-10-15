#include "sonarsensor.h"
#include "simpletools.h"

/** @brief Default constructor for sonar sensor.
 *   
 */
SonarSensor::SonarSensor()
{
    trigPin = -1;
    echoPin = -1;
    trigLen = -1;
    enabled = 0;
    usTimeout = -1;
    echoTimeout = -1;
    tof = -1;
}


/** @brief Basic constructor.
  *
  * @param int t_pin: Trigger pin (0-31)
  * @param int e_pin: Echo pin (0-31), can be the same as t_pin for one-pin sensors
  * @param int t_len: Trigger pin length in microseconds
  * @param int t_out: Echo timeout in milliseconds
  */
SonarSensor::SonarSensor(int t_pin, int e_pin, int t_len, int t_out)
{
    trigPin = -1;
    echoPin = -1;
    trigLen = -1;
    enabled = 0;
    usTimeout = -1;
    echoTimeout = -1;
    tof = -1;
    init(t_pin, e_pin, t_len, t_out);
}


/** @brief Initializer function, used to initialize default constructed objects
  *
  * @param int t_pin: Trigger pin (0-31)
  * @param int e_pin: Echo pin (0-31), can be the same as t_pin for one-pin sensors
  * @param int t_len: Trigger pin length in microseconds
  * @param int t_out: Echo timeout in milliseconds
  */
void SonarSensor::init(int t_pin, int e_pin, int t_len, int t_out)
{
    if (t_pin > 27 or t_pin < 0)
        return;

    if (t_len < 5)
        t_len = 5;

    if (t_out < 10)
        t_out = 10;
    if (t_out > 50)
        t_out = 50;

    trigPin = t_pin;
    echoPin = e_pin;
    trigLen = t_len;
    enabled = 1;
    
    usTimeout = t_out*1000;
    echoTimeout = t_out*(CLKFREQ/1000);
    tof = -1;
}


/** @brief Trigger a sonar ranging cycle 
 *
 *  Sends a positive trigger pulse on trigPin of the duration specified by
 *  trigLen.  Then waits for positive echo pulse on echoPin.  Returns as soon as
 *  pos->neg transition is detected or the echoTimeout limit is reached.
 *
 *  @return Microsecond time-of-flight for the detected echo
 */
long SonarSensor::trigger()
{
    if (!enabled or echoPin < 0 or trigPin < 0)
        tof = -1;
    else
    {
        // Perform a ranging cycle and store the time of flight result.
        set_io_timeout(echoTimeout);
        low(trigPin);
        pulse_out(trigPin, trigLen);
        tof = pulse_in(echoPin, 1);
    }

    if (tof == 0)
        tof = usTimeout;
        
    
    return tof;
}


/** @brief Get the range in inches for the previous ranging cycle.
 *
 *  @return int: Range in tenths of an inch or negative for invalid value.
 */
long SonarSensor::getRange_in()
{
    if (!enabled or tof < 0)
        return tof = -1;

    long in_range = (tof*10) / 148;
    return in_range;
}


/** @brief Get the range in centimeters for the previous ranging cycle.
 *
 *  @return int: range in centimeters or negative for invalid value.
 */
long SonarSensor::getRange_cm()
{
    if(!enabled or tof < 0)
        return tof = -1;

    long cm_range = tof / 58;
    return cm_range;
}

