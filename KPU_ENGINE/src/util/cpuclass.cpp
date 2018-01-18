///////////////////////////////////////////////////////////////////////////////
// Filename: cpuclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include"kpu.h"
#include "cpuclass.h"


CpuClass::CpuClass()
{
}


CpuClass::CpuClass(const CpuClass& other)
{
}


CpuClass::~CpuClass()
{
}


void CpuClass::Initialize()
{
	PDH_STATUS status;


	// Initialize the flag indicating whether this object can read the system cpu usage or not.
	m_canReadCpu = true;

	// Create a query object to poll cpu usage.
	status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if(status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	// 생성된 질의 개체는 시스템의 모든 cpu들의 사용량 총합을 합하여 돌려줍니다.
	//만약 핸들이나 질의를 얻지 못하는 들의 이유로 cpu사용량 정보를 읽을 수 없다면 m_canReadCpu 변수를 False로 바꿔서 사용량을 0으로 고정합니다.
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if(status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	m_lastSampleTime = GetTickCount(); 

	m_cpuUsage = 0;

	return;
}


void CpuClass::Shutdown()
{
	if(m_canReadCpu)
	{
		PdhCloseQuery(m_queryHandle);
	}

	return;
}


void CpuClass::Frame()
{
	PDH_FMT_COUNTERVALUE value; 

	if(m_canReadCpu)
	{
		if((m_lastSampleTime + 1000) < GetTickCount())//1초마다 질의
		{
			m_lastSampleTime = GetTickCount(); 

			PdhCollectQueryData(m_queryHandle);
        
			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_cpuUsage = value.longValue;
		}
	}

	return;
}


int CpuClass::GetCpuPercentage()
{
	int usage;

	if(m_canReadCpu)
	{
		usage = (int)m_cpuUsage;
	}
	else
	{
		usage = 0;
	}

	return usage;
}