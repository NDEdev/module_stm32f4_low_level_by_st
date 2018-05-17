#include "timer.h"
#include "port_base.h"

// Включаем тактирование SPI.
static void clkTimInit ( TIM_TypeDef* tim ) {
	switch ( ( uint32_t )tim ) {
#ifdef TIM
		case	TIM_BASE:		__HAL_RCC_TIM_CLK_ENABLE();			break;
#endif
#ifdef TIM1
		case	TIM1_BASE:		__HAL_RCC_TIM1_CLK_ENABLE();		break;
#endif
#ifdef TIM2
		case	TIM2_BASE:		__HAL_RCC_TIM2_CLK_ENABLE();		break;
#endif
#ifdef TIM3
		case	TIM3_BASE:		__HAL_RCC_TIM3_CLK_ENABLE();		break;
#endif
#ifdef TIM4
		case	TIM4_BASE:		__HAL_RCC_TIM4_CLK_ENABLE();		break;
#endif
#ifdef TIM5
		case	TIM5_BASE:		__HAL_RCC_TIM5_CLK_ENABLE();		break;
#endif
#ifdef TIM6
		case	TIM6_BASE:		__HAL_RCC_TIM6_CLK_ENABLE();		break;
#endif
#ifdef TIM7
		case	TIM7_BASE:		__HAL_RCC_TIM7_CLK_ENABLE();		break;
#endif
#ifdef TIM8
		case	TIM8_BASE:		__HAL_RCC_TIM8_CLK_ENABLE();		break;
#endif
#ifdef TIM9
		case	TIM9_BASE:		__HAL_RCC_TIM9_CLK_ENABLE();		break;
#endif
#ifdef TIM10
		case	TIM10_BASE:		__HAL_RCC_TIM10_CLK_ENABLE();		break;
#endif
#ifdef TIM11
		case	TIM11_BASE:		__HAL_RCC_TIM11_CLK_ENABLE();		break;
#endif
#ifdef TIM12
		case	TIM12_BASE:		__HAL_RCC_TIM12_CLK_ENABLE();		break;
#endif
#ifdef TIM13
		case	TIM13_BASE:		__HAL_RCC_TIM13_CLK_ENABLE();		break;
#endif
#ifdef TIM14
		case	TIM14_BASE:		__HAL_RCC_TIM14_CLK_ENABLE();		break;
#endif
#ifdef TIM15
		case	TIM15_BASE:		__HAL_RCC_TIM15_CLK_ENABLE();		break;
#endif
#ifdef TIM16
		case	TIM16_BASE:		__HAL_RCC_TIM16_CLK_ENABLE();		break;
#endif
	};
}


uint32_t getTimBusFreq ( TIM_TypeDef* tim ) {
	switch ( ( uint32_t )tim ) {
#ifdef TIM1
		case TIM1_BASE: return HAL_RCC_GetPCLK2Freq();
#endif

#ifdef TIM2
		case TIM2_BASE: return HAL_RCC_GetPCLK1Freq();
#endif

#ifdef TIM3
		case TIM3_BASE: return HAL_RCC_GetPCLK1Freq();
#endif

#ifdef TIM4
		case TIM4_BASE: return HAL_RCC_GetPCLK1Freq();
#endif

#ifdef TIM5
		case TIM5_BASE: return HAL_RCC_GetPCLK1Freq();
#endif

#ifdef TIM6
		case TIM6_BASE: return HAL_RCC_GetPCLK1Freq();
#endif

#ifdef TIM7
		case TIM7_BASE: return HAL_RCC_GetPCLK1Freq();
#endif

#ifdef TIM8
		case TIM8_BASE: return HAL_RCC_GetPCLK2Freq();
#endif

#ifdef TIM9
		case TIM9_BASE: return HAL_RCC_GetPCLK2Freq();
#endif

#ifdef TIM10
		case TIM10_BASE: return HAL_RCC_GetPCLK2Freq();
#endif

#ifdef TIM11
		case TIM11_BASE: return HAL_RCC_GetPCLK2Freq();
#endif

#ifdef TIM12
		case TIM12_BASE: return HAL_RCC_GetPCLK1Freq();
#endif

#ifdef TIM13
		case TIM13_BASE: return HAL_RCC_GetPCLK1Freq();
#endif

#ifdef TIM14
		case TIM14_BASE: return HAL_RCC_GetPCLK1Freq();
#endif
	}
}

