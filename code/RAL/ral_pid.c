#include <LPC213x.h>
#include <stdlib.h>
#include "ral_pid.h"

struct PIDStruct* PIDList[MAX_PID_CHANNEL]={NULL};
int PIDListCount = 0;


struct PIDStruct* PID_Init(struct PIDStruct* PIDS,double (*err_Calc)(void* info),void (*output_Func)(void* info, double err),void *info){
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
    return;
}

void PID_UpdateAll(void){
    int i;
    for(i = 0; i < PIDListCount; i++)
	PID_Update(PIDList[i]);
    return;
}
double POS_ComputerTarget(struct PosInfo* POSS)
	double s;
	double T1,T2,Tend;
	int t= POSS->time;
	int time_region = 0;

	if(POSS->T1 < POSS->T2){
		if(POSS->time < POSS->T1){
			POSS->time_region = 1;		
		} else if(POSS->time < POSS->T2){
			POSS->time_region = 2;
		} else if(POSDS->time < POSS->Tend){
			POSS->time_region = 3;
		} else {
			POSS->time_region = 4;
		}
	} else {
		if(POSS->time < POSS->Tend / 2){
			POSS->time_region = 1;
		} else if(POSS->time < Tend){
			POSS->time_region = 3;
		} else {
			POSS->time_region = 4;
		}
	}	
	time_region = POSS->time_region;
	T1 = POSS->T1;
	T2 = POSS->T2;
	Tend = POSS->Tend;
	if(POSS->init_pos < POSS->target_pos){

		switch(time_region){
			case 1:
				s = t * t * POSS->acceleration / 2 + (double)POSS->init_pos;
			break;
			case 2:
				s =   (double)POSS->top_speed * ( t - T1) 
					+ (double)POSS->top_speed * POSS->top_speed / (2 * POSS->acceleration)
					//+ (double)POSS->top_speed / 2.0 * T1
					+ (double)POSS->init_pos;
			break;
			case 3:
				s =   -1 * (t-Tend) * (t-Tend) * POSS->deceleration / 2 + (double)POSS->target_pos;
			break;
			case 4:
				s =   POSS->target_pos;
			break;
		}
	}
	
	// the case that the curve decreases with time
	else{
		switch(time_region){
			case 1:
				s = -1 * t * t * POSS->acceleration / 2 + (double)POSS->init_pos;
			break;
			case 2:
				s =  -1* (double)POSS->top_speed * ( t - T1) 
					- (double)POSS->top_speed * POSS->top_speed / (2 * POSS->acceleration)
					+ (double)POSS->init_pos;
			break;
			case 3:
				s =   (t-Tend) * (t-Tend) * POSS->deceleration / 2 + (double)POSS->target_pos;
			break;
			case 4:
				s =   POSS->target_pos;
			break;
		}
	
	}
	return s;	

}
float PosInfoErrCal(struct PosInfo* POSS){
	double correctPos = POS_ComputerTarget(POSS);
	return (double)Encoder_ReadBuffer(POSS->encoderChannel) - correctPos; 
}

float VelInfoErrCal(struct VelInfo* VELS){
	double currentSpeed;
	int newPos = Encoder_ReadBuffer(VELS->encoderChannel);
	currentSpeed = newPos - VELS->oldPos;
	VELS->oldPos = newPos;
	return currentSpeed - VELS->targetSpeed;
}

float ThetaInfoErrCal(struct ThetaInfo* THES){
	return THES->targetTheta - THES->POS->theta; 
}
