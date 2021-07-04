#include "pid.h"

#include <math.h>


void PIDSetTarget(struct PIDCircle* pid, s16 target)
{
	
	pid->tgt = target;
	
}

void limit(s16* val, s16 tgt)
{
	
	if( abs(*val) > tgt )
		*val = *val > 0 ? tgt : -tgt;
	
}

void PIDReset(struct PIDCircle* pid)
{
	
	pid->e = 0;
	pid->last_cur = 0;
	pid->out = 0;
	pid->p_out = 0;
	pid->i_out = 0;
	pid->d_out = 0;
	
}

s16 PIDUpdate(struct PIDCircle* pid)
{
	
	pid->e = *(pid->cur) - pid->tgt;
	
	pid->d_out = pid->kd * ( *(pid->cur) - pid->last_cur ); //¼ÆËãd
	pid->last_cur = *(pid->cur);
	
	pid->p_out = pid->kp * pid->e;
	
	if( abs( pid->e ) > pid->i_band && pid->i_band > 0 )
		pid->i_out = 0;
	else
		pid->i_out += pid->ki * pid->e;
	
	limit(&(pid->i_out), pid->i_limit );
	
	pid->out = pid->p_out + pid->i_out + pid->d_out;
	
	limit(&(pid->out), pid->sum_limit);
	
	return pid->out;
	
}
	