#ifndef _FILTER_H_
#define	_FILTER_H_

#include "sys.h"

typedef struct{
	bool IsCountFilter;
	bool IsTimeFilter;
	bool IsTimerReseted;
	bool rsv;
	uint32_t CountFilterThread;
	uint64_t TimeFilterThread;
	uint32_t CountFilterValue;
	uint64_t TimeFilterValue;
}FilterItem_T, *pFilterItem_T;

extern void Filter_Init(pFilterItem_T pfilter,int count,bool isCount,bool isTime,uint32_t CountFilter,uint32_t TimeFilter);
extern bool ChkFilter(pFilterItem_T pFilter, uint64_t Tick, bool b);
extern void ResetFilterCount(pFilterItem_T pFilter);
extern void ResetFilterTimer(pFilterItem_T pFilter);

#endif
