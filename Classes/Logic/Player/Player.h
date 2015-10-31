#pragma once
#include "GeneralKit.h"
#include <vector>
#include <string>
#include "cocos2d.h"

USING_NS_CC;

class CPlayerData : SingleTon<CPlayerData>
{
public:
  int _Face;                                //����
  int _Speed;                               //�ƶ��ٶ�
  int _FallSpeed;                           //�½��ٶ�
  ItemProperty _Weapon;                     //����
  int _BulletNum;                           //�ӵ�����
  int _Score;                               //�÷�
public:
  CPlayerData();
  void PreInitData();
};


#define _AUTOGETSET(T, V) inline void Set##V(T set){_PlayerData.##_V = set;} \
  inline T Get##V(){return _PlayerData.##_V;}\

class CPlayerControll : public SingleTon<CPlayerControll>
{
public:
  class CPlayerSprite* _Hero;                     //Ӣ�۾���
  CPlayerData _PlayerData;                        //�������
  int _BlockX;
  int _BlockY;
  int _Level;
  std::vector<struct ItemRecord> _Items;          //����
public:
  CPlayerControll();
  //_AUTOGETSET(int, BulletNum);
public:
  void PushItem(struct ItemRecord Item);
  void PushItem(int Id);
  void UseItem(int idx);
  void RealseItem(int Id, int Num = 1);
  void DeleteItem(int idx);
  void SetScore(int Score);
  int GetSocre();
  void MoveLeft();
  void MoveUp();
  void MoveRight();
  void MoveDown();
  void Attack();
  void GetBlockPosition();
  void ClearData();
  void UseRangeItem(int Id);
  void UseLineVerticalItem(int id);
  void UseDeliveryItem(int Id);
};