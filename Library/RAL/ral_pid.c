/**
 *	@file
 *	@brief	RAL_PID Function Implementation
 * */
#include <stdlib.h>
#include "ral_pid.h"
#include "ral_encoder.h"

///	PID List to store PID members
static struct PIDStruct* PIDList[MAX_PID_CHANNEL]={NULL};
/// Number of members in List
static int PIDListCount = 0;
/// For update divider list
static unsigned int PIDUpdateCount = 0;
static unsigned int MaxCount = 1;

/**
 *	@brief	Init generic PID struct
 *
 *	@see	PIDStruct
 *	@see	PID_Init_Pos
 *	@see	PID_Init_Vel
 *	@see	PID_Init_Theta
 *
 *	@param	PIDS		PID Struct to be inited
 *	@param	err_Calc	Error Function Pointer
 *	@param	output_Func	Output Function Pointer
 *	@param	info		Extra parameters
 *	@param	divider		Update divider
 *
 *	@return	Pointer to the inited PIDStruct
 * */
struct PIDStruct* PID_Init(struct PIDStruct* PIDS,double (*err_Calc)(void* info),void (*output_Func)(void* info, double err),void *info,int divider){
	PIDS->kp = 0;
	PIDS->ki = 0;
	PIDS->kd = 0;
	PIDS->imax = 0;
	PIDS->tolerance = 0;
	PIDS->error_cumu = 0;
	PIDS->last_error = 0;

	PIDS->err_Calc = err_Calc;
	PIDS->output_Func = output_Func;
	PIDS->info = info;
	PIDS->start = 0;

	PIDS->error_term = 0;
	PIDS->PIDResult = 0;
	if(divider == 0) divider = 1;
	PIDS->update_divider = divider;    

	return PIDS;
}

/**
 *	@brief	Set PID constants
 *	
 *	@param	PIDS		PID Struct to be set
 *	@param	kp			KP constant
 *	@param	ki			KI constant
 *	@param	kd			KD constant
 *	@param	imax		Max for the I term
 *	@param	tolerance	Tolerance for error
 * */
struct PIDStruct* PID_SetParameter(struct PIDStruct* PIDS,double kp,double ki,double kd,double imax,double tolerance){
	PIDS->kp = kp;
	PIDS->ki = ki;
	PIDS->kd = kd;
	PIDS->imax = imax;
	PIDS->tolerance = tolerance;
	return PIDS;
}

/**
 *	@brief	Calculate PID result
 *
 *	@param	PIDS	PID Struct to be processed
 *	@return PID Result
 * */
double PID_Calc(struct PIDStruct* PIDS){
	double p_term;
	double i_term;
	double d_term;

	p_term = PIDS->kp * PIDS->error_term;

	PIDS->error_cumu += PIDS->error_term;
	if (PIDS->error_cumu > PIDS->imax){
		PIDS->error_cumu = PIDS->imax;
	} else if (PIDS->error_cumu < - PIDS->imax){
		PIDS->error_cumu = -PIDS->imax;
	}

	i_term = PIDS->ki * PIDS->error_cumu;

	d_term = PIDS->kd * (PIDS->error_term - PIDS->last_error);
	PIDS->last_error = PIDS->error_term;

	return(p_term + i_term + d_term);
}

void PID_Update(struct PIDStruct* PIDS){
	if(PIDS->start == 0) return;
	PIDS->error_term = PIDS->err_Calc(PIDS->info);
	PIDS->PIDResult = PID_Calc(PIDS);
	PIDS->output_Func(PIDS,PIDS->PIDResult);
	return;    
}

/**
 *	@brief	Start processing a PID Struct
 *	@param	PIDS	PID to be started
 * */
void PID_Start(struct PIDStruct* PIDS){
	PIDS->start = 1;
	return;
}

/**
 *	@brief	Stop processing a PID Struct
 *	@param	PIDS	PID to be stopped
 * */
void PID_Stop(struct PIDStruct* PIDS){
	PIDS->start = 0;
	return;
}

/**
 *	@brief	Find if the PID is in the List
 *	@param	PIDS	PID to be searched
 *	@return	Position at the List
 * */
int PID_Search_List(struct PIDStruct* PIDS){
	int i;
	for(i = 0; i < PIDListCount; i++)
		if(PIDList[i] == PIDS) return i;
	return -1;
}

/**
 *	@brief	Push a PID Struct into the List
 *	@param	PIDS	PID to be pushed
 *	@see	PIDList
 *	@see PID_Search_List
 * */
void PID_Push(struct PIDStruct* PIDS){
	int pos = PID_Search_List(PIDS);
	if(pos == -1){
		PIDList[PIDListCount] = PIDS;
		PIDListCount++;
	}
	if(PIDS->update_divider > MaxCount)
		MaxCount = PIDS->update_divider;
	return;
}

/**
 *	@brief	Update all PID Struct in the List
 *
 *	Handle the Update Divider Here.
 *	@see	PIDList
 *	@see	PID_Update
 * */
