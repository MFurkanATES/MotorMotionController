#pragma once
#include "stdafx.h"
//code by </MATRIX>@Neod Anderjon
//author: Neod Anderjon
//====================================================================================================
/*
	电机驱动脉冲底层框架模块
	采用高级定时器1、8对移动平台机械臂分别控制
	ULN2003A具有IO反相驱动增强功能，使用时5V口悬空，IO输出口接上拉电阻拉到电源
	上拉电阻选择：5V -- 3.3k 24V -- 4.7k
	通用分频数=500000/定时器时基
*/

//脉冲IO口
#define IO_MainPulse 					PAout(7)		//主脉冲输出
#define IO_Direction 					PAout(6)		//方向线输出

#define OneLoopPerPulse 				1600.f			//实际脉冲个数/圈(细分数x(360度/步距角))	
#define MaxLimit_Dis					315.f			//滑轨限位，单位mm
#define OneLoopHeight					5.f				//丝杠滑轨一圈变化距离，单位mm
#define RadUnitConst					4.445f			//(float)(OneLoopPerPulse / 360.f)
#define LineUnitConst					320.f			//(float)(OneLoopPerPulse / OneLoopHeight)

#define ResetStartFrequency				7500u			//复位起始频率

//脉冲发送结束后电机驱动IO口的复位状态
#ifdef use_ULN2003A										//ULN2003A反相设置
#define MD_IO_Reset						lvl				//反相拉低
#else 
#define MD_IO_Reset						hvl				//正相拉高
#endif

//方向选择
#ifdef use_ULN2003A										//ULN2003A反相设置
typedef enum {Pos_Rev = 0, Nav_Rev = !Pos_Rev} RevDirection;		
#else
typedef enum {Pos_Rev = 1, Nav_Rev = !Pos_Rev} RevDirection;	
#endif

//电机运行状态(停止/运行)
typedef enum {Run = 1, Stew = !Run} MotorRunStatus;
//电机运行模式(有限运行(位置控制模式)/无限运行(速度控制模式))
typedef enum {PosiCtrl = 0, SpeedCtrl = 1} MotorRunMode;
//线度角度切换(RA/RD)
typedef enum {RadUnit = 0, LineUnit = 1} LineRadSelect;

//电机S形加减速算法
/*
	sigmoid函数原型
	matlab建模原型方程：
	x取10为间隔，从0取到400
	y在1500到2500之间摆动
	测试参数a=0.1 b=50
	>>	x = [0:10:400];
	>>	y = (2500-1500)./(1+exp(-0.1*(x-50)))+1500;
	>>	plot(x, y)
	只取整数进行演算
	调参方法：优化曲线A，B值
	不建议把最低频率设置到0
*/	
#ifndef SIGMOID_FUNCTION
#define SIGMOID_FUNCTION(ymax, ymin, a, b, x)	(u16)((ymax - ymin) / (1 + exp((double)(-a * (x - b)))) + ymin)
#endif

//加减匀速段标记
typedef enum 
{
	asym = 1,											//加速段
	usym = 2,											//匀速段
	dsym = 3,											//减速段
} AUD_Symbol;

//S型加减速X取值个数高精度典型值
//开环系统无法保证绝对精度
typedef enum
{
	R360 	= 10u,										//行程360度以内
	R1080 	= 16u,										//行程1080度以内
	R1800	= 25u,										//行程1800度以内
	R3600 	= 40u,										//行程3600度以内
} XSizeTypicalValue;

//sigmoid函数参数结构体
typedef __packed struct 
{
	u16 	freq_max;									//参数freq_max，设置最高达到频率，但要注意抑制机械振动
	u16 	freq_min;									//参数freq_min，设置最小换向频率
	float 	para_a;										//参数para_a，越小曲线越平滑
	float 	para_b;										//参数para_b，越大曲线上升下降越缓慢
	float 	ratio;										//参数ratio，S形加减速阶段分化比例
	u16 	x_range;									//x取值范围
	u16		table_size;									//加减速表大小
	u16		table_index;								//离散表序列
	u16 	*disp_table;								//整型离散表
} Sigmoid_Parameter;		

//电机调用结构体
typedef __packed struct 						
{
	//基础运动控制
    volatile uint32_t 	ReversalCnt;					//脉冲计数器
	volatile uint32_t	IndexCnt;						//序列计数器
	uint32_t			ReversalRange;					//脉冲回收系数
	uint32_t			IndexRange[3];					//序列回收系数(加速、匀速、减速共三个)
    uint32_t			RotationDistance;				//行距
    uint16_t 			SpeedFrequency;					//设定频率
	volatile uint16_t	divFreqCnt;						//分频计数器
	uint16_t			CalDivFreqConst;				//分频系数
	float 				DivCorrectConst;				//分频数矫正系数
	//电机状态标志
	MotorRunStatus		MotorStatusFlag;				//运行状态
	MotorRunMode		MotorModeFlag;					//运行模式
	LineRadSelect		DistanceUnitLS;					//线度角度切换
	RevDirection		RevDirectionFlag;				//方向标志
	//S型加减速
	Sigmoid_Parameter 	*asp;							//加速参数
	Sigmoid_Parameter	*dsp;							//减速参数
	AUD_Symbol			aud_sym;						//加减匀速阶段标识
} MotorMotionSetting;
extern MotorMotionSetting st_motorAcfg;					//测试步进电机A

//电机控制IO初始化
void PulseDriver_IO_Init (void);
void Direction_IO_Init (void);		

//基于定时器的基本电机驱动
void SigmoidParam_Init (MotorMotionSetting *mcstr);									//创建加减速表
void MotorConfigStrParaInit (MotorMotionSetting *mcstr);							//结构体成员初始化
void TIM1_MecMotorDriver_Init (void);												//高级定时器初始化函数声明		
void TIM1_OutputChannelConfig (uint16_t Motorx_CCx, FunctionalState control);		//定时器输出比较模式通道配置
void DivFreqAlgoUpdate (MotorMotionSetting *mcstr);									//更新分频系数
void DistanceAlgoUpdate (MotorMotionSetting *mcstr);								//更新行距
void MotorWorkBooter (MotorMotionSetting *mcstr);									//电机运行启动
void MotorWorkStopFinish (MotorMotionSetting *mcstr);								//电机运行停止
void MotorPulseProduceHandler (MotorMotionSetting *mcstr);							//电机脉冲产生中断

//运动测试算例
extern void MotorMotionController (u16 spfq, u16 mvdis, RevDirection dir, 
	MotorRunMode mrm, LineRadSelect lrs, MotorMotionSetting *mcstr);				//机械臂运动算例
extern void PeriodUpDnMotion (u16 count, MotorMotionSetting *mcstr);				//滑轨上下测试
extern void RepeatTestMotion (MotorMotionSetting *mcstr);							//传感器限位反复测试
extern void PosNavRepeatMotion (MotorMotionSetting *mcstr, u16 speed, u16 dis);		//正反转重复性测试
extern void Axis_Pos_Reset (MotorMotionSetting *mcstr);								//开机滑轨复位到零点
void OLED_DisplayMotorStatus (MotorMotionSetting *mcstr);

//====================================================================================================
//code by </MATRIX>@Neod Anderjon
