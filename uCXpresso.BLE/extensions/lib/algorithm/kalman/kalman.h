/* Original: www.mbed.org
 * Modify  : uCXpresso
 * Date	   : 2014/5/22
 */

#ifndef KALMAN_H 
#define KALMAN_H

#include "uCXpresso.h"

#define R_matrix          0.590 
#define Q_Gyro_matrix     0.002 
#define Q_Accel_matrix    0.001
 
class Kalman{
public:

	Kalman(double R_angle, double Q_gyro, double Q_angle);

	void predict(double dot_angle,  double dt);
	void update(double angle_measured);

	// Get the bias.
	inline double get_bias()
	{
	    return x_bias;
	}

	// Get the rate.
	inline double get_rate()
	{
	    return x_rate;
	}

	// Get the angle.
	inline double get_angle()
	{
	    return x_angle;
	}

protected:
    // Two states, angle and gyro bias. Unbiased angular rate is a byproduct. 
    double x_bias;
    double x_rate; 
    double x_angle; 
 
    // Covariance of estimation error matrix. 
    double P_00; 
    double P_01; 
    double P_10; 
    double P_11; 
 
    // State constants. 
    //double dt; 
    double R_angle; 
    double Q_gyro; 
    double Q_angle;
};
 
#endif