/*!
 * TimCounter.
 */
TimCounter::TimCounter ( const timCounterCfg* const cfg ) : cfg( cfg ) {
	this->tim.Instance						= this->cfg->tim;

	this->tim.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;
	this->tim.Init.CounterMode				= TIM_COUNTERMODE_UP;
}

BASE_RESULT TimCounter::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->cfg->countCfg )
		return BASE_RESULT::INPUT_VALUE_ERROR;

	this->tim.Init.Period					= this->cfg->cfg[ numberCfg ].period;
	this->tim.Init.Prescaler				= this->cfg->cfg[ numberCfg ].prescaler;

	clkTimInit( this->tim.Instance );

	if ( HAL_TIM_Base_DeInit( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_TIM_Base_Init( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	return BASE_RESULT::OK;
}

BASE_RESULT TimCounter::on ( void ) {
	if ( this->tim.State == HAL_TIM_STATE_RESET )
		return BASE_RESULT::ERROR_INIT;

	HAL_TIM_Base_Start( &this->tim );

	return BASE_RESULT::OK;
}

void TimCounter::off ( void ) {
	HAL_TIM_Base_Stop( &this->tim  );
}

uint32_t TimCounter::getCounter ( void ) {
	return this->tim.Instance->CNT;
}

//**********************************************************************
// tim_comp_one_channel
//**********************************************************************
TimCompOneChannel::TimCompOneChannel ( const timCompOneChannelCfg* const cfg ) : cfg( cfg ) {
	this->tim.Instance						= this->cfg->tim;

	this->tim.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;
	this->tim.Init.CounterMode				= TIM_COUNTERMODE_UP;

	this->timCh.OCMode						= TIM_OCMODE_TOGGLE;
	this->timCh.OCPolarity					= this->cfg->polarity;
	this->timCh.Pulse						= 0;
}

BASE_RESULT TimCompOneChannel::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->cfg->countCfg )
		return BASE_RESULT::INPUT_VALUE_ERROR;

	this->tim.Init.Period					= this->cfg->cfg[ numberCfg ].period;
	this->tim.Init.Prescaler				= this->cfg->cfg[ numberCfg ].prescaler;
	this->timCh.Pulse						= this->cfg->cfg[ numberCfg ].pulse;

	clkTimInit( this->tim.Instance );

	if ( HAL_TIM_OC_DeInit( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_TIM_OC_Init( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_TIM_OC_ConfigChannel( &this->tim, &this->timCh, this->cfg->outChannel ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	return BASE_RESULT::OK;
}

BASE_RESULT TimCompOneChannel::on ( void ) {
	if ( this->tim.State == HAL_TIM_STATE_RESET )
		return BASE_RESULT::ERROR_INIT;

	HAL_TIM_OC_Start( &this->tim, this->cfg->outChannel );

	return BASE_RESULT::OK;
}

void TimCompOneChannel::off ( void ) {
	HAL_TIM_OC_Stop( &this->tim, this->cfg->outChannel );
}

//**********************************************************************
// tim_pwm_one_channel
//**********************************************************************
TimPwmOneChannel::TimPwmOneChannel ( const timPwmOneChannelCfg* const cfg ) : cfg( cfg ) {
	this->tim.Instance						= this->cfg->tim;

	this->tim.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;
	this->tim.Init.CounterMode				= TIM_COUNTERMODE_UP;

	this->timCh.OCMode						= TIM_OCMODE_PWM2;
	this->timCh.OCPolarity					= this->cfg->polarity;
}

BASE_RESULT TimPwmOneChannel::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->cfg->countCfg )
		return BASE_RESULT::INPUT_VALUE_ERROR;

	this->tim.Init.Period					= this->cfg->cfg[ numberCfg ].period;
	this->tim.Init.Prescaler				= this->cfg->cfg[ numberCfg ].prescaler;

	clkTimInit( this->cfg->tim );

	if ( HAL_TIM_PWM_DeInit( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_TIM_PWM_Init( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_TIM_PWM_ConfigChannel( &this->tim, &this->timCh, this->cfg->outChannel ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	return BASE_RESULT::OK;
}

BASE_RESULT TimPwmOneChannel::on ( void ) {
	if ( this->tim.State == HAL_TIM_STATE_RESET )
		return BASE_RESULT::ERROR_INIT;

	HAL_TIM_PWM_Start( &this->tim, this->cfg->outChannel );

	return BASE_RESULT::OK;
}

void TimPwmOneChannel::off ( void ) {
	HAL_TIM_PWM_Stop( &this->tim, this->cfg->outChannel );
}

void TimPwmOneChannel::setDuty ( float duty ) {
	__HAL_TIM_SET_COMPARE( &this->tim, this->cfg->outChannel, this->cfg->tim->ARR * duty );
}

//**********************************************************************
// tim_interrupt
//**********************************************************************
TimInterrupt::TimInterrupt( const timInterruptCfg* const cfg ) : cfg( cfg ) {
	this->tim.Instance						= this->cfg->tim;

	this->tim.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;
	this->tim.Init.CounterMode				= TIM_COUNTERMODE_UP;
}

BASE_RESULT TimInterrupt::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->cfg->countCfg )
		return BASE_RESULT::INPUT_VALUE_ERROR;

	this->tim.Init.Period					= this->cfg->cfg[ numberCfg ].period;
	this->tim.Init.Prescaler				= this->cfg->cfg[ numberCfg ].prescaler;

	clkTimInit( this->cfg->tim );

	if ( HAL_TIM_Base_DeInit( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_TIM_Base_Init( &this->tim ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	return BASE_RESULT::OK;
}

BASE_RESULT TimInterrupt::on ( void ) {
	if ( this->tim.State == HAL_TIM_STATE_RESET )
		return BASE_RESULT::ERROR_INIT;

	HAL_TIM_Base_Start_IT( &this->tim );

	return BASE_RESULT::OK;
}

void TimInterrupt::off ( void ) {
	HAL_TIM_Base_Stop_IT( &this->tim );
}

void TimInterrupt::clearInterruptFlag ( void ) {
	HAL_TIM_IRQHandler( &this->tim );
}

//**********************************************************************
// tim_encoder
//**********************************************************************

TimEncoder::TimEncoder(const timEncoderCfg* const _cfg) : cfg(_cfg){

}

BASE_RESULT TimEncoder::reinit(uint32_t numberCfg){
	BASE_RESULT rv = BASE_RESULT::OK;

	if ( numberCfg >= this->cfg->countCfg )
			return BASE_RESULT::INPUT_VALUE_ERROR;

	clkTimInit( this->cfg->tim );

	tim.Instance 				= this->cfg[numberCfg].tim;
	tim.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV4; // вроде не влияет не на что
	tim.Init.CounterMode		= TIM_COUNTERMODE_UP;
	tim.Init.Period				= this->cfg[numberCfg].period;
	tim.Init.Prescaler			= 0; // не влияет не на что
	tim.Init.RepetitionCounter	= 0; // не влияет не на что

	TIM_Encoder_InitTypeDef sCfg;
	sCfg.EncoderMode = TIM_ENCODERMODE_TI12;
	sCfg.IC1Polarity = TIM_ICPOLARITY_RISING;
	sCfg.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sCfg.IC1Prescaler = TIM_ICPSC_DIV1;
	sCfg.IC1Filter = 0;
	sCfg.IC2Polarity = TIM_ICPOLARITY_RISING;
	sCfg.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sCfg.IC2Prescaler = TIM_ICPSC_DIV1;
	sCfg.IC2Filter = 0;

	if(HAL_TIM_Encoder_Init(&this->tim, &sCfg) != HAL_OK)
		rv = BASE_RESULT::INPUT_VALUE_ERROR;

	return rv;
}

BASE_RESULT TimEncoder::on(void){
	BASE_RESULT rv = BASE_RESULT::OK;

	if(HAL_TIM_Encoder_Start(&this->tim, TIM_CHANNEL_ALL) != HAL_OK)
		rv = BASE_RESULT::ERROR_INIT;

	return rv;
}

void TimEncoder::off(void){
	HAL_TIM_Encoder_Stop(&this->tim, TIM_CHANNEL_ALL);
}

uint32_t	TimEncoder::getEncoderCounts (void){
	return this->tim.Instance->CNT;
}



TimCapture::TimCapture( const timCaptureCfg* const cfg, const uint32_t countCfg ) :
		cfg( cfg ), countCfg( countCfg ) {

	this->tim.Instance						= this->cfg->tim;
}

bool TimCapture::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->countCfg ) return false;

	this->nowCfg = numberCfg;

	clkTimInit( this->tim.Instance );

	this->tim.Init.Prescaler			= this->cfg[ numberCfg ].prescaler;
	this->tim.Init.Period				= 0xFFFF;

	/// Получаем период одного тика.
	uint32_t fTimer = getTimBusFreq( this->tim.Instance ) * 2;
	this->tickPeriod = 1 / (	static_cast< float >( fTimer ) /
								static_cast< float >( this->cfg[ numberCfg ].prescaler + 1 ) );

	HAL_StatusTypeDef halResult;
	halResult = HAL_TIM_Base_Init( &this->tim );
	if ( halResult != HAL_StatusTypeDef::HAL_OK ) return false;

	TIM_ClockConfigTypeDef clockSourceCfg;
	clockSourceCfg.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	halResult = HAL_TIM_ConfigClockSource( &this->tim, &clockSourceCfg );
	if ( halResult != HAL_StatusTypeDef::HAL_OK ) return false;

	halResult = HAL_TIM_IC_Init( &this->tim );
	if ( halResult != HAL_StatusTypeDef::HAL_OK ) return false;

	TIM_MasterConfigTypeDef sMasterConfig;
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	halResult = HAL_TIMEx_MasterConfigSynchronization( &this->tim, &sMasterConfig );
	if ( halResult != HAL_StatusTypeDef::HAL_OK ) return false;

	const static TIM_IC_InitTypeDef channelCfg = {
			.ICPolarity				=	TIM_ICPOLARITY_FALLING,
			.ICSelection			=	TIM_ICSELECTION_DIRECTTI,
			.ICPrescaler			=	TIM_ICPSC_DIV1,
			.ICFilter				=	0xF
	};

	/// Настраиваем каналы захвата.
	for ( uint32_t channelCapture = 0; channelCapture < 4; channelCapture++ ) {
		HAL_TIM_IC_ConfigChannel( &this->tim, ( TIM_IC_InitTypeDef* )&channelCfg,
								  this->channelHalName[ channelCapture ] );
	}

	/*
	 * Обращение к флагам через Bit Banding.
	 */
	uint32_t channelCaptureBit = 1;
	for ( uint32_t channelCapture = 0; channelCapture < 4; channelCapture++ ) {
		this->captureFlag[ channelCapture ] = getBitWordAddr( ( void* )&this->tim.Instance->SR, channelCaptureBit );
		channelCaptureBit++;
	}

	// Для более удобной вычитки данных из регистров в прерывании.
	this->captureData[ 0 ] = ( uint32_t* )&this->tim.Instance->CCR1;
	this->captureData[ 1 ] = ( uint32_t* )&this->tim.Instance->CCR2;
	this->captureData[ 2 ] = ( uint32_t* )&this->tim.Instance->CCR3;
	this->captureData[ 3 ] = ( uint32_t* )&this->tim.Instance->CCR4;

	/// Флаг переполнения.
	this->updateFlag = getBitWordAddr( ( void* )&this->tim.Instance->SR, 0 );

	/// Начальная инициализация таймера.
	uint32_t channelCaptureMskEn = ( TIM_CCER_CC1E << TIM_CHANNEL_1 ) |
								   ( TIM_CCER_CC1E << TIM_CHANNEL_2 ) |
								   ( TIM_CCER_CC1E << TIM_CHANNEL_3 ) |
								   ( TIM_CCER_CC1E << TIM_CHANNEL_4 );

	this->tim.Instance->CCER |= channelCaptureMskEn;
	__HAL_TIM_MOE_ENABLE( &this->tim );

	/// Прерывания по захвату и переполнению.
	__HAL_TIM_ENABLE_IT( &this->tim, TIM_IT_UPDATE | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 );
}

bool TimCapture::on	( void ) {
	__HAL_TIM_ENABLE( &this->tim );
	return true;
}

bool TimCapture::off ( void ) {
	__HAL_TIM_DISABLE( &this->tim );
	return true;
}

bool TimCapture::getFrequency ( const uint32_t channel, float& returnFrequency ) {
	if ( channel > 3 ) return false;
	returnFrequency =  1 / ( this->channelControl[ channel ].periodTick * this->tickPeriod );
	return true;
}

bool TimCapture::getStateChannel  ( const uint32_t channel ) {
	if ( channel > 3 ) return false;

	/// Если канал не заблокирован.
	if ( this->channelControl[ channel ].flagLock == false ) {
		return true;
	}

	/// Канал заблокирован.
	if ( this->channelControl[ channel ].lockUpCount == 0 ) {
		/// Несмотря на то, что мы узнали, что канал
		/// теперь активен, мы вернем то, что он заблокирован,
		/// поскольку факт блокировки был и не был ранее вычитан.
		this->channelControl[ channel ].flagLock = false;
	}

	return false;
}

bool TimCapture::getFlagCaptureChannel  ( const uint32_t channel ) {
	bool buffer = this->channelControl[ channel ].updateFlag;
	this->channelControl[ channel ].updateFlag = false;
	return buffer;
}

void TimCapture::timHandler ( void ) {
	/*!
	 * Данный флаг выставляется в true если детектировано
	 * хотя бы одно поддерживаемое прерывание.
	 * Если вдруг по окончании проверок он будет false,
	 * тогда у нас не детектируемое прерывание и надо все нафиг сбросить (регистр SR)!
	 */
	bool flagInterrupt = false;

	/// Фиксируем переполнение.
	if ( *this->updateFlag != 0 ) {
		*this->updateFlag = 0;
		flagInterrupt = true;
		for ( uint32_t captureChannel = 0; captureChannel < 4; captureChannel++ ) {
			/// Если канал был отключен.
			if ( this->channelControl[ captureChannel ].lockUpCount >= 1 ) {
				this->channelControl[ captureChannel ].lockUpCount--;

				/// Если время пришло, то восстанавливаем канал.
				if ( this->channelControl[ captureChannel ].lockUpCount == 0 ) {
					TIM_CCxChannelCmd( this->tim.Instance, this->channelHalName[ captureChannel ], TIM_CCx_ENABLE );
				}
			} else {					/// Если канал работает штатно.
				/*!
				 * Если на канале все еще присутствует частота
				 * (количество переполнений не перевалило за
				 * максимально дозволенный, см. ниже).
				 */
				if ( this->channelControl[ captureChannel ].frequencyPresent == true ) {
					this->channelControl[ captureChannel ].countUpdate++;
				}

				/*!
				 * Возможна ситуация, когда частота на канале пропадет.
				 * Чтобы не получилось так, что мы считаем актуальной
				 * частоту с момента последнего обновления -
				 * вводим максимально возможный период между импульсами.
				 * Если мы его достигли, то считается что период
				 * бесконечность, а частота, соответственно 0.
				 */
				/// Мы достигли максимально возможного периода.
				if ( this->channelControl[ captureChannel ].countUpdate == this->cfg->maxCountUpTimerPeriod ) {
					/// Сообщаем, что частота на канале пропала.
					this->channelControl[ captureChannel ].frequencyPresent	=	false;

					/// Количество переполнений на этом канале теперь не актуально.
					this->channelControl[ captureChannel ].countUpdate		=	0;

					/// Делаем максимально возможный период, чтобы при пересчете в частоту у нас был 0.
					this->channelControl[ captureChannel ].periodTick		=	0xFFFFFFFF;
				}
			}


		}
	}

	/// Смотрим прерывание на всех 4-х каналах.
	for ( uint32_t captureChannel = 0; captureChannel < 4; captureChannel++ ) {
		if ( *this->captureFlag[ captureChannel ] == 0 ) continue;				/// Если прерывания не было.

		/// Фиксируем факт захвата на канале.
		flagInterrupt = true;

		/// Фиксируем событие захвата частоты на канале.
		this->channelControl[ captureChannel ].updateFlag = true;

		/*!
		 * Если до этого момента на канале
		 * присутствовала стабильная частота.
		 */
		if ( this->channelControl[ captureChannel ].frequencyPresent == true ) {
			/// Конечное становится начальным, все едино (с)
			this->channelControl[ captureChannel ].startPos = this->channelControl[ captureChannel ].stopPos;

			/// Забираем из таймера время завершения.
			this->channelControl[ captureChannel ].stopPos = *this->captureData[ captureChannel ];

			/// Пошел анализ частоты.

			/// Тот случай, когда переполнения не произошло и точки находятся
			/// рядом в положительном диапазоне где значение старта младше остановки.
			if ( this->channelControl[ captureChannel ].countUpdate == 0 ) {
				/// Защита от очень высокой частоты (помех).
				if (	this->channelControl[ captureChannel ].stopPos -
						this->channelControl[ captureChannel ].startPos <
						this->cfg[ nowCfg ].minTickValue ) {
					/// Отключаем захват с канала.
					TIM_CCxChannelCmd( this->tim.Instance, this->channelHalName[ captureChannel ], TIM_CCx_DISABLE );

					/// Указываем, на какой срок заблокировали.
					this->channelControl[ captureChannel ].lockUpCount = this->cfg[ nowCfg ].countUpForResetChannel;
					this->channelControl[ captureChannel ].flagLock = true;
					continue;
				}

				/// Получаем частоту.
				this->channelControl[ captureChannel ].periodTick = ( this->channelControl[ captureChannel ].stopPos -
																	  this->channelControl[ captureChannel ].startPos );
				continue;			/// С текущим каналом точно все.
			}

			/// Тот случай, когда начало перед переполнением, а конец за ним.
			if ( this->channelControl[ captureChannel ].countUpdate == 1 ) {
				this->channelControl[ captureChannel ].periodTick =
						( ( 0x10000 - this->channelControl[ captureChannel ].startPos ) +
						  this->channelControl[ captureChannel ].stopPos );
				this->channelControl[ captureChannel ].countUpdate = 0;
				continue;			/// С текущим каналом точно все.
			}

			/// Точно знаем, что у нас широкий импульс на минимум 1 переполнение между.
			this->channelControl[ captureChannel ].periodTick =
					( ( 0x10000 - this->channelControl[ captureChannel ].startPos ) +
					  this->channelControl[ captureChannel ].stopPos +
					  ( this->channelControl[ captureChannel ].countUpdate - 1 ) * 0x10000 );
			this->channelControl[ captureChannel ].countUpdate = 0;
			continue;				/// С текущим каналом точно все.
		} else {
			/*!
			 * Если мы тут, то у нас еще не было захваченной
			 * частоты и приходит первый импульс.
			 * По одному импульсу частоту не получить и
			 * просто фиксируем его. А частота по прежнему будет 0.
			 */
			this->channelControl[ captureChannel ].stopPos = *this->captureData[ captureChannel ];

			/*!
			 * Со следующего импульса у нас будет уже частота.
			 * Или снова очередное переполнение, которое
			 * отловит обработчик переполнений.
			 */
			this->channelControl[ captureChannel ].frequencyPresent = true;
		}
	}

	/// Защита от внештатных ситуаций.
	if ( !flagInterrupt ) {
		this->tim.Instance->SR = 0;
	}
}

