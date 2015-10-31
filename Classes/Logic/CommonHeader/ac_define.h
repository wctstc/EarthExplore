#pragma once
#include <string>
using namespace std;

const int nullvalue = -1;

const int MatrixXNum = 10;
const int MatrixYNum = 11;

const int NULL_MAP = -1;

const int MOVESPEED = 3;
const int StrLen = 128;

const int StartX = 14;
const int StartY = 60;

const int TotleNum = 5;

const float IntervalTime = 0.05f;

enum ETICKSTATE
{
  ETST_NONE = 0,
  ETST_REMOVESELECT,
  ETST_MAX,
};

enum EMAPTYPE
{
  EMT_NONE = 0,
  EMT_YELLOW,
  EMT_RED,
  EMT_GREEN,
  EMT_PURPLE,
  EMT_BLUE,
  EMT_MAX,
};

enum EFaceType
{
  EFT_NONE = 0,
  EFT_UP ,
  EFT_LEFT,
  EFT_DOWN,
  EFT_RIGHT,
  EFT_MAX,
};

enum EItemType
{
  EIT_NONE = 0,
  EIT_BLOCK,
  EIT_PROPERTY,
  EIT_MAX,
};

enum EItemDetailType
{
  EIDT_NONE = 0,                     //NULL
  EIDT_RANGE,                        //·¶Î§
  EIDT_LINEVERTICAL,                 //ÊúÏß
  EIDT_DELIVERY,                     //´«ËÍ
  EIDT_MAX,
};

const int StarSpriteSizeX = 44;
const int StarSpriteSizeY = 44;
const int OffSetX = 1;
const int OffSetY = 1;
const int HeroStartX = 5;
const int HeroStartY = 10;
const int PropNum = 5;

struct ItemProperty
{
  int Id;
  string Name;
  string Describe;
  string Pic;
  int Type;
  bool AutoDown;
  int ElimitNum;
  int RangeType;
  int DetailType;
  int Life;
};

struct Coord2D
{
  int x;
  int y;
};

struct Coord3D
{
  int x;
  int y;
  int z;
};

struct ItemRecord
{
  int ItemId;
  int ItemNum;
  ItemRecord()
  {
    ItemRecord(0, 0);
  }
  ItemRecord(int Id, int Num)
  {
    ItemId = Id;
    ItemNum = Num;
  }
};