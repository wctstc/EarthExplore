#include "GUIHeader.h"
#include "logicHeader.h"

bool CBaseSprite::bHit( cocos2d::Vec2 TouchPoint )
{
  return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

GLProgramState* Effect::getGLProgramState() const
{
  return _glprogramstate;
}

void Effect::setTarget(class EffectSprite *sprite)
{
  return ;
}

bool Effect::initGLProgramState(const std::string &fragmentFilename)
{
  auto fileUtiles = FileUtils::getInstance();
  auto fragmentFullPath = fileUtiles->fullPathForFilename(fragmentFilename);
  auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
  auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
  _fragSource = fragSource;
#endif

  _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
  _glprogramstate->retain();

  return _glprogramstate != nullptr;
}

Effect::Effect()
  : _glprogramstate(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
  _backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
    [this](EventCustom*)
  {
    auto glProgram = _glprogramstate->getGLProgram();
    glProgram->reset();
    glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
    glProgram->link();
    glProgram->updateUniforms();
  }
  );
  Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
}

Effect::~Effect()
{
  CC_SAFE_RELEASE_NULL(_glprogramstate);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
  Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}

bool EffectOutline::init()
{
  initGLProgramState("Shaders/example_outline.fsh");

  Vec3 color(0.0f, 1.0f, 1.0f);
  GLfloat radius = 0.01f;
  GLfloat threshold = 1.75;

  _glprogramstate->setUniformVec3("u_outlineColor", color);
  _glprogramstate->setUniformFloat("u_radius", radius);
  _glprogramstate->setUniformFloat("u_threshold", threshold);
  return true;
}

EffectSprite* EffectSprite::create(const std::string& filename)
{
  auto ret = new (std::nothrow) EffectSprite;
  if(ret && ret->initWithFile(filename)) {
    ret->autorelease();
    return ret;
  }
  CC_SAFE_RELEASE(ret);
  return nullptr;
}

void EffectSprite::setEffect(Effect* effect)
{
  if(_defaultEffect != effect) 
  {
    effect->setTarget(this);

    CC_SAFE_RELEASE(_defaultEffect);
    _defaultEffect = effect;
    CC_SAFE_RETAIN(_defaultEffect);

    setGLProgramState(_defaultEffect->getGLProgramState());
  }
}

void EffectSprite::addEffect(Effect *effect, ssize_t order) 
{
  effect->retain();
  effect->setTarget(this);
  _effects.push_back(std::make_tuple(order,effect,QuadCommand()));
}

void EffectSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
#if CC_USE_CULLING
  // Don't do calculate the culling if the transform was not updated
  _insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
  if(_insideBounds)
  #endif
  {
    // negative effects: order < 0
    int idx=0;
    for(auto &effect : _effects) 
    {
      if(std::get<0>(effect) >=0)
        break;
      QuadCommand &q = std::get<2>(effect);
      q.init(_globalZOrder, _texture->getName(), std::get<1>(effect)->getGLProgramState(), _blendFunc, &_quad, 1, transform);
      renderer->addCommand(&q);
      idx++;
    }
    // normal effect: order == 0
    _quadCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, &_quad, 1, transform);
    renderer->addCommand(&_quadCommand);
//     // postive effects: oder >= 0
//     for(auto it = std::begin(_effects)+idx; it != std::end(_effects); ++it) 
//     {
//       QuadCommand &q = std::get<2>(*it);
//       q.init(_globalZOrder, _texture->getName(), std::get<1>(*it)->getGLProgramState(), _blendFunc, &_quad, 1, transform, flags);
//       renderer->addCommand(&q);
//       idx++;
//     }
  }
}

