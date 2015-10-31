#include "logicHeader.h"
#include "GUIHeader.h"

FloatWord* FloatWord::create(const std::string& word,const int fontSize,Vec2 begin){
	FloatWord* ret = new FloatWord();
	if(ret && ret->init(word,fontSize,begin)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool FloatWord::init(const std::string& word,const int fontSize,Vec2 begin){
	if(!Node::init()){
		return false;
	}
	_begin = begin;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	_label = Label::create(word,"Verdana-Bold",fontSize);
	_label->setPosition(begin);
	this->addChild(_label);
	
	return true;
}

void FloatWord::floatIn(const float delay,std::function<void()> callback){
	Size visibleSize = Director::getInstance()->getVisibleSize();
	MoveTo* move = MoveTo::create(delay,Vec2(visibleSize.width/2,_begin.y));
	CallFunc* call = CallFunc::create(callback);
	Sequence* action = Sequence::create(move,call,NULL);
	_label->runAction(action);
}

void FloatWord::floatOut(const float delay,std::function<void()> callback){
	Size visibleSize = Director::getInstance()->getVisibleSize();
	MoveTo* move = MoveTo::create(delay,Vec2(0,_begin.y));
	CallFunc* call = CallFunc::create(callback);
	CallFunc* removeC = CallFunc::create([this](){
		this->removeFromParentAndCleanup(true);
	});
	Sequence* action = Sequence::create(move,call,removeC,NULL);
	_label->runAction(action);
}

void FloatWord::floatInOut(const float speed,const float delayTime,std::function<void()> callback){
	Size visibleSize = Director::getInstance()->getVisibleSize();
	MoveTo* moveIn = MoveTo::create(speed,Vec2(_begin.x,_begin.y));
	MoveTo* moveOut = MoveTo::create(speed,Vec2(_begin.x,_begin.y+35));
	CallFunc* removeC = CallFunc::create([this](){
		this->removeFromParentAndCleanup(true);
	});
	Sequence* action = Sequence::create(moveIn,DelayTime::create(delayTime),moveOut,removeC,CallFunc::create(callback),NULL);
	_label->runAction(action);
}