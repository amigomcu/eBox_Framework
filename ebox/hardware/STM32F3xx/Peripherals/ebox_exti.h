/**
  ******************************************************************************
  * @file    exti.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EXTI_H
#define __EXTI_H
#include "ebox_core.h"
#include "mcu.h"
#include "FunctionPointer.h"

/*
	1.�ṩһ��io�ж�
	*ע�⣺stm32һ���ж���EXTI_Linexֻ�����ӵ�һ��port��GPIO_Pin_x��������PA0Ϊ�ж�Դ֮��Ͳ�������PB0��PC0��Ϊ�ж�Դ
    trigger: ���Ŵ����е�������������һ������ģʽ�е�һ�֣�
        - RISING: �����ش����ж�
        - FALLING: �½��ش����ж�
        - CHANGE: �����غ��½��ؾ������ж�
 */
 
// ��������
enum TrigType
{
    FALL = 0,	// �½��ش���
    RISE,			// �����ش���
    FALL_RISING		// �������½���
};

enum ExtiType
{
    IT = 0,			// �ж�
    EVENT,			// �¼�
    IT_EVENT		// �ж�&�¼�
};

class Exti
{
public:
    Exti(Gpio *exti_pin);
    void begin(PIN_MODE mode = INPUT, ExtiType type = IT);
    void nvic(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
    void interrupt(FunctionalState enable);

    void attach(void (*fptr)(void));
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void))
    {
        _irq.attach(tptr, mptr);
    }

private:
    // ��̬��Ա��������Ĵ�������ʹ�������࣬��Ҳ���ڣ��ҿ������ⲿͨ����������: IRQ::irq_handler(0)
    // ����ʵ��������̬��Ա����̬��Ա���ܷ�����ͨ��Ա����Ҫͨ����������ӷ���
    static void _irq_handler(uint32_t pObj);
    // �ص�����ָ�����飬�ֱ���½��ػص��������ػص�����
    FunctionPointer _pirq[2];
    Gpio	*_pin;
    uint16_t	_extiLine;	//�ⲿ�ж�0-15

    void _init(ExtiType type = IT);


protected:
    FunctionPointer _irq;
};
#ifdef __cplusplus
extern "C" {
#endif


typedef void (*exti_irq_handler)(uint32_t id);

void exti_irq_init(uint8_t index, exti_irq_handler handler, uint32_t id);

#ifdef __cplusplus
}
#endif

#endif