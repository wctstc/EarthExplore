#pragma once
#include "GeneralKit.h"
#include <vector>
#include <string>
#include "cocos2d.h"

USING_NS_CC;

class CPlayerData : SingleTon<CPlayerData>
{
public:
  int _Face;                                //面向
  int _Speed;                               //移动速度
  int _FallSpeed;                           //下降速度
  ItemProperty _Weapon;                     //武器
  int _BulletNum;                           //子弹数量
  int _Score;                               //得分
public:
  CPlayerData();
  void PreInitData();
};


#define _AUTOGETSET(T, V) inline void Set##V(T set){_PlayerData.##_V = set;} \
  inline T Get##V(){return _PlayerData.##_V;}\

class CPlayerControll : public SingleTon<CPlayerControll>
{
public:
  class CPlayerSprite* _Hero;                     //英雄精灵
  CPlayerData _PlayerData;                        //玩家数据
  int _BlockX;
  int _BlockY;
  int _Level;
  std::vector<struct ItemRecord> _Items;          //道具
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