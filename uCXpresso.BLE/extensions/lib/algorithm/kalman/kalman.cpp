#include "kalman.h" 

/*********************************************************************  
 The implememted kalman filter estimates the angle that will be used 
 on the PID controller alogrithm. The filter consists of two states 
 [angle, gyro_bais].
 *********************************************************************/ 

/* Original: www.mbed.org
 * Modify  : uCXpresso
 * Date	   : 2014/5/22
 */
 
/******************************************************************** 
*    Function Name:  Constructor                                    *
*    Return Value:   none                                           * 
*    Parameters:     dt, R_angle, Q_gyro, Q_angle    				*
*    Description:    Initialize the kalman Filter parameters.       * 
********************************************************************/  
Kalman::Kalman(double R_angle, double Q_gyro, double Q_angle) // double dt,
{ 
    // Initialize the two states, the angle and the gyro bias. As a 
    // byproduct of computing the angle, we also have an unbiased 
    // angular rate available. 
    x_bias = 0.0;
    x_rate = 0.0;
    x_angle = 0.0;
 
 
    // Initialize the delta in seconds between gyro samples. 
    //dt = dt;
 
    // Initialize the measurement noise covariance matrix values. 
    // In this case, R is a 1x1 matrix tha represents expected 
    // jitter from the accelerometer. 
    R_angle = R_angle;
 
    // Initialize the process noise covariance matrix values. 
    // In this case, Q indicates how much we trust the acceleromter 
    // relative to the gyros. 
    // Q_gyro set to 0.003 and Q_angle set to 0.001. 
    
    Q_gyro = Q_gyro;
    Q_angle = Q_angle;
 
    // Initialize covariance of estimate state.  This is updated 
    // at every time step to determine how well the sensors are 
    // tracking the actual state. 
    
    P_00 = 1.0;
    P_01 = 0.0;
    P_10 = 0.0;
    P_11 = 1.0;
} 

/******************************************************************** 
*    Function Name:  predict                            			*
*    Return Value:   none                                           * 
*    Parameters:     measured gyroscope value        				*
*    Description:    Called every dt(Timer 1 overflow with a biased * 
*                    gyro. Also updates the current rate and angle  * 
*                    estimate).                                     * 
********************************************************************/  
void Kalman::predict(double dot_angle,  double dt)
{ 
    // Static so these are kept off the stack. 
    static double gyro_rate_unbiased; 
    static double Pdot_00; 
    static double Pdot_01; 
    static double Pdot_10; 
    static double Pdot_11;
    
    // Unbias our gyro. 
    gyro_rate_unbiased= dot_angle - x_bias;
 
    // Store our unbiased gyro estimate. 
    x_rate= gyro_rate_unbiased;
     
    // Update the angle estimate. 
    x_angle= x_angle + (dot_angle - x_bias)*dt;
 
    // Compute the derivative of the covariance matrix 
    // Pdot = A*P + P*A' + Q 
    Pdot_00 = Q_angle - P_01 - P_10;
    Pdot_01 = -P_11;
    Pdot_10 = -P_11;
    Pdot_11 = Q_gyro;
 
    // Update the covariance matrix. 
    P_00 += Pdot_00 * dt;
    P_01 += Pdot_01 * dt;
    P_10 += Pdot_10 * dt;
    P_11 += Pdot_11 * dt;
}

/********************************************************************* 
*    Function Name:  update                                   		 *
*    Return Value:   none                                            * 
*    Parameters:     measured angle value             				 *
*    Description:    Called when a new accelerometer angle           * 
*                    measurement is available. Updates the estimated * 
*                    angle that will be used.                        * 
*********************************************************************/
void Kalman::update(double angle_measured)
{
    // Static so these are kept off the stack. 
    static double y;
    static double S;
    static double K_0;
    static double K_1;
    
    // Compute the error in the estimate. 
    // Innovation or Measurement Residual 
    // y = z - Hx 
    y= angle_measured - x_angle;
 
    // Compute the error estimate. 
    // S = C P C' + R 
    S = R_angle + P_00;
 
    // Compute the kalman filter gains. 
    // K = P C' inv(S) 
     K_0 = P_00 / S;
     K_1 = P_10 / S;
 
    // Update covariance matrix. 
    // P = P - K C P 
    P_00 -= K_0 * P_00;
    P_01 -= K_0 * P_01;
    P_10 -= K_1 * P_00;
    P_11 -= K_1 * P_01;
   
    // Update the state (new)estimates (Correct the prediction of the state). 
    // Also adjust the bias on the gyro at every iteration. 
    // x = x + K * y 
    x_angle = x_angle + K_0 * y;
    x_bias = x_bias + K_1 * y;
     
}
