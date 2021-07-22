#ifndef __PID_H__
#define __PID_H__

#include "main.h"
#include <math.h>

typedef int16_t s16;
typedef int32_t s32;
typedef uint16_t u16;

struct PIDCircle
{
    float kp, ki, kd;
    s32 tgt;
    s16 i_band, i_limit;
    s16 sum_limit;
    s32 *cur;
    u16 maxE;
    
    s16 p_out, i_out, d_out;
    s16 e;
    s16 last_cur;
    s16 out;

};

extern struct PIDCircle pidBalance, pidStartRotating, pidMotorSpd;
extern int abs(int);

void PIDSetTarget(struct PIDCircle* pid, s16 target);

void PIDInit();
void PIDReset(struct PIDCircle* pid);
s16 PIDUpdate(struct PIDCircle* pid);

#endif
