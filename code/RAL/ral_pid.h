#ifndef __RAL_PID_H
#define __RAL_PID_H

#define MAX_PID_CHANNEL	10
#include "ral_odometry.h"

struct PIDStruct{
	double kp,ki,kd,imax,tolerance,error_cumu,last_error;
	double (*err_Calc)(void* info);
	void (*output_Func)(void* info, double err);
	void* info;
	char start;
	double error_term;
	double PIDResult;
};

struct PosInfo{
	double acceleration;
	double topSpeed;
	double deceleration;
	int targetPos;
	int initPos;
	int time;
	int T1,T2,Tend;
	int timeRegion;
	int pwmChannel;
	int encoderChannel;	
};

double PosInfoErrCal(struct PosInfo*);
void PosInfoOutFunc(struct PosInfo* POSS,double err);

struct VelInfo{
	double targetSpeed;
	int oldPos;
	int pwmChannel;
	int encoderChannel;
};

double VelInfoErrCal(struct VelInfo*);
void VelInfoOutFunc(struct VelInfo* VELS, double err);

struct ThetaInfo{
	double targetTheta;
	struct Pos* pos;
};

double ThetaInfoErrCal(struct ThetaInfo*);
void ThetaInfoOutFunc(struct ThetaInfo* THES,double err);

struct PIDStruct* PID_Init(struct PIDStruct* PIDS,double (*err_Calc)(void* info),void (*output_Func)(void* info, double err),void *info);
void PID_SetParameter(struct PIDStruct* PIDS,double kp,double ki,double kd,double imax,double tolerance);
double PID_Calc(struct PIDStruct* PIDS);
void PID_Update(struct PIDStruct* PIDS);
void PID_Start(struct PIDStruct* PIDS);
void PID_Stop(struct PIDStruct* PIDS);
void PID_Push(struct PIDStruct* PIDS);
void PID_UpdateAll(void);
struct PIDStruct* PID_Init_Pos(struct PIDStruct* PIDS,double acc,double topSpeed,double dece,int targetPos,int initPos,int pwmChannel,int encoderChannel);
struct PIDStruct* PID_Init_Vel(struct PIDStruct* PIDS,double targetSpeed,int pwmChannel,int encoderChannel);
#endif
