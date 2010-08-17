#ifndef __RAL_ODOMETRY_H
#define __RAL_ODOMETRY_H

#define ENCODER_COUNT	  76000.0 	//count per revolution
#define ENCODER_L_RADIUS  4.75	    //cm
#define ENCODER_R_RADIUS  4.75	    //cm
#define ENCODER_DISTANCE  46.0	    //Distance between 2 encoders (cm)
#define PI	  		3.14159265
#define PI_180		0.017453292519943295769236907684886
#define PI_2		1.5707963267948966192313216916398
#define C180_PI		57.295779513082320876798154814105


struct Pos{
	double x;
	double y;
	double theta;
	int pwmChannelL;
	int pwmChannelR;
	int encoderChannelL;
	int encoderChannelR;
	int encoderLastCntL;
	int encoderLastCntR;
};

void Odometry_Init(struct Pos* P, int encoderChannelL, int encoderChannelR);
void Odometry_Update(struct Pos* P);
void Odometry_Set(struct Pos* P, double x, double y, double theta);
#endif