void PID_UpdateAll(void){
	int i;
	PIDUpdateCount++;
	for(i = 0; i < PIDListCount; i++){
		if(PIDUpdateCount % PIDList[i]->update_divider == 0)
			PID_Update(PIDList[i]);
	}
	if(PIDUpdateCount == MaxCount)PIDUpdateCount = 0;
	return;
}

/**
 *	@brief	Compute current target position
 *
 *	Calculate the target position based on the current time
 *	different region would have different calculation
 *	@param	POSS	Position Mode information
 *	@return	Current target Position
 * */
double POS_ComputerTarget(struct PosInfo* POSS){
	double s;
	//	double T1,T2,Tend;
	int t= POSS->time;
	int time_region = 0;

	if(POSS->T1 < POSS->T2){
		if(POSS->time < POSS->T1){
			POSS->timeRegion = 1;		
		} else if(POSS->time < POSS->T2){
			POSS->timeRegion = 2;
		} else if(POSS->time < POSS->Tend){
			POSS->timeRegion = 3;
		} else {
			POSS->timeRegion = 4;
		}
	} else {
		if(POSS->time < POSS->Tend / 2){
			POSS->timeRegion = 1;
		} else if(POSS->time < POSS->Tend){
			POSS->timeRegion = 3;
		} else {
			POSS->timeRegion = 4;
		}
	}	
	time_region = POSS->timeRegion;
	//	T1 = POSS->T1;
	//	T2 = POSS->T2;
	//	Tend = POSS->Tend;
	if(POSS->initPos < POSS->targetPos){

		switch(time_region){
			case 1:
				s = t * t * POSS->acceleration / 2 + (double)POSS->initPos;
				break;
			case 2:
				s =   (double)POSS->topSpeed * ( t - POSS->T1) 
					+ (double)POSS->topSpeed * POSS->topSpeed / (2 * POSS->acceleration)
					//+ (double)POSS->top_speed / 2.0 * T1
					+ (double)POSS->initPos;
				break;
			case 3:
				s =   -1 * (t-POSS->Tend) * (t-POSS->Tend) * POSS->deceleration / 2 + (double)POSS->targetPos;
				break;
			case 4:
				s =   POSS->targetPos;
				break;
		}
	}

	// the case that the curve decreases with time
	else{
		switch(time_region){
			case 1:
				s = -1 * t * t * POSS->acceleration / 2 + (double)POSS->initPos;
				break;
			case 2:
				s =  -1* (double)POSS->topSpeed * ( t - POSS->T1) 
					- (double)POSS->topSpeed * POSS->topSpeed / (2 * POSS->acceleration)
					+ (double)POSS->initPos;
				break;
			case 3:
				s =   (t-POSS->Tend) * (t-POSS->Tend) * POSS->deceleration / 2 + (double)POSS->targetPos;
				break;
			case 4:
				s =   POSS->targetPos;
				break;
		}

	}
	return s;	

}

/**
 *	@brief	Error calculation function of Position Mode
 *
 *	difference between current positon and current target position
 *	@param	tempptr	Pointer to Position Mode information
 *	@return Error term
 * */
double PosInfoErrCal(void* tempptr){
	struct PosInfo* POSS = (struct PosInfo*)tempptr;
	double correctPos = POS_ComputerTarget(POSS);
	return (double)Encoder_ReadBuffer(POSS->encoderChannel) - correctPos; 
}

/**
 *	@brief	Output Function of Position Mode
 *
 *	Set the Motor PWM with PID Result
 *
 *	@param	tempptr	Pointer to Position Mode information
 *	@param	result	PID result
 * */
void PosInfoOutFunc(void* tempptr,double result){
	struct PosInfo* POSS = (struct PosInfo*)tempptr;
	Motor_SetPWM(POSS->motor,result);
	return;
}

/**
 *	@brief	Error calculation function of Velocity Mode
 *
 *	Difference between the current velocity and the target velocity.
 *	Current velocity is approximated by the difference between the last count
 *	and current count.
 *	@param	tempptr	Pointer to Velocity Mode information
 *	@return	Error term
 * */
double VelInfoErrCal(void* tempptr){
	struct VelInfo* VELS = (struct VelInfo*)tempptr;
	double currentSpeed;
	int newPos = Encoder_ReadBuffer(VELS->encoderChannel);
	currentSpeed = newPos - VELS->oldPos;
	VELS->oldPos = newPos;
	return currentSpeed - VELS->targetSpeed;
}

/**
 *	@brief	Output Function of Velocity Mode
 *
 *	Set the Motor PWM with PID Result
 *
 *	@param	tempptr	Pointer to Velocity Mode information
 *	@param	result	PID result
 * */
void VelInfoOutFunc(void* tempptr,double result){
	struct VelInfo* VELS = (struct VelInfo*)tempptr;
	Motor_SetPWM(VELS->motor,result);
	return;
}

/**
 *	@brief	Error calculation function of Theta Mode
 *	
 *	Difference between target theta and current theta
 *	@param	tempptr	Pointer to Theta Mode information
 *	@return Error term
 * */
double ThetaInfoErrCal(void* tempptr){
	struct ThetaInfo* THES = (struct ThetaInfo*)tempptr;
	return THES->targetTheta - THES->pos->theta; 
}

/**
 *	@brief	Output Function of Theta Mode
 *
 *	Set the targetspeed of the two Velocity PID substrated / added by the PID
 *	result.
 *	
 *	@param	tempptr	Pointer to Theta Mode information
 *	@param	result	PID result
 * */
void ThetaInfoOutFunc(void* tempptr,double result){
	struct ThetaInfo* THES = (struct ThetaInfo*)tempptr;
	if(result < 0){
		THES->motorL->targetSpeed -= result;
	} else {
		THES->motorR->targetSpeed -= result;
	}
	return;
}

/**
 *	@brief Init Function of Position Mode
 *
 *	Calculate the time boundaries.
 *
 * 	@see	PosInfo
 *	@param	PIDS			PID Struct to be inited as Position Mode
 *	@param	acc				Accleration
 *	@param	topSpeed		Top Speed
 *	@param	dece			Deceleration
 *	@param	targetPos		Target Position
 *	@param	initPos			Init Position
 *	@param	motor			Motor under control
 *	@param	encoderChannel	Encoder Channel
 *	@param	divider			Update Divider
 *
 *	@return	Pointer to inited Position Mode PID Struct
 * */
struct PIDStruct* PID_Init_Pos(struct PIDStruct* PIDS,double acc,double topSpeed,double dece,int targetPos,int initPos,struct Motor_Struct* motor,int encoderChannel,int divider){
	struct PosInfo* POSS;
	double posDiff = targetPos - initPos;
	if(posDiff < 0)
		posDiff = -posDiff;

	POSS = (struct PosInfo*)malloc(sizeof(struct PosInfo));
	POSS->acceleration = acc;
	POSS->topSpeed =topSpeed;
	POSS->deceleration = dece;
	POSS->targetPos = targetPos;
	POSS->initPos = initPos;
	POSS->time = 0;

	POSS->T1 = POSS->topSpeed / POSS->acceleration;
	POSS->Tend = 	(posDiff / POSS->topSpeed)
		+	(POSS->topSpeed / (POSS->acceleration * 2.0))
		+	(POSS->topSpeed / (POSS->deceleration * 2.0));
	POSS->T2 = POSS->Tend - (POSS->topSpeed / POSS->deceleration);

	POSS->timeRegion = 0;
	//	POSS->pwmChannel= pwmChannel;
	POSS->motor = motor;
	POSS->encoderChannel = encoderChannel;

	return PID_Init(PIDS,PosInfoErrCal,PosInfoOutFunc,POSS,divider);
}

/**
 *	@brief	Init Function of Velocity Mode
 *	
 *	@param	PIDS			PID Struct to be inited as Velocity Mode
 *	@param	targetSpeed		Target Velocity
 *	@param	motor			Motor under control
 *	@param	encoderChannel	Encoder Channel
 *	@param	divider			Update Divider
 *
 *	@return	Pointer to inited Velocity Mode PID Struct
 * */
struct PIDStruct* PID_Init_Vel(struct PIDStruct* PIDS,double targetSpeed,struct Motor_Struct* motor,int encoderChannel,int divider){
	struct VelInfo* VELS;
	VELS = (struct VelInfo*)malloc(sizeof(struct VelInfo));
	VELS->targetSpeed = targetSpeed;
	VELS->oldPos = 0;
	//	VELS->pwmChannel = pwmChannel;
	VELS->motor = motor;
	VELS->encoderChannel = encoderChannel;
	return PID_Init(PIDS,VelInfoErrCal,VelInfoOutFunc,VELS,divider);
}

/**
 *	@brief	Init Function of Theta Mode
 *	
 *	@param	PIDS			PID Struct to be inited as Theta Mode
 *	@param	targetTheta		Target Theta
 *	@param	motorL			Left Motor Velocity Mode PID Struct
 *	@param	motorR			Right Motor Velocity Mode PID Struct
 *	@param	pos				Position Struct
 *	@param	divider			Update Divider
 *
 *	@return	Pointer to inited Velocity Mode PID Struct
 * */
struct PIDStruct* PID_Init_Theta(struct PIDStruct* PIDS,double targetTheta,struct VelInfo* motorL, struct VelInfo* motorR, struct Pos* pos,int divider){
	struct ThetaInfo* THES;
	THES = (struct ThetaInfo*)malloc(sizeof(struct ThetaInfo));
	THES->targetTheta = targetTheta;
	THES->pos = pos;
	THES->motorL = motorL;
	THES->motorR = motorR;
	return PID_Init(PIDS,ThetaInfoErrCal,ThetaInfoOutFunc,THES,divider);
}
