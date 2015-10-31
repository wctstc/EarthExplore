#pragma once

class CIntervalSchedule
{
public:
  float _Cumtime;
  virtual bool Schedule(float Delta){return false;}
  CIntervalSchedule();
};

class CIntervalSchedule02 : public CIntervalSchedule,public SingleTon<CIntervalSchedule02>
{
public:
  virtual bool Schedule(float Delta);
};

class CIntervalSchedule250 : public CIntervalSchedule,public SingleTon<CIntervalSchedule02>
{
public:
  virtual bool Schedule(float Delta);
};