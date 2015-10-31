#include "logicHeader.h"

CIntervalSchedule::CIntervalSchedule()
{
  _Cumtime = 0;
}

bool CIntervalSchedule02::Schedule(float Delta)
{
  _Cumtime += Delta;
  if (_Cumtime>=IntervalTime)
  {
    _Cumtime = 0;
    return true;
  }
  return false;
}

bool CIntervalSchedule250::Schedule( float Delta )
{
  return false;
}
