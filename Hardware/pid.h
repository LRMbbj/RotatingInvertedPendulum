#ifndef __PID_H__
#define __PID_H__

#include "main.h"

typedef int16_t s16;

struct PIDCircle
{
	float kp, ki, kd;
	s16 tgt;
	s16 i_band, i_limit;
	s16 sum_limit;
	s16 *cur;
	
	s16 p_out, i_out, d_out;
	s16 e;
	s16 last_cur;
	s16 out;

};

void PIDSetTarget(struct PIDCircle* pid, s16 target);

void PIDReset(struct PIDCircle* pid);
s16 PIDUpdate(struct PIDCircle* pid);

#endif
