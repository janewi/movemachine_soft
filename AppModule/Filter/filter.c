#include "filter.h"



/*****************************************************************************
 �� �� ��  : Filter_Init
 ��������  : IO�����������ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 �޸���ʷ      :
  1.��    ��   : 2013��2��19��
    ��    ��   : ����
    �޸�����   : �����ɺ���
*****************************************************************************/
void Filter_Init(pFilterItem_T pfilter,int count,bool isCount,bool isTime,uint32_t CountFilter,uint32_t TimeFilter)
{
	uint32_t i;

	//��������ʼ��
	for(i=0; i< count; i++){
		pfilter->IsCountFilter= isCount;
		pfilter->IsTimeFilter= isTime;

		pfilter->CountFilterThread = CountFilter;

		pfilter->TimeFilterThread = TimeFilter;
		ResetFilterCount(pfilter);
	}
}

bool ChkFilter(pFilterItem_T pFilter, uint64_t Tick, bool b)
{
	if(pFilter->IsCountFilter){
		if(b){
			if(pFilter->CountFilterValue == 0){
				return TRUE;
			}else{
				pFilter->CountFilterValue--;
			}
		}else{
			pFilter->CountFilterValue = pFilter->CountFilterThread;
		}
		return FALSE;
	}

	if(pFilter->IsTimeFilter){
		if(b){		
			if(pFilter->IsTimerReseted){
				pFilter->IsTimerReseted = FALSE;
				pFilter->TimeFilterValue = Tick;
			}
			if((Tick-pFilter->TimeFilterValue)>pFilter->TimeFilterThread){
				return TRUE;
			}
		}else{
			pFilter->TimeFilterValue = Tick;
		}
	}
	return FALSE;
}

void ResetFilterCount(pFilterItem_T pFilter)
{
	pFilter->CountFilterValue = pFilter->CountFilterThread;
}

void ResetFilterTimer(pFilterItem_T pFilter)
{
	pFilter->IsTimerReseted = TRUE;
}

