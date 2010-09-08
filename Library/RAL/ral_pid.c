#include <stdlib.h>
#include "ral_pid.h"
#include "ral_encoder.h"

static struct PIDStruct* PIDList[MAX_PID_CHANNEL]={NULL};
static int PIDListCount = 0;
static unsigned int PIDUpdateCount = 0;
static unsigned int MaxCount = 1;


struct PIDStruct* PID_Init(struct PIDStruct* PIDS,double (*err_Calc)(void* info),void (*output_Func)(void* info, double err),void *info,int divider){
	//if(PIDS == NULL)
	//PIDS = (struct PIDStruct*)malloc(sizeof(struct PIDStruct));

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


void PID_SetParameter(struct PIDStruct* PIDS,double kp,double ki,double kd,double imax,double tolerance){
	PIDS->kp = kp;
	PIDS->ki = ki;
	PIDS->kd = kd;
	PIDS->imax = imax;
	PIDS->tolerance = tolerance;
	return;
}

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

void PID_Start(struct PIDStruct* PIDS){
	PIDS->start = 1;
	return;
}

void PID_Stop(struct PIDStruct* PIDS){
	PIDS->start = 0;
	return;
}

int PID_Search_List(struct PIDStruct* PIDS){
	int i;
	for(i = 0; i < PIDListCount; i++)
		if(PIDList[i] == PIDS) return i;
	return -1;
}

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

void PID_UpdateAll(void){
	int i;
	PIDUpdateCount++;
	for(i = 0; i < PIDListCount; i++){
		if(PIDUpdateCount == PIDList[i]->update_divider)
			PID_Update(PIDList[i]);
	}
	if(PIDUpdateCount == MaxCount)PIDUpdateCount = 0;
	return;
}
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
double PosInfoErrCal(void* tempptr){
	struct PosInfo* POSS = (struct PosInfo*)tempptr;
	double correctPos = POS_ComputerTarget(POSS);
	return (double)Encoder_ReadBuffer(POSS->encoderChannel) - correctPos; 
}

void PosInfoOutFunc(void* tempptr,double result){
	struct PosInfo* POSS = (struct PosInfo*)tempptr;
	POSS->motor->Motor_SetPWM(POSS->motor,result);
	return;
}
double VelInfoErrCal(void* tempptr){
	struct VelInfo* VELS = (struct VelInfo*)tempptr;
	double currentSpeed;
	int newPos = Encoder_ReadBuffer(VELS->encoderChannel);
	currentSpeed = newPos - VELS->oldPos;
	VELS->oldPos = newPos;
	return currentSpeed - VELS->targetSpeed;
}

void VelInfoOutFunc(void* tempptr,double result){
	struct VelInfo* VELS = (struct VelInfo*)tempptr;
	VELS->motor->Motor_SetPWM(VELS->motor,result);
	return;
}

double ThetaInfoErrCal(void* tempptr){
	struct ThetaInfo* THES = (struct ThetaInfo*)tempptr;
	return THES->targetTheta - THES->pos->theta; 
}

void ThetaInfoOutFunc(void* tempptr,double result){
	struct ThetaInfo* THES = (struct ThetaInfo*)tempptr;
	if(result < 0){
		THES->motorL->targetSpeed -= result;
	} else {
		THES->motorR->targetSpeed -= result;
	}
	return;
}

struct PIDStruct* PID_Init_Pos(struct PIDStruct* PIDS,double acc,double topSpeed,double dece,int targetPos,int initPos,struct Motor_Setting* motor,int encoderChannel,int divider){
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

struct PIDStruct* PID_Init_Vel(struct PIDStruct* PIDS,double targetSpeed,struct Motor_Setting* motor,int encoderChannel,int divider){
	struct VelInfo* VELS;
	VELS = (struct VelInfo*)malloc(sizeof(struct VelInfo));
	VELS->targetSpeed = targetSpeed;
	VELS->oldPos = 0;
	//	VELS->pwmChannel = pwmChannel;
	VELS->motor = motor;
	VELS->encoderChannel = encoderChannel;
	return PID_Init(PIDS,VelInfoErrCal,VelInfoOutFunc,VELS,divider);
}

struct PIDStruct* PID_Init_Theta(struct PIDStruct* PIDS,double targetTheta,struct VelInfo* motorL, struct VelInfo* motorR, struct Pos* pos,int divider){
	struct ThetaInfo* THES;
	THES = (struct ThetaInfo*)malloc(sizeof(struct ThetaInfo));
	THES->targetTheta = targetTheta;
	THES->pos = pos;
	THES->motorL = motorL;
	THES->motorR = motorR;
	return PID_Init(PIDS,ThetaInfoErrCal,ThetaInfoOutFunc,THES,divider);
}
