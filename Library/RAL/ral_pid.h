/**
 *	@file
 *	@brief	RAL_PID Header File
 * */
#ifndef __RAL_PID_H
#define __RAL_PID_H

///	Maximum number of the PID channels
#define MAX_PID_CHANNEL	10
#include "ral_odometry.h"
#include "ral_motor.h"


/**
 *	@brief	Generic PID Struct storing constants
 *
 *	Apart from the KP KI KD Imax, 
 *	error calculation function and output function can be anchored 
 *	to support different PID usage.
 * */
struct PIDStruct{
	///	Constant for PID calculation
	double kp,ki,kd,imax,tolerance,error_cumu,last_error;
	/// Error calculation function anchor point
	double (*err_Calc)(void* info);
	/// output function anchor point
	void (*output_Func)(void* info, double err);
	/// Pointer for special parameters for different PID use
	void* info;
	/// Whether this PID would be updated
	char start;
	/// Error term
	double error_term;
	/// PID result
	double PIDResult;
	/// Update divider, Update frequency is timer frequncy divided by this 
	int update_divider;
};
/**
 *	@brief	PID extra parameters for Position Mode
 *	@see PID_Init_Pos
 * */
struct PosInfo{
	double acceleration;		///< Accleration
	double topSpeed;			///< Maximum Speed
	double deceleration;		///< Deceleration
	int targetPos;				///< Target Position
	int initPos;				///< Initial Position
	int time;					///< Current time
	int T1,						///< End of accleration phrase
		T2,						///< End of max speed phrase
		Tend;					///< End of the journey
	int timeRegion;				///< current region
	struct Motor_Struct* motor;	///< Motor under control
	int encoderChannel;			///< Encoder Channel
};

double PosInfoErrCal(void*);
void PosInfoOutFunc(void* POSS,double result);
struct PIDStruct* PID_Init_Pos(struct PIDStruct* PIDS,double acc,double topSpeed,double dece,int targetPos,int initPos,struct Motor_Struct* motor,int encoderChannel,int divider);

/**
 *	@brief	PID extra parameters for velocity mode
 *	@see PID_Init_Vel
 * */
struct VelInfo{
	double targetSpeed;			///< Target speed
	int oldPos;					///< Pervious Position
	struct Motor_Struct* motor;	///< Motor under control
	int encoderChannel;			///< Encoder Channel
};

double VelInfoErrCal(void*);
void VelInfoOutFunc(void*, double result);
struct PIDStruct* PID_Init_Vel(struct PIDStruct* PIDS,double targetSpeed,struct Motor_Struct* motor,int encoderChannel,int divider);
struct PIDStruct* PID_Init_RealVel(struct PIDStruct* PIDS,double targetSpeed,int countPerRevolution,double lengthPerRevolution,int updateFrequency, struct Motor_Struct* motor,int encoderChannel,int divider);

/**
 *	@brief	PID extra parameters for Theta Mode
 *	
 *	Control through two VelInfo
 *	@see	PID_Init_Theta
 * */
struct ThetaInfo{
	double targetTheta;		///< Target Theta
	struct Pos* pos;		///< Pointer to struct Pos
	struct VelInfo* motorL;	///< VelInfo of the Left Motor
	struct VelInfo* motorR;	///< VelInfo of the Right Motor
};

double ThetaInfoErrCal(void*);
void ThetaInfoOutFunc(void*,double result);
struct PIDStruct* PID_Init_Theta(struct PIDStruct* PIDS,double targetTheta,struct VelInfo* motorL, struct VelInfo* motorR, struct Pos* pos,int divider);

struct PIDStruct* PID_Init(struct PIDStruct* PIDS,double (*err_Calc)(void* info),void (*output_Func)(void* info, double err),void *info,int divider);
struct PIDStruct* PID_SetParameter(struct PIDStruct* PIDS,double kp,double ki,double kd,double imax,double tolerance);
double PID_Calc(struct PIDStruct* PIDS);
void PID_Update(struct PIDStruct* PIDS);
void PID_Start(struct PIDStruct* PIDS);
void PID_Stop(struct PIDStruct* PIDS);
void PID_Push(struct PIDStruct* PIDS);
void PID_UpdateAll(void);
#endif
