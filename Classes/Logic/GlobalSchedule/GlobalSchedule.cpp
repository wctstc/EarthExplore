#include "logicHeader.h"

#define kCCRepeatForever (UINT_MAX -1)
#define SCHEDULE CCDirector::getInstance()->getScheduler()

GlobalSchedule* GlobalSchedule::m_pSchedule = NULL;
GlobalSchedule::GlobalSchedule(float fInterval, float fDelay) {
  SCHEDULE->schedule(schedule_selector(GlobalSchedule::globalUpdate),this,fInterval, kCCRepeatForever,fDelay,false);
}


GlobalSchedule::~GlobalSchedule() {
  SCHEDULE->unschedule(
    schedule_selector(GlobalSchedule::globalUpdate), this);
}

void GlobalSchedule::startSchedule(float fInterval, float fDelay) {
  m_pSchedule=new GlobalSchedule(fInterval, fDelay);
}

void GlobalSchedule::stopSchedule() {
  CC_SAFE_DELETE(m_pSchedule);
}

void GlobalSchedule::pauseSchedule() {
  SCHEDULE->pauseTarget(m_pSchedule);
}

void GlobalSchedule::resumeSchedule() {
  SCHEDULE->resumeTarget(m_pSchedule);
}

void GlobalSchedule::globalUpdate(float delta)
{
  if (GameMgr->_StarMatrix)
    GameMgr->_StarMatrix->Update(delta);
  if (GameMgr->_Hero)
    GameMgr->_Hero->Update(delta);
}