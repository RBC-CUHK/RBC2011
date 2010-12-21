/**
 *	@file
 *	@brief	RAL_Odometry Header File
 * */
#ifndef __RAL_ODOMETRY_H
#define __RAL_ODOMETRY_H

/// Count per revolution of the Encoder
#define ENCODER_COUNT	  76000.0 	//count per revolution
/// Radius of Right Encoder
#define ENCODER_L_RADIUS  4.75	    //cm
/// Radius of Left Encoder
#define ENCODER_R_RADIUS  4.75	    //cm
///	Distance between two encoders
#define ENCODER_DISTANCE  46.0	    //Distance between 2 encoders (cm)

///	PI 
#define PI	  		3.14159265
/// PI divided by 180
#define PI_180		0.017453292519943295769236907684886
///	PI halved
#define PI_2		1.5707963267948966192313216916398
///	180 divided by PI
#define C180_PI		57.295779513082320876798154814105

/**
 *	@brief	Store X , Y coordinates and heading angle of the cart
 * */
struct Pos{
	double x;				///< X coordinate
	double y;				///< Y coordinate
	double theta;			///< Heading Angle
	int encoderChannelL;	///< Left Encoder Channel
	int encoderChannelR;	///< Right Encoder Channel
	int encoderLastCntL;	///< Last Left Encoder Value (For computation)
	int encoderLastCntR;	///< Last Right Encoder Value (For computation)
};

void Odometry_Init(int encoderL, int encoderR);
void Odometry_Update_Pos(struct Pos* PS, int newEncoderLCount, int newEncoderRCount);
void Odometry_Update(void);
void Odometry_Set(double x, double y, double theta);
#endif
