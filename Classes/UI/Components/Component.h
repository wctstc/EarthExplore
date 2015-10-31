#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CBaseSprite : public Sprite
{
public:
  virtual bool bHit(Vec2 TouchPoint);
};

class Effect : public Ref
{
public:
  GLProgramState* getGLProgramState() const;
  virtual void setTarget(class EffectSprite *sprite);
protected:
  bool initGLProgramState(const std::string &fragmentFilename);
  Effect();
  virtual ~Effect();
  GLProgramState *_glprogramstate;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
  std::string _fragSource;
  EventListenerCustom* _backgroundListener;
#endif
};

class EffectOutline : public Effect
{
public:
  CREATE_FUNC(EffectOutline);
  bool init();
};

class EffectSprite : public CBaseSprite
{
public:
  static EffectSprite *create(const std::string& filename);
  virtual void setEffect(Effect* effect);
  virtual void addEffect(Effect *effect, ssize_t order);
  void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
protected:
  EffectSprite() : _defaultEffect(nullptr)
  {
    _effects.reserve(2);
  }
  virtual ~EffectSprite();
  std::vector<std::tuple<ssize_t,Effect*,QuadCommand>> _effects;
  Effect* _defaultEffect;
};

class CStarSprite : public EffectSprite
{
public:
  int _CoordX;
  int _CoordY;
  int m_Type;
  int _ItemId;
  bool _bSelected;
  Vec2 _DesPos;
  bool _bUpdatePos;
  ItemProperty _Property;
  CStarSprite();
public:
  static CStarSprite* create(const char* file);
  void UpdatePosition(int NullBlock);
  void Update(float time);
};

class CStarMatrix : public CBaseSprite //CCLayer
{
public:
  std::deque<CStarSprite*> _SelectStars;
  CStarSprite* m_Maps[MatrixYNum][MatrixXNum];
public:
  static CStarMatrix* create(const char* file);
  virtual bool onTouchBegan(Touch * touch,Event * event);
  virtual void onTouchMoved(Touch * touch,Event * event);
  virtual void onTouchEnded(Touch * touch,Event * event);
public:
  CStarMatrix();
  void initMap();
  void initSprite();
  void Eliminate(int x, int y);
  void EliminateItem(int x, int y);
  void RemoveStar(int x, int y);
  void RemoveSelectStar();
  void HighlightSelect();
  void UpdateDesPosition();
  void PushSelectStar(int x, int y);
  void ShowScoreAni(int size);
  void OperatorSelectStar();
  void Update(float time);
  void Init();
  void RemoveAllStars();
  void NewGame();
  void CreateProperty();
  void CreateStone();
  void CreateWithId(int Id, int x, int y);
};

class CPlayerSprite : public CStarSprite
{
public:
  int _MoveType;
public:
  CPlayerSprite();
  static CPlayerSprite* Create(const char* file);
public:
  void Update(float time);
  void UpdatePositionX(int Type, int Block);
  void UpdatePositionY(int Type, int DesY);
  //ÒÆ¶¯Ïà¹Ø
  void TurnLeft();
  void TurnRight();
  void TurnUp();
  void TurnDown();
  void NewGame();
};