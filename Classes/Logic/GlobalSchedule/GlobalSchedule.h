#pragma once

#include "cocos2d.h"

USING_NS_CC;
class GlobalSchedule: public Node {
public:
  static void startSchedule();
  // ��ʼȫ�ֶ�ʱ�� fInterval: ʱ���� ; fDelay: �ӳ�����
  static void startSchedule(float fInterval = 0.0f, float fDelay = 0.0f);
  // ֹͣȫ�ֶ�ʱ��
  static void stopSchedule();
  // ȫ�ֶ�ʱ����ͣ
  static void pauseSchedule();
  // ȫ�ֶ�ʱ����ͣ�ָ�
  static void resumeSchedule();
  //ȫ�ֶ�ʱ��
  void globalUpdate(float delta) ;
private:
  // ���캯��˽�л���ֻ��ͨ�� start ������ȫ�ֶ�ʱ��
  GlobalSchedule(float fInterval, float fDelay);
  ~GlobalSchedule();
  // ������̬�������ֵ���
  static GlobalSchedule* m_pSchedule;
};
