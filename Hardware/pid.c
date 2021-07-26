#include "pid.h"

#include <math.h>

#ifdef USER_PID_DEBUG
#define user_pid_printf(format, ...) HostSendLog(LOG_COLOR_BLACK, format "\r\n", ##__VA_ARGS__)
#define user_pid_info(format, ...) HostSendLog(LOG_COLOR_BLACK, "[\tpid]info:" format "\r\n", ##__VA_ARGS__)
#define user_pid_debug(format, ...) HostSendLog(LOG_COLOR_GREEN, "[\tpid]debug:" format "\r\n", ##__VA_ARGS__)
#define user_pid_error(format, ...) HostSendLog(LOG_COLOR_RED, "[\tpid]error:" format "\r\n",##__VA_ARGS__)
#else
#define user_pid_printf(format, ...)
#define user_pid_info(format, ...)
#define user_pid_debug(format, ...)
#define user_pid_error(format, ...)
#endif

struct PIDCircle pidBalance, pidStartRotating, pidMotorSpd;

void PIDSetTarget(struct PIDCircle* pid, s16 target)
{
    
    pid->tgt = target;
    
}

s32 limit(s32 val, s32 tgt)
{
    
    if( abs(val) > tgt )
        return val > 0 ? tgt : -tgt;
    else
        return val;
    
}

s32 Normalize(s32 val, s32 max)
{
    return (val + max * 5) % (max * 2) - max;
}

void PIDInit()
{
    // pid 平衡环
    pidBalance.kp = -10;
    pidBalance.ki = -1;
    pidBalance.kd = -30;
    pidBalance.tgt = 0;
    pidBalance.i_band = 50;
    pidBalance.i_limit = 500;
    pidBalance.sum_limit = 1000;
    PIDReset(&pidBalance);
    
    // pid 起摆正反馈环
    pidStartRotating.kp = 0;
    pidStartRotating.ki = 0;
    pidStartRotating.kd = -20;
    pidStartRotating.tgt = 0;
    pidStartRotating.i_band = 10;
    pidStartRotating.i_limit = 100;
    pidStartRotating.sum_limit = 300;
    PIDReset(&pidStartRotating);
    
    // pid 电机速度环
    pidMotorSpd.kp = 10;
    pidMotorSpd.ki = 0;
    pidMotorSpd.kd = 5;
    pidMotorSpd.tgt = 0;
    pidMotorSpd.i_band = 10;
    pidMotorSpd.i_limit = 100;
    pidMotorSpd.sum_limit = 1000;
    PIDReset(&pidMotorSpd);
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
    
    pid->e = Normalize( *(pid->cur) - pid->tgt, pid->maxE);
    
    pid->d_out = pid->kd * Normalize( *(pid->cur) - pid->last_cur, pid->maxE); //计算d
    pid->last_cur = *(pid->cur);
    
    pid->p_out = pid->kp * pid->e;
    
    if( abs( pid->e ) > pid->i_band && pid->i_band > 0 )
        pid->i_out = 0;
    else
        pid->i_out += pid->ki * pid->e;
    
    pid->i_out = limit(pid->i_out, pid->i_limit );
    
    pid->out = pid->p_out + pid->i_out + pid->d_out;
    
    pid->out = limit(pid->out, pid->sum_limit);
    
    return pid->out;
    
}
