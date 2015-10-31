#pragma once

#include "cocos2d.h"

USING_NS_CC;
class GlobalSchedule: public Node {
public:
  static void startSchedule();
  // 开始全局定时器 fInterval: 时间间隔 ; fDelay: 延迟运行
  static void startSchedule(float fInterval = 0.0f, float fDelay = 0.0f);
  // 停止全局定时器
  static void stopSchedule();
  // 全局定时器暂停
  static void pauseSchedule();
  // 全局定时器暂停恢复
  static void resumeSchedule();
  //全局定时器
  void globalUpdate(float delta) ;
private:
  // 构造函数私有化，只能通过 start 来启用全局定时器
  GlobalSchedule(float fInterval, float fDelay);
  ~GlobalSchedule();
  // 声明静态变量保持单例
  static GlobalSchedule* m_pSchedule;
};
