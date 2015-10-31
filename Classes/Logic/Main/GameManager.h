#pragma once


class CStarMatrix;
class CStarSprite;
class MainScene;
class CPlayerSprite;

class CGameManger
{
public:
  int _Level;                   //µ±Ç°¹Ø¿¨
  int _GameState;
  MainScene* _MainScene;
  CStarMatrix* _StarMatrix;
  CPlayerSprite* _Hero;
public:
  static CGameManger* GetInstance();
  void InitGameManager();
  void InitLuaState();
  void InitData();
  void Attack();
  void NewGame();
  void ShowEffect(int Type, cocos2d::Layer* Node, int ExData=0);
  void ClickCoord(int x, int y);
  bool  CanAttack(int x, int y);
  bool CheckValid(int x, int y);
  int GetDesCoordX(int start, int CoordY, int Des);
  CStarSprite* GetAttackSprite();
  float GetMatrixX();
  float GetMatrixY();
  int GetHeroUpdateY();
  bool CheckClearance();
  void NewLevel(int Level);
  void RefreshScore(int Num);
  void ShowComboEffect(int size, class Layer* layer);
  int GetPropNum();
  void showStarParticleEffect(int color,Point position,class Node* node);
  struct Color4F getColor4F(int color);
  void CheckSquare(int , int y);
  bool CheckPassLevel(int x, int y);
  void ToNextLevel();
public:
#include "../System/GameSystem.h"
private:
  CGameManger();
  static CGameManger* _Instance;
};