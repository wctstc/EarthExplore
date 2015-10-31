#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

class GParse
{
public:
  GParse(char* Str);
  int getInt();
  string getString();
private:
  vector<string> _StrtokStr;
  int _Index;
};

class CItemMgr
{
public:
  bool LoadConfig();
public:
  map<int, struct ItemProperty> Items;
};

class CTestLuaMgr
{
public:
  bool LoadConfig();
};

struct ScoreTable
{
  int Num;
  int Score;
};

class CEliminateScore
{
public:
  bool LoadConfig();
  int GetSocre(int Num);
private:
  std::vector<ScoreTable> Scores;
};

class CDefaultItem
{
public:
  bool LoadConfig();
};

class CGlobleData
{
public:
  bool LoadConfig();
  int getValue(string key);
private:
  map<string, int> GlobleValue;
};

struct BlockMap
{
  int Idx;
  int Id;
  BlockMap()
  {
    Idx = Id = 0;
  }
  BlockMap(int v1, int v2)
  {
    Idx = v1;
    Id = v2;
  }
};

struct PropertySet
{
  int Idx;
  int Id;
};

class CBlockMatrix
{
public:
  bool LoadConfig();
  int GetId(int Idx);
public:
  vector<BlockMap> BlockMaps;
  vector<PropertySet> PropertySets;
};

class CPropertySet
{
public:
  bool LoadConfig();
  int GetItemId(int Idx);
  int GetPropertySetNum();
public:
  vector<PropertySet> PropertySets;
};

struct ShopData
{
  int Id;
  int money;
};

class CShop
{
public:
  bool LoadConfig();
  const TArray<ShopData>& GetShopItemData();
private:
  TArray<ShopData> _ShopDatas;
};

class CGameString
{
public:
  bool LoadConfig();
  const std::string& getString(int idx);
public:
  std::vector<string> GameStrings;
};

class CDataManager
{
public:
  CItemMgr        ItemMgr;
  CEliminateScore EliminateScore;
  CDefaultItem    DefaultItem;
  CGameString     GameString;
  CGlobleData     GlobleData;
  CBlockMatrix    BlockMatrix;
  CPropertySet    ProperSet;
  CShop           Shop;
public:
  CDataManager();
  static CDataManager* GetInstance();
  bool LoadConfig();
  ItemProperty* GetItemProperty(int Id);
  const char* GetGameString(int Id);
private:
  //static CDataManager* m_Instance;
};