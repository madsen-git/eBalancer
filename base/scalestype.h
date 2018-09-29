/*****************************************************************************************
* ��  Ŀ��ͨ��
* ˵  ����
*	
*	
* ��  �ڣ�2016-12-15				
* ��  �ߣ�madsen	
*****************************************************************************************/
#ifndef __BASETYPE_H__
#define __BASETYPE_H__
//#pragma once


// ������Ʒ�
#define CMD_CONTROL_END_A			0x0D			// ���ݽ�������λ
#define CMD_CONTROL_END_B			0x0A			//			��λ


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������� ��ʽ������(2B)+����ֵ(4B)+β��(2B \r\n)
#define CMD_MASTER_START_A			0x10			// ��������
#define CMD_MASTER_START_B			0x01			// 
#define CMD_MASTER_STOP_A			0x10			// ֹͣ����
#define CMD_MASTER_STOP_B			0x02			// 

#define CMD_SCALE_INIT_A			0x20			// ��ʼ���豸
#define CMD_SCALE_INIT_B			0x01			// 
#define CMD_SCALE_ONE_A				0x20			// ��һ�β���ֵ
#define CMD_SCALE_ONE_B				0x02			// 
#define CMD_SCALE_EMPTY_A			0x20			// ���ز���
#define CMD_SCALE_EMPTY_B			0x04			// 
#define CMD_SCALE_FULL_A			0x20			// ���ز���
#define CMD_SCALE_FULL_B			0x08			// 

#define CMD_MOTOR_START_A			0x30			// �������
#define CMD_MOTOR_START_B			0x01			// 
#define CMD_MOTOR_STOP_A			0x30			// ֹͣ���
#define CMD_MOTOR_STOP_B			0x02			// 
#define CMD_MOTOR_READY_A			0x30			// �����λ��������׽�����ж�
#define CMD_MOTOR_READY_B			0x04			// �����λ

#define CMD_POWER_START_A			0x40			// ������
#define CMD_POWER_START_B			0x01			// 

// ���͸��û�����Ӧ����
#define CMD_RSP_FLAG_A				0x80
#define CMD_RSP_FLAG_B				0x80
#define CMD_RSP_MASTER_START_A		(CMD_RSP_FLAG_A | CMD_MASTER_START_A)	// 0x90��������
#define CMD_RSP_MASTER_START_B		(CMD_RSP_FLAG_B | CMD_MASTER_START_B)	// 0x81
#define CMD_RSP_MASTER_STOP_A		(CMD_RSP_FLAG_A | CMD_MASTER_STOP_A)	// 0x90ֹͣ����
#define CMD_RSP_MASTER_STOP_B		(CMD_RSP_FLAG_B | CMD_MASTER_STOP_B)	// 0x82
#define CMD_RSP_MOTOR_STOP_A		(CMD_RSP_FLAG_A | CMD_MOTOR_STOP_A)		// ������� �����λ
#define CMD_RSP_MOTOR_STOP_B		(CMD_RSP_FLAG_B | CMD_MOTOR_STOP_B)
#define CMD_RSP_SCALE_INIT_A		(CMD_RSP_FLAG_A | CMD_SCALE_INIT_A)
#define CMD_RSP_SCALE_INIT_B		(CMD_RSP_FLAG_B | CMD_SCALE_INIT_B)
#define CMD_RSP_SCALE_ONE_A			(CMD_RSP_FLAG_A | CMD_SCALE_ONE_A)
#define CMD_RSP_SCALE_ONE_B			(CMD_RSP_FLAG_B | CMD_SCALE_ONE_B)
#define CMD_RSP_SCALE_EMPTY_A		(CMD_RSP_FLAG_A | CMD_SCALE_EMPTY_A)
#define CMD_RSP_SCALE_EMPTY_B		(CMD_RSP_FLAG_B | CMD_SCALE_EMPTY_B)
#define CMD_RSP_SCALE_FULL_A		(CMD_RSP_FLAG_A | CMD_SCALE_FULL_A)
#define CMD_RSP_SCALE_FULL_B		(CMD_RSP_FLAG_B | CMD_SCALE_FULL_B)
#define CMD_RSP_SCALE_ITEM_A		0xF0			// ���ؽ������
#define CMD_RSP_SCALE_ITEM_B		0xF0
#define CMD_RSP_SCALE_STATE_A		0xF1			// ����״̬����
#define CMD_RSP_SCALE_STATE_B		0xF1

