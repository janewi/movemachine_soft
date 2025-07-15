#include "filter.h"



/*****************************************************************************
 函 数 名  : Filter_Init
 功能描述  : IO软件过滤器初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 修改历史      :
  1.日    期   : 2013年2月19日
    作    者   : 刘洁
    修改内容   : 新生成函数
*****************************************************************************/
void Filter_Init(pFilterItem_T pfilter,int count,bool isCount,bool isTime,uint32_t CountFilter,uint32_t TimeFilter)
{
	uint32_t i;

	//过滤器初始化
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

