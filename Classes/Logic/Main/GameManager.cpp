#include "logicHeader.h"
#include "GUIHeader.h"
extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

//全局控制变量
lua_State* GLuaState;
CPlayerControll* GSelfCtl = CPlayerControll::GetInstance();

CGameManger* CGameManger::_Instance = NULL;
CGameManger* GameMgr =  CGameManger::GetInstance();
CGameManger::CGameManger()
{
  InitGameManager();
}

CGameManger* CGameManger::GetInstance()
{
  if(!_Instance)
    _Instance = new CGameManger;
  return _Instance;
}

void CGameManger::InitGameManager()
{
  InitLuaState();
  InitData();
}

void CGameManger::InitLuaState()
{
  GLuaState=lua_open();
  luaopen_base(GLuaState);
  luaopen_table(GLuaState);
  luaopen_string(GLuaState);
  luaopen_math(GLuaState);
}

void CGameManger::InitData()
{
  _Level = 1;
  _MainScene = 0;
  _StarMatrix = NULL;
  _MainScene = NULL;
  _Hero = NULL;
  _GameState = ETST_NONE;
}

void CGameManger::Attack()
{
  CStarSprite* AttackSprite = GetAttackSprite();
  if (!AttackSprite)
    return;
  _StarMatrix->Eliminate(AttackSprite->_CoordY, AttackSprite->_CoordX);
}

void CGameManger::NewGame()
{
  _Hero->NewGame();
  _StarMatrix->NewGame();
  _MainScene->RefreshLevel(_Level);
}

void CGameManger::ShowEffect( int Type, cocos2d::Layer* Node, int ExData/*=0*/ )
{

}

void CGameManger::ClickCoord( int x, int y )
{
  if (!_Hero)
    return;
  if (!CheckValid(x, y))
    return ;
  //直接消除
  if (CanAttack(x, y))
  {
    GSelfCtl->_PlayerData._BulletNum--;
    _MainScene->RefreshBullet(GSelfCtl->_PlayerData._BulletNum);
    _StarMatrix->EliminateItem(x, y);
    //_StarMatrix->Eliminate(x, y);
    //_Hero->UpdatePositionY(EFT_DOWN, GetHeroUpdateY());
  }
  else if (y != _Hero->_CoordY)
    return;
  else
  {
    int DesX = GetDesCoordX(_Hero->_CoordX, _Hero->_CoordY, x);
    if (DesX>=0&&DesX!=_Hero->_CoordX)
    {
      _Hero->UpdatePositionX(x>_Hero->_CoordX?EFT_RIGHT:EFT_LEFT, DesX);
    }
  }
}

bool CGameManger::CanAttack( int x, int y )
{
  if (!_Hero)
    return false;
  CStarSprite* Star = _StarMatrix->m_Maps[y][x];
  if (!Star)
    return false;
  ItemProperty* Property = DataMgr->GetItemProperty(Star->_ItemId);
  if (Property&&Property->Type == EIT_BLOCK)
  {
    if (y==_Hero->_CoordY)
      return abs(x-_Hero->_CoordX)==1&&_StarMatrix->m_Maps[y][x];
    if (x==_Hero->_CoordX)
      return abs(y-_Hero->_CoordY)==1&&_StarMatrix->m_Maps[y][x];
  }
  return false;
}

int CGameManger::GetDesCoordX(int start, int CoordY, int Des)
{
  if (!_StarMatrix)
    return -1;
  if (start > Des)
  {
    for (int i = start-1; i >= Des; i--)
    {
      if (_StarMatrix->m_Maps[CoordY][i])
      {
        ItemProperty* Property = DataMgr->GetItemProperty(_StarMatrix->m_Maps[CoordY][i]->_ItemId);
        if (Property&&Property->Type == EIT_BLOCK)
          return i+1;
      }
      if (CoordY-1>=0&&!_StarMatrix->m_Maps[CoordY-1][i])
        return i;
      if (i == Des)
        return i;
    }
  }
  else if (start  < Des)
  {
    for (int i = start+1; i <= Des; i++)
    {
      if (_StarMatrix->m_Maps[CoordY][i])
      {
        ItemProperty* Property = DataMgr->GetItemProperty(_StarMatrix->m_Maps[CoordY][i]->_ItemId);
        if (Property&&Property->Type == EIT_BLOCK)
          return i-1;
      }
      if (CoordY-1>=0&&!_StarMatrix->m_Maps[CoordY-1][i])
        return i;
      if (i == Des)
        return i;
    }
  }
  return -1;
}

bool CGameManger::CheckValid( int x, int y )
{
  return x>=0&&x<MatrixXNum&&y>=0&&y<MatrixYNum;
}

CStarSprite* CGameManger::GetAttackSprite()
{
  int Face = GSelfCtl->_PlayerData._Face;
  int DesX = GSelfCtl->_BlockX;
  int DesY = GSelfCtl->_BlockY;
  switch(Face)
  {
  case EFT_UP:
    DesX++;
    break;
  case EFT_LEFT:
    DesY--;
    break;
  case EFT_DOWN:
    DesX--;
    break;
  case EFT_RIGHT:
    DesY++;
    break;
  }
  int Index = DesX*MatrixXNum+DesY;
  return 0;
}

