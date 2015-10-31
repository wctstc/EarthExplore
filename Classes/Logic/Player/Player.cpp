#include "../Main/logicHeader.h"


CPlayerData::CPlayerData()
{
  PreInitData();
}

void CPlayerData::PreInitData()
{
  _Face = EFT_DOWN;
  _Speed = 3;
  _FallSpeed = 3;
  _BulletNum = 600;
  _Score = 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

CPlayerControll::CPlayerControll()
{

}

void CPlayerControll::PushItem(ItemRecord Item)
{
  int size = _Items.size();
  for (int i = 0; i < size; i++)
  {
    if (_Items[i].ItemId == Item.ItemId)
    {
      _Items[i].ItemNum += Item.ItemNum;
      return;
    }
  }
  _Items.push_back(Item);
}

void CPlayerControll::PushItem(int Id)
{
  int size = _Items.size();
  for (int i = 0; i < size; i++)
  {
    if (_Items[i].ItemId == Id)
    {
      _Items[i].ItemNum ++;
      return;
    }
  }
  _Items.push_back(ItemRecord(Id, 1));
}

void CPlayerControll::DeleteItem(int idx)
{
  vector <ItemRecord>::iterator It;
  int Index = 0;
  for (It=_Items.begin();It!=_Items.end();It++)
  {
    if (Index == idx)
    {
      _Items.erase(It);
      It=_Items.begin();
    }
    Index++;
  }
}

void CPlayerControll::RealseItem(int Id, int Num)
{
  int size = _Items.size();
  for (int i = 0; i < size; i++)
  {
    if (_Items[i].ItemId == Id)
    {
      _Items[i].ItemNum -= Num;
      if (_Items[i].ItemNum<=0)
        DeleteItem(i);
      return;
    }
  }
}

void CPlayerControll::UseItem( int idx )
{
  int size = _Items.size();
  if (idx>=0&&idx<size&&_Items[idx].ItemNum)
  {
    ItemRecord Record = _Items[idx];
    RealseItem(_Items[idx].ItemId);
    ItemProperty* Property = DataMgr->GetItemProperty(Record.ItemId);
    if (!Property)
      return;
    switch (Property->RangeType)
    {
    case EIDT_RANGE:
      UseRangeItem(_Items[idx].ItemId);
      break;
    case EIDT_LINEVERTICAL:
      UseLineVerticalItem(_Items[idx].ItemId);
      break;
    case EIDT_DELIVERY:
      break;
    default:
      break;
    }
  }
}

void CPlayerControll::UseRangeItem( int Id )
{
  ItemProperty* Property = DataMgr->GetItemProperty(Id);
  if (!Property)
    return;
  int Range = Property->ElimitNum;
  for (int  j = GameMgr->_Hero->_CoordY+Range; j >= GameMgr->_Hero->_CoordY-Range; j--)
  {
    if (j < 0 || j > MatrixYNum-1)
      continue;
    for (int  i = GameMgr->_Hero->_CoordX-Range; i <= GameMgr->_Hero->_CoordX+Range; i++)
    {
      if (i < 0 || i > MatrixXNum)
        continue;
      GameMgr->_StarMatrix->PushSelectStar(i, j);
    }
  }
  GameMgr->_StarMatrix->OperatorSelectStar();
}

void CPlayerControll::UseLineVerticalItem( int Id )
{
  ItemProperty* Property = DataMgr->GetItemProperty(Id);
  if (!Property)
    return;
  int Range = Property->ElimitNum;
  for (int i = GameMgr->_Hero->_CoordY+1; i >= GameMgr->_Hero->_CoordY-Range; i--)
  {
    if (i < 0 || i > MatrixYNum-1)
      continue;
    GameMgr->_StarMatrix->PushSelectStar(GameMgr->_Hero->_CoordX, i);
  }
  GameMgr->_StarMatrix->OperatorSelectStar();
}

void CPlayerControll::UseDeliveryItem( int Id )
{
  ItemProperty* Property = DataMgr->GetItemProperty(Id);
  if (!Property)
    return;
  int Range = Property->ElimitNum;
}

void CPlayerControll::SetScore( int Score )
{
  _PlayerData._Score += Score;
}

int CPlayerControll::GetSocre()
{
  return _PlayerData._Score;
}

void CPlayerControll::MoveLeft()
{

}

void CPlayerControll::MoveUp()
{

}

void CPlayerControll::MoveRight()
{

}

void CPlayerControll::MoveDown()
{

}

void CPlayerControll::Attack()
{

}

void CPlayerControll::GetBlockPosition()
{
  _BlockX = 0;
  _BlockY = 0;
}

void CPlayerControll::ClearData()
{
  _BlockX = 10;
  _BlockY = 5;
  _Level = 1;
}
