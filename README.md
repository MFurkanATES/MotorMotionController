Copyright
===============

	Copyright @2018 T.WKVER </MATRIX>
	Code by </MATRIX>@Neod Anderjon(LeaderN)
	Thank to fork or watch my project

Update
===============

	最后更新时间: 2018/11/04pm1909

Project Manual
==============

	</MATRIX>提供嵌入式开发维护支持

	通用嵌入式运动控制器Demo
	该工程基于框架EmbeddBreakerCore搭建，专用于工业电机的控制
	使用高级定时器的更新中断通道发一路S形加减速脉冲用于控制
	自带24V低电平有效限位传感器连接，可安装于滑轨上

	主控MCU：		STM32F103RET6
	芯片资源：		64脚芯片 256k flash 64k SRAM
	嵌入式系统：	μC/OS-III v3.03

	Frame name: 	EmbeddedBreakerCore
	Developer: 		Neod Anderjon 
	Organization: 	</MATRIX>
	Laboratory: 	T.WKVER
	OS_Version: 	μC/OS-III v3.03
	FWLib_Verson: 	v3.5
	Project_Verson: OS_v0p5_LTE 长期演进版(什么时候TMD才能跑出个stable来啊)_(:з」∠)_ _(┐「ε:)_
	
Use Manual
===============

	同一头文件包含头文件：stdafx.h Windows编程标准包含
	使用前请先打开config.c config.h文件按照注释提示修改系统资源配置
	之后详细阅读README.md，并且保证自己很清醒
	更新框架覆盖时不覆盖EmbeddedBreakerCore\Modules目录和外部应用目录
	频率精确度矫正系数需要根据主频来进行变换
	
Inner IO Map
===============

	(标注有*表示该IO口在PCB上使用ULN2003A驱动，该芯片有反相特性)

	引脚	用途		I/O		备注

	PA0 ---- WKUPIO		I		
	PA1 ---- RAILGUN			*
	PA2/PB0  					*
	PA3/PB1  					*
	PA4 ---- 					*
	PA5 ---- 					*
	PA6 ---- Dir_IO				*
	PA7 ---- Pulse_IO			*
	PA8 ---- LED0
	PA9/PB6  TXD		
	PA10/PB7 RXD		I
	PA11 --- DV_IO				*
	PA12 --- AV_IO				*
	PA13 --- SWDIO		I
	PA14 --- SWCLK
	PA15 --- KEY1		I	
	
	PB0/PA2  Pulse_IO			*
	PB1/PA3  					*
	PB2 ----
	PB3 ---- 			I
	PB4 ---- 			I
	PB5 ----
	PB6 ----
	PB7 ----
	PB8 ---- STEWIO		I
	PB9 ----
	PB10 --- 
	PB11 --- 	
	PB12 --- A1USR		I
	PB13 --- OLED_SCL
	PB14 --- A1DSR		I
	PB15 --- OLED_SDA	I
	
	PC0 ---- 			I
	PC1 ---- 			I
	PC2 ---- ENCODE_Z 	I
	PC3 ---- 			I
	PC4 ---- BEEP
	PC5 ---- KEY0		I
	PC6 ---- ENCODE_A			*
	PC7 ---- ENCODE_B			*
	PC8 ---- 					*
	PC9 ---- 					*
	PC10 --- LVDIO		I
	PC11 --- LED2
	PC12 --- LED3
	PC13 --- 					*
	PC14 --- OSC
	PC15 --- OSC
	
	PD0 ---- OSC
	PD1 ---- OSC
	PD2 ---- LED1
	PD5/PA2  
	PD6/PA3  			
	