float CGameManger::GetMatrixX()
{
  return _StarMatrix?_StarMatrix->getPositionX():0;
}

float CGameManger::GetMatrixY()
{
  return _StarMatrix?_StarMatrix->getPositionY():0;
}

int CGameManger::GetHeroUpdateY()
{
  int Block = 0;
  for (int i = _Hero->_CoordY-1; i >= 0; i--)
  {
    if (!_StarMatrix->m_Maps[i][_Hero->_CoordX])
      Block++;
  }
  if (Block)
    _Hero->_bUpdatePos = true;
  for (int i = _Hero->_CoordY-1; i >= 0; i--)
  {
    CStarSprite* Star = _StarMatrix->m_Maps[i][_Hero->_CoordX];
    if (!Star)
      continue;
    ItemProperty* Property = DataMgr->GetItemProperty(Star->_ItemId);
    if (!Property)
      continue;
    if (Property->Type == EIT_PROPERTY)
      Block++;
    if (Property->Type == EIT_BLOCK)
      break;
  }
  return _Hero->_CoordY-Block;
}

bool CGameManger::CheckClearance()
{
  bool Clearance = _Hero->_CoordY == 0;
  if (Clearance)
  {
    NewLevel(_Level+1);
  }
  return Clearance;
}

void CGameManger::NewLevel( int Level )
{
  _Level = Level;
  NewGame();
}

void CGameManger::RefreshScore( int Num )
{
  int Score = DataMgr->EliminateScore.GetSocre(Num);
  GSelfCtl->SetScore(Score);
  _MainScene->RefreshScore(0);
}

void CGameManger::ShowComboEffect( int size, class Layer* layer )
{
  if(size<2)
    return;
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Sprite* comboSprite = NULL;
  if (size >=8 )
  {
    comboSprite = Sprite::create("combo_fantastic.png");
  }
  else if(size>=6){
    comboSprite = Sprite::create("combo_awesome.png");
  }else if(size>=4){
    comboSprite = Sprite::create("combo_cool.png");
  }else{
    comboSprite = Sprite::create("combo_good.png");
  }
  if (!comboSprite)
    return;
  comboSprite->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
  layer->addChild(comboSprite);

  Blink* blink = Blink::create(1.0f,5);
  CallFunc* remove = CallFunc::create([=](){comboSprite->removeFromParentAndCleanup(true);});
  Sequence* action = Sequence::create(blink,remove,nullptr);
  comboSprite->runAction(action);
}

int CGameManger::GetPropNum()
{
  float Rate = (_Level+10)/10;
  return PropNum/Rate;
}


void CGameManger::showStarParticleEffect(int color,Point position,class Node* node)
{
  ParticleExplosion* effect = ParticleExplosion::create();
  effect->setTexture(Director::getInstance()->getTextureCache()->addImage("star.png"));
  //effect->setTotalParticles(35);
  effect->setStartColor(getColor4F(color));
  effect->setStartColorVar(Color4F(0,0,0,1));
  effect->setEndColor(getColor4F(color));
  effect->setEndColorVar(Color4F(0,0,0,1));
  effect->setStartSize(25.0f);
  effect->setGravity(Point(0,-300));
  effect->setLife(0.05f);
  effect->setSpeed(200);
  effect->setSpeedVar(10);
  effect->setPosition(position);
  node->addChild(effect);
}

struct Color4F CGameManger::getColor4F( int color )
{
  switch(color){
  case EMT_PURPLE:
    return Color4F(0.74f,0.30f,0.99f,1);
    //return Color4F(189,78,253,1);
  case EMT_BLUE:
    return Color4F(84/255.0f,203/255.0f,254/255.0f,1);
  case EMT_RED:
    return Color4F(253/255.0f,80/255.0f,126/255.0f,1);
  case EMT_YELLOW:
    return Color4F(253/255.0f,234/255.0f,84/255.0f,1);
  case EMT_GREEN:
    return Color4F(132/255.0f,226/255.0f,111/255.0f,1);
  }
  return Color4F(1.f,1.f,1.f,0);
}

void CGameManger::CheckSquare( int x, int y )
{
  CStarSprite* Star = _StarMatrix->m_Maps[y][x];
  if (!Star)
    return;
  ItemProperty* Property = DataMgr->GetItemProperty(Star->_ItemId);
  if (!Property)
    return;
  if (Property->Type == EIT_PROPERTY)   //道具拾取
  {
    _StarMatrix->RemoveStar(x, y);
    GSelfCtl->PushItem(Property->Id);
    _MainScene->RefreshListView();;
  }
}

//检查 通关
bool CGameManger::CheckPassLevel( int x, int y )
{
  return y < 0;
}

void CGameManger::ToNextLevel()
{

}
