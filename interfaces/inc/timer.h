#pragma once

#include "mc_hardware_interfaces_timer.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_rcc.h"

struct clkTimBaseCfg {
	const uint32_t					period;					// 0..0xFFFF или 0..0xFFFFFFFF
	const uint32_t					prescaler;				// 0..0xFFFF или 0..0xFFFFFFFF.

	/// Используется только в:
	/// 1. timCompOneChannelCfg
	const uint16_t					pulse;					// 0..0xFFFF или 0..0xFFFFFFFF.
};

struct timCounterCfg {
	// Используемый таймер.
	TIM_TypeDef*					tim;

	const clkTimBaseCfg*			const cfg;
	uint32_t						countCfg;
};

class TimCounter : public TimCounterBase {
public:
	TimCounter( const timCounterCfg* const cfg );

	BASE_RESULT		reinit					( uint32_t numberCfg = 0 );

	BASE_RESULT		on						( void );
	void			off						( void );

	uint32_t		getCounter				( void );

private:
	const timCounterCfg*		const cfg;

	TIM_HandleTypeDef				tim;
};

struct timCompOneChannelCfg {
	// Используемый таймер.
	TIM_TypeDef*					tim;

	const clkTimBaseCfg*			const cfg;
	uint32_t						countCfg;

	// Параметры выходного канала.
	uint8_t							outChannel;		// TIM_CHANNEL_x.
	uint32_t						polarity;		// TIM_OCPOLARITY_LOW  / TIM_OCPOLARITY_HIGH.

};

class TimCompOneChannel : public TimCompOneChannelBase {
public:
	TimCompOneChannel( const timCompOneChannelCfg* const cfg );

	BASE_RESULT		reinit					( uint32_t numberCfg = 0 );

	BASE_RESULT		on						( void );
	void			off						( void );

private:
	const timCompOneChannelCfg*		const cfg;

	TIM_HandleTypeDef				tim;
	TIM_OC_InitTypeDef				timCh;
};

struct timPwmOneChannelCfg {
	// Используемый таймер.
	TIM_TypeDef*					tim;

	const clkTimBaseCfg*			const cfg;
	uint32_t						countCfg;

	// Параметры выходного канала.
	uint8_t							outChannel;			// TIM_CHANNEL_x.
	uint32_t						polarity;			// TIM_OCPOLARITY_LOW  / TIM_OCPOLARITY_HIGH.
};

class TimPwmOneChannel : public TimPwmOneChannelBase {
public:
	TimPwmOneChannel( const timPwmOneChannelCfg* const cfg );

	BASE_RESULT		reinit					( uint32_t numberCfg = 0 );

	BASE_RESULT		on						( void );
	void			off						( void );

	void			setDuty					( float duty );

private:
	const timPwmOneChannelCfg*		const cfg;

	TIM_HandleTypeDef				tim;
	TIM_OC_InitTypeDef				timCh;
};

struct timInterruptCfg {
	TIM_TypeDef*					tim;
	const clkTimBaseCfg*			const cfg;
	uint32_t						countCfg;
};

class TimInterrupt : public TimInterruptBase {
public:
	TimInterrupt( const timInterruptCfg* const cfg );

	BASE_RESULT		reinit							( uint32_t numberCfg = 0 );

	BASE_RESULT		on								( void );
	void			off								( void );

	void			clearInterruptFlag				( void );

private:
	const timInterruptCfg*			const cfg;

	TIM_HandleTypeDef				tim;
};

struct timEncoderCfg {
	TIM_TypeDef		*tim;	// Сущность таймера
	uint32_t 		period;	// Потолок инкрементирования после которого счётчик сбросится в 0 и продолжить свою работу
	uint32_t		countCfg;
};


class TimEncoder : public TimEncoderBase{
public:
	TimEncoder(const timEncoderCfg* const _cfg); // cfg - указатель на структуру или на массив структур

	BASE_RESULT		reinit							( uint32_t numberCfg = 0 );

	BASE_RESULT		on								( void );
	void			off								( void );