EffectSprite::~EffectSprite()
{
  for(auto &tuple : _effects) {
    std::get<1>(tuple)->release();
  }
  CC_SAFE_RELEASE(_defaultEffect);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CStarSprite* CStarSprite::create( const char* file )
{
  CStarSprite* StarSprite = new CStarSprite;
  if (StarSprite->initWithFile(file))
  {
    StarSprite->autorelease();
    return StarSprite;
  }
  CC_SAFE_DELETE(StarSprite);
  return nullptr;
}

void CStarSprite::UpdatePosition( int NullBlock )
{
  if (!NullBlock)
    return;
  _DesPos = Vec2(getPositionX(), (_CoordY-NullBlock)*(OffSetY+StarSpriteSizeY));
  _bUpdatePos = true;
}

void CStarSprite::Update( float time )
{
  if (!_bUpdatePos)
    return;
  int y = getPositionY();
  if (getPositionY()>=_DesPos.y)
  {
    setPositionY(getPositionY()-MOVESPEED);
    int CY = (getPositionY()+StarSpriteSizeY/2)/(OffSetY+StarSpriteSizeX);
    if (CY != _CoordY)
    {
      GameMgr->_StarMatrix->m_Maps[CY][_CoordX] = this;
      GameMgr->_StarMatrix->m_Maps[_CoordY][_CoordX] = nullptr;
      _CoordY = CY;
    }
  }
  else
  {
    setPositionY(_DesPos.y);
    _bUpdatePos = false;
  }
}

CStarSprite::CStarSprite()
{
  _CoordY = 0;
  _CoordX = 0;
  m_Type = EMT_YELLOW;
  _bSelected = false;
  _bUpdatePos = false;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CStarMatrix::CStarMatrix()
{
  initMap();
  GameMgr->_GameState = ETST_NONE;
}

CStarMatrix* CStarMatrix::create( const char* file )
{
  CStarMatrix* StarSprite = new CStarMatrix;
  if (StarSprite->initWithFile(file))
  {
    StarSprite->autorelease();
    StarSprite->Init();
    return StarSprite;
  }
  CC_SAFE_DELETE(StarSprite);
  return nullptr;
}

bool CStarMatrix::onTouchBegan( Touch * touch,Event * event )
{
  Vec2 point = touch->getLocation();
  int x = point.x-getPositionX();
  int y = point.y-getPositionY();
  int yc = y<0?-1:y/(StarSpriteSizeY+OffSetY);
  GameMgr->ClickCoord(x/(StarSpriteSizeX+OffSetX), yc);
  return false;
}

void CStarMatrix::onTouchMoved( Touch * touch,Event * event )
{

}

void CStarMatrix::onTouchEnded( Touch * touch,Event * event )
{

}

void CStarMatrix::initMap()
{
  for (int i = 0; i < MatrixYNum; i++)
  {
    for (int j = 0; j < MatrixXNum; j++)
    {
      m_Maps[i][j] = NULL;
    }
  }
}

void CStarMatrix::initSprite()
{
  srand(time(0));
  for (int i = 0; i < MatrixYNum; i++)
  {
    for (int j = 0; j < MatrixXNum; j++)
    {
      if (i == MatrixYNum -1)
      {
        m_Maps[i][j] = nullptr;
        continue;
      }
      int Type = rand()%TotleNum;
      char file[20] = "0";
      int Id = Type+1;//DataMgr->BlockMatrix.GetId(Type);
      CreateWithId(Id, i, j);
    }
  }
  //生成道具
  CreateProperty();
  CreateStone();
}

void CStarMatrix::CreateWithId(int Id, int x, int y)
{
  ItemProperty* Property = DataMgr->GetItemProperty(Id);
  if (!Property)
    return;
  CStarSprite* StarSprite = CStarSprite::create(Property->Pic.c_str());
  if (StarSprite)
  {
    StarSprite->setPosition(Vec2(x*(OffSetX+StarSpriteSizeX), y*(OffSetY+StarSpriteSizeY)));
    StarSprite->setContentSize(Size(StarSpriteSizeX, StarSpriteSizeY));
    //StarSprite->setScale(0.8f);
    StarSprite->setAnchorPoint(Vec2(0, 0));
    addChild(StarSprite);
    StarSprite->_CoordY = y;
    StarSprite->_CoordX = x;
    StarSprite->_ItemId = Id;
    StarSprite->_Property = *Property;
    m_Maps[y][x] = StarSprite;
  }
}

void CStarMatrix::CreateProperty()
{
  srand(time(0));
  int Num = GameMgr->GetPropNum();
  for (int i = 0; i < Num; i++)
  {
    int x = rand()%MatrixXNum;
    int y = rand()%(MatrixYNum-1);
    int PropNum = DataMgr->ProperSet.GetPropertySetNum();
    int Type = rand()%PropNum;
    int Id = DataMgr->ProperSet.GetItemId(Type);
    RemoveStar(x, y);
    CreateWithId(Id, x, y);
  }
}

void CStarMatrix::CreateStone()
{
  for (int i = 0; i < 2; i++)
  {
    int x = rand()%MatrixXNum;
    int y = rand()%(MatrixYNum-1);
    RemoveStar(x, y);
    CreateWithId(6, x, y);
  }
}

void CStarMatrix::Eliminate( int x, int y )
{
  if (x<0 || x >= MatrixXNum
    || y < 0 || y >= MatrixYNum)
    return;
  _SelectStars.clear();
  CStarSprite* Star = m_Maps[y][x];
  if (!Star||Star->_bUpdatePos)
    return;
  ItemProperty* Property = DataMgr->GetItemProperty(Star->_ItemId);
  if (!Property)
    return;
  std::vector<CStarSprite*> Temps;
  Temps.push_back(Star);
  int Type = Property->DetailType;
  while (Temps.size())
  {
    CStarSprite* TempStar = Temps.at(Temps.size()-1);
    TempStar->_bSelected = true;
    _SelectStars.push_back(TempStar);
    Temps.pop_back();
    int CoordX = TempStar->_CoordX;
    int CoordY = TempStar->_CoordY;
    CStarSprite* LinkStar = nullptr;
    ItemProperty* TempProperty = NULL;
    if (CoordX+1<MatrixXNum&&(LinkStar=m_Maps[CoordY][CoordX+1])&&
      !LinkStar->_bSelected/*&&LinkStar->m_Type == Type*/)
    {
      TempProperty = DataMgr->GetItemProperty(LinkStar->_ItemId);
      if (TempProperty&&TempProperty->DetailType == Type)
      {
        Temps.push_back(LinkStar);
        LinkStar->_bSelected = true;
      }
    }
    if (CoordY-1>=0&&(LinkStar=m_Maps[CoordY-1][CoordX])&&
      !LinkStar->_bSelected/*&&LinkStar->m_Type == Type*/)
    {
      TempProperty = DataMgr->GetItemProperty(LinkStar->_ItemId);
      if (TempProperty&&TempProperty->DetailType == Type)
      {
        Temps.push_back(LinkStar);
        LinkStar->_bSelected = true;
      }
    }
    if (CoordX-1>=0&&(LinkStar=m_Maps[CoordY][CoordX-1])&&
      !LinkStar->_bSelected/*&&LinkStar->m_Type == Type*/)
    {
      TempProperty = DataMgr->GetItemProperty(LinkStar->_ItemId);
      if (TempProperty&&TempProperty->DetailType == Type)
      {
        Temps.push_back(LinkStar);
        LinkStar->_bSelected = true;
      }
    }
    if (CoordY+1<MatrixYNum&&(LinkStar=m_Maps[CoordY+1][CoordX])&&
      !LinkStar->_bSelected/*&&LinkStar->m_Type == Type*/)
    {
      TempProperty = DataMgr->GetItemProperty(LinkStar->_ItemId);
      if (TempProperty&&TempProperty->DetailType == Type)
      {
        Temps.push_back(LinkStar);
        LinkStar->_bSelected = true;
      }
    }
  }
  int size= _SelectStars.size();
  ShowScoreAni(size);
  GameMgr->RefreshScore(size);
  GameMgr->ShowComboEffect(size, GameMgr->_MainScene);
  OperatorSelectStar();
}


void CStarMatrix::EliminateItem( int x, int y )
{
  if (!CheckValid(x, y))
    return;
  CStarSprite* Star = m_Maps[y][x];
  if (!Star||Star->_bUpdatePos)
    return;
  ItemProperty& Property = Star->_Property;
  if ((Property.Life -= 1) <= 0)          //生命值为0消除
  {
    Eliminate(x, y);
    GameMgr->_Hero->UpdatePositionY(EFT_DOWN, GameMgr->GetHeroUpdateY());
  }
  else                                    //剪生命值
  {

  }
}

void CStarMatrix::OperatorSelectStar()
{
  int size = _SelectStars.size();
  if (size)
  {
    HighlightSelect();
    GameMgr->_GameState = _SelectStars.size()>0?ETST_REMOVESELECT:ETST_NONE;
  }
}

void CStarMatrix::PushSelectStar(int x, int y)
{
  if (!CheckValid(x, y))
    return;
  CStarSprite* Star = m_Maps[y][x];
  if (!Star)
    return;
  ItemProperty* Property = DataMgr->GetItemProperty(Star->_ItemId);
  if (Property&&Property->Type==EIT_BLOCK)
    _SelectStars.push_back(Star);
}

void CStarMatrix::ShowScoreAni(int size)
{
  if (!size)
    return;
  int Score = DataMgr->EliminateScore.GetSocre(size);
  char buf[32] = "0";
  sprintf(buf, "+%d", Score);
  FloatWord* flyword = FloatWord::create(buf, 24, 
    Vec2(GameMgr->_Hero->getPositionX()+20, GameMgr->_Hero->getPositionY()));
  this->addChild(flyword);
  flyword->floatInOut(0.25f,0.50f,
    [=](){});
}

void CStarMatrix::RemoveStar( int x, int y )
{
  if (!m_Maps[y][x])
    return;
  m_Maps[y][x]->removeFromParentAndCleanup(true);
  m_Maps[y][x] = nullptr;
}

void CStarMatrix::HighlightSelect()
{
  int size = _SelectStars.size();
  for (int i = 0; i < size; i++)
  {
    CStarSprite* Star = _SelectStars[i];
    //消除描边
    Star->setEffect(EffectOutline::create());
  }
}

void CStarMatrix::RemoveSelectStar()
{
  do 
  {
    int size = _SelectStars.size();
    if (!size)
      break;
    CStarSprite* Star = _SelectStars[0];
    _SelectStars.pop_front();
    ItemProperty* Property = DataMgr->GetItemProperty(Star->_ItemId);
    if (!Property)
      continue;
    GameMgr->showStarParticleEffect(Property->DetailType,Star->getPosition(),this);
    m_Maps[Star->_CoordY][Star->_CoordX] = nullptr;
    Star->removeFromParentAndCleanup(true);
  } while (0);
}

void CStarMatrix::UpdateDesPosition()
{
  for (int i = 0; i < MatrixYNum; i++)
  {
    for (int j = 0; j < MatrixXNum; j++)
    {
      CStarSprite* Star = m_Maps[i][j];
      if (!Star)
        continue;
      int NullRow = 0;
      for (int k = i-1; k >= 0; k--)
      {
        if (!m_Maps[k][j])
          NullRow++;
      }
      if (!NullRow)
        continue;
      Star->UpdatePosition(NullRow);
    }
  }
}

void CStarMatrix::Update( float time )
{
  int size = _SelectStars.size();
  if (size)
  {
    if (CIntervalSchedule02::GetInstance()->Schedule(time))
      RemoveSelectStar();
  }
  else if (GameMgr->_GameState == ETST_REMOVESELECT)
  {
    UpdateDesPosition();
    GameMgr->_Hero->UpdatePositionY(EFT_DOWN, GameMgr->GetHeroUpdateY());
    GameMgr->_GameState = ETST_NONE;
  }
  else
  {
    for (int i = 0; i < MatrixXNum; i++)
    {
      for (int j = 0; j < MatrixYNum; j++)
      {
        CStarSprite* Star = m_Maps[i][j];
        if (Star && Star->_bUpdatePos)
          Star->Update(time);
      }
    }
  }
}

void CStarMatrix::Init()
{
  //事件监听
  auto listener = EventListenerTouchOneByOne::create();
  listener->onTouchBegan = CC_CALLBACK_2( CStarMatrix::onTouchBegan, this );
  listener->onTouchMoved = CC_CALLBACK_2( CStarMatrix::onTouchMoved, this );
  listener->onTouchEnded = CC_CALLBACK_2( CStarMatrix::onTouchEnded, this );
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, this );
}

void CStarMatrix::RemoveAllStars()
{
  for (int j = 0; j < MatrixYNum; j++)
  {
    for (int i = 0; i < MatrixXNum; i++)
    {
      CStarSprite* Star = m_Maps[i][j];
      if (Star)
      {
        m_Maps[Star->_CoordY][Star->_CoordX] = nullptr;
        Star->removeFromParentAndCleanup(true);
      }
//      PushSelectStar(i, j);
    }
  }
}

void CStarMatrix::NewGame()
{
  RemoveAllStars();
  initSprite();
}

/************************************************************************/
/*       CPlayerSprite                                                  */
/************************************************************************/

CPlayerSprite::CPlayerSprite()
{
  _MoveType = EFT_NONE;
}

CPlayerSprite* CPlayerSprite::Create( const char* file )
{
  CPlayerSprite* PlayerSprite = new CPlayerSprite;
  if (PlayerSprite->initWithFile(file))
  {
    PlayerSprite->autorelease();
    PlayerSprite->setAnchorPoint(Vec2(0, 0));
    PlayerSprite->_CoordY = 10;
    PlayerSprite->_CoordX = 5;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //初始化在屏幕正中上方
    PlayerSprite->setPosition(Vec2(GameMgr->_StarMatrix->getPositionX()+5*(StarSpriteSizeX+OffSetX), 
      GameMgr->_StarMatrix->getPositionY()+10*(StarSpriteSizeY+OffSetY)));
    return PlayerSprite;
  }
  CC_SAFE_DELETE(PlayerSprite);
  return nullptr;
}

void CPlayerSprite::Update( float time )
{
  if (!_bUpdatePos)
    return;
  int posX = getPositionX();
  if (_MoveType == EFT_LEFT)
  {
    if (getPositionX()>=_DesPos.x)
    {
      setPositionX(getPositionX()-MOVESPEED);
      int CX = (getPositionX()-StartX+StarSpriteSizeX/2)/(OffSetX+StarSpriteSizeX);
      _CoordX = CX!=_CoordX?CX:_CoordX;
      GameMgr->CheckSquare(_CoordX, _CoordY);
    }
    else
    {
      setPositionX(_DesPos.x);
      _bUpdatePos = false;
      UpdatePositionY(EFT_DOWN, GameMgr->GetHeroUpdateY());
    }
  }
  else if (_MoveType == EFT_RIGHT)
  {
    if (getPositionX()<=_DesPos.x)
    {
      setPositionX(getPositionX()+MOVESPEED);
      int CX = (getPositionX()-StartX+StarSpriteSizeX/2)/(OffSetX+StarSpriteSizeX);
      _CoordX = CX!=_CoordX?CX:_CoordX;
      GameMgr->CheckSquare(_CoordX, _CoordY);
    }
    else
    {
      setPositionX(_DesPos.x);
      _bUpdatePos = false;
      UpdatePositionY(EFT_DOWN, GameMgr->GetHeroUpdateY());
    }
  }
  else if (_MoveType == EFT_DOWN)
  {
    if (getPositionY()>_DesPos.y)
    {
      setPositionY(getPositionY()-MOVESPEED);
      int CY = (getPositionY()-StartY+StarSpriteSizeY/2)/(OffSetY+StarSpriteSizeY);
      _CoordY = CY!=_CoordY?CY:_CoordY;
      GameMgr->CheckSquare(_CoordX, _CoordY);
    }
    else
    {
      setPositionY(_DesPos.y);
      GameMgr->CheckClearance();
      _bUpdatePos = false;
    }
  }
}

void CPlayerSprite::UpdatePositionX( int Type, int Block )
{
  _MoveType = Type;
  _DesPos = Vec2(StartX+Block*(StarSpriteSizeX+OffSetX), getPositionY());
  //_CoordX = Block;
  _bUpdatePos = true;
}

void CPlayerSprite::UpdatePositionY( int Type, int DesY )
{
  _MoveType = Type;
  _DesPos = Vec2(getPositionX(), StartY+DesY*(StarSpriteSizeX+OffSetX));
  //_CoordY = DesY;
  _bUpdatePos = true;
}

void CPlayerSprite::TurnLeft()
{
}

void CPlayerSprite::TurnRight()
{
}

void CPlayerSprite::TurnUp()
{
}

void CPlayerSprite::TurnDown()
{
}

void CPlayerSprite::NewGame()
{
  _CoordX = HeroStartX;
  _CoordY = HeroStartY;
  //初始化在屏幕正中上方
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  setPosition(Vec2(GameMgr->GetMatrixX()+_CoordX*(StarSpriteSizeX+OffSetX), 
    GameMgr->GetMatrixY()+_CoordY*(StarSpriteSizeY+OffSetY)));
}
