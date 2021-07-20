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

struct PIDCircle pidMotorPos, pidRotateAngle;

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
    return (val + max) % (max * 2) - max;
}

void PIDInit()
{
    // pidM
    //输入参数为线数/100
    pidMotorPos.kp = -5;
    pidMotorPos.ki = 0;
    pidMotorPos.kd = 0;
    pidMotorPos.tgt = 50;
    pidMotorPos.i_band = 10;
    pidMotorPos.i_limit = 200;
    pidMotorPos.sum_limit = 1000;
    PIDReset(&pidMotorPos);
    
    // pidR(未确定)
//    pidRotateAngle.kp = -5;
//    pidRotateAngle.ki = 0;
//    pidRotateAngle.kd = 0;
//    pidRotateAngle.tgt = 50;
//    pidRotateAngle.i_band = 10;
//    pidRotateAngle.i_limit = 200;
//    pidRotateAngle.sum_limit = 1000;
//    PIDReset(&pidRotateAngle);
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