	uint32_t		getEncoderCounts			( void );

private:
	TIM_HandleTypeDef				tim;

	const timEncoderCfg* const 		cfg;
};

struct controlChannelCapture {
	/// Начало захвата.
	uint32_t				startPos;

	/// Количество переполнений.
	uint32_t				countUpdate;

	/// Последний захват.
	uint32_t 				stopPos;

	/// Период, зафексированный ранее на канале.
	uint32_t				periodTick;

	/*!
	 * Блокирован ли канал.
	 * >0 - да, блокирован на look переполнений таймера.
	 * 0 - нет, активен.
	 */
	uint32_t 				lockUpCount;

	/*!
	 * Был ли канал блокирован ранее?
	 * Сбрасывается вызовом метода getStateChannel
	 * для соответсвующего канала.
	 * Если в момент вызова метода канал был по
	 * прежнему заблокирован, то сброса не происходит.
	 */
	bool					flagLock;

	/*!
	 * Флаг обновления частоты на канале.
	 * Устанавливается в true всякий раз,
	 * когда было зафиксировано событие захвата частоты.
	 */
	bool 					updateFlag;

	/*!
	 * Флаг того, что на канале НЕ БЫЛ зафиксирован
	 * максимально возможный период (пропала частота).
	 *
	 * Иначе говоря, флаг того, что частота на канале присутствует.
	 *
	 * Сбрасывается автоматически при появлении
	 * частоты на канале.
	 *
	 * False		-	на канале зафиксировано отсутствие частоты
	 * 					или с момента включения еще не было захвата.
	 *
	 * True			-	на канале стабильная частота.
	 */
	bool 					frequencyPresent;
};


struct timCaptureCfg {
	TIM_TypeDef*					tim;

	const uint16_t					prescaler;						// 0..0xFFFF.

	/*!
	 * Минимальное количество тиков между импульсами.
	 * Это защита от помех или дикой частоты
	 * (которая может занять все процессорное время).
	 */
	const uint32_t					minTickValue;

	/*!
	 * Количество переполнений таймера
	 * перед восстановлением отключенного канала.
	 */
	const uint32_t					countUpForResetChannel;

	/*!
	 * Максимально возможный период на канале. Выше него считается,
	 * что период равен бесконечности, а частота при это равна нулю.
	 * Защита на тот случай, когда двигатель остановится, а между
	 * остановкой и предыдущим измерением будет какой-то период.
	 *
	 * Период указывается в количестве переполнений таймера.
	 */
	const uint32_t					maxCountUpTimerPeriod;
};

class TimCapture : public TimCaptureBase {
public:
	TimCapture( const timCaptureCfg* const cfg, const uint32_t countCfg );

	bool	reinit					( uint32_t numberCfg = 0 );

	bool 	on						( void );
	bool 	off						( void );

	bool 	getFrequency			( const uint32_t channel, float& returnFrequency );
	bool 	getStateChannel			( const uint32_t channel );
	bool	getFlagCaptureChannel	( const uint32_t channel );

		void 	timHandler				( void );

private:
	const timCaptureCfg*			const cfg;
	const uint32_t					countCfg;

	/// Текущая конфигурация модуля захвата.
	uint32_t 						nowCfg;

	TIM_HandleTypeDef				tim;

	/// Bit Banding адреса бит прерываний по захвату.
	uint32_t*						captureFlag[ 4 ];

	/// Флаг переполнения.
	uint32_t*						updateFlag;

	/// Адреса регистров захваченных точек захвата.
	uint32_t*						captureData[ 4 ];

	/// Текущее состояние каждого канала
	/// (начало захвата, количество переполнений, последний захват).
	controlChannelCapture			channelControl[ 4 ];

	/// Время одного тика таймера в секундах.
	double 							tickPeriod;

	/// Имена каналов для удобного обращения к методам HAL.
	const uint32_t channelHalName[ 4 ] = {
		TIM_CHANNEL_1, TIM_CHANNEL_2,
		TIM_CHANNEL_3, TIM_CHANNEL_4
	};
};