/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////////////
// 2016-3-7		xutong	���ӳӽӿڣ���ʼ��ʱ���������
typedef struct
{
	int8_t		addressWeigher;			// ����ģ���ַ
	int8_t		addressSCR;				// �������ַ
	int8_t		upChannel;				// ��ͨ���˿�
	int8_t		downChannel;			// ��ͨ���˿�
}BSScalerBusface;

typedef struct 
{
	BSScalerBusface	busface;

	int32_t		logicLow;				// ��ѹ�߼�ֵ���磺50�˻�0��
	int32_t		logicHigh;				// ��ѹ�߼�ֵ���磺1000��
	int32_t		msvLow;					// ��ѹʵ��ֵ
	int32_t		msvHigh;				// ��ѹʵ��ֵ

}BSScaler;


//////////////////////////////////////////////////////////////////////////////////////////
// 2016-3-7		xutong	����ʱ��ͨ���������
typedef struct {
	int16_t		feederUpPower0;			// ��ʼ�������ֵ
	int16_t		feederUpPer1;			// �ϸ�������Ŀ���ذٷֱ�ֵ1
	int16_t		feederUpPower1;			// ���� feederUpPer1 ʱ����Ĺ���ֵ
	int16_t		feederUpPer2;			// �ϸ�������Ŀ���ذٷֱ�ֵ2
	int16_t		feederUpPower2;			// ���� feederUpPer2 ʱ����Ĺ���ֵ

	int16_t		feederDownPower0;		// ��ʼ�������ֵ
	int16_t		feederDownPer1;			// �ϸ�������Ŀ���ذٷֱ�ֵ1
	int16_t		feederDownPower1;		// ���� feederDownPer1 ʱ����Ĺ���ֵ
	int16_t		feederDownPer2;			// �ϸ�������Ŀ���ذٷֱ�ֵ2
	int16_t		feederDownPower2;		// ���� feederDownPer2 ʱ����Ĺ���ֵ
}BSPowerConfig;

//////////////////////////////////////////////////////////////////////////////////////////
// 2016-3-10		xutong	�����Ϣ
typedef struct {
	// Ŀ����ء��磺123.25
	int16_t		weightPackInteger;		// Ŀ����ء��߼�ֵ�������� 123
	int16_t		weightPackDecimal;		// Ŀ����ء��߼�ֵС������ 250	����1000��
										// Ŀ����ص���Χ���磺0.5
	int16_t		marginErrorInteger;		// Ŀ����ص���Χ���߼�ֵ�������� 0
	int16_t		marginErrorDecimal;		// Ŀ����ص���Χ���߼�ֵС������ 500	����1000��
	int16_t		packCount;				// ���������
	int16_t		realCount;				// �ϸ�Ĵ������
	int16_t		failedCount;			// �����İ�����

}BSPack;


//////////////////////////////////////////////////////////////////////////////////////////
// 2016-3-7		xutong	��ʼ����ʱ���صĲ���
typedef struct 
{
	BSPack			pack;
	BSPowerConfig	power;
	BSScaler		scaler;

}BSScaleWork;

//////////////////////////////////////////////////////////////////////////////////////////
// 2016-3-12		xutong	���͸��û��ĳ�������
typedef struct 
{
	int32_t		weightInteger;			// ���ء��߼�ֵ�������� 123
	int32_t		weightDecimal;			// ���ء��߼�ֵС������ 250	����1000��
	uint8_t		bOk;					// �Ƿ�ϸ�

}BSScaleItem;
///////////////////////////////////////////////


#pragma pack(pop)





#endif // __BASETYPE_H__
