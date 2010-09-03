#include <math.h>
#include "ral_encoder.h"
#include "ral_odometry.h"
#include "ral_pid.h"

struct Pos Odometry_Pos;

void Odometry_Init(int encoderL, int encoderR){ 
    Odometry_Pos.x = 0;
    Odometry_Pos.y = 0;
    Odometry_Pos.theta = 0.0;
    Odometry_Pos.encoderChannelL = encoderL;
    Odometry_Pos.encoderChannelR = encoderR;
	//init the last cnt by reading encoder 
	Odometry_Pos.encoderLastCntL = Encoder_ReadBuffer(Odometry_Pos.encoderChannelL);
	Odometry_Pos.encoderLastCntR = Encoder_ReadBuffer(Odometry_Pos.encoderChannelR);
    return;
};

void Odometry_Update(){
	int LCount;
	int RCount;
	double LCONST;
	double RCONST;
	double LDistance;
	double RDistance;
	double delta_Distance;
	int LnewCount;
	int RnewCount;

    LnewCount = Encoder_ReadBuffer(Odometry_Pos.encoderChannelL);
    RnewCount = Encoder_ReadBuffer(Odometry_Pos.encoderChannelR);
	
	//do odometry update by the delta cnt in L/R encoders
	LCount = LnewCount - Odometry_Pos.encoderLastCntL;
	RCount = RnewCount - Odometry_Pos.encoderLastCntR;

	Odometry_Pos.encoderLastCntL = LnewCount;
	Odometry_Pos.encoderLastCntR = RnewCount;

    LCONST = 2 * ENCODER_L_RADIUS * PI / ENCODER_COUNT; 
    RCONST = 2 * ENCODER_R_RADIUS * PI / ENCODER_COUNT; 
    LDistance = LCount * LCONST;
    RDistance = RCount * RCONST;
    delta_Distance = (LDistance + RDistance) / 2.0;
    Odometry_Pos.x += delta_Distance * cos(Odometry_Pos.theta * PI_180);
    Odometry_Pos.y += delta_Distance * sin(Odometry_Pos.theta * PI_180);
    Odometry_Pos.theta -= (LDistance - RDistance) / ENCODER_DISTANCE * C180_PI;
    return;	
};

void Odometry_Set(double x, double y, double theta){
    Odometry_Pos.x = x;
    Odometry_Pos.y = y;
    Odometry_Pos.theta = theta;
    return;
};
