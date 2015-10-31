#pragma once

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <fstream>
#include <time.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "../Log/Log.h"
#include "Core/template.h"
#include "../CommonHeader/value_define.h"
#include "../CommonHeader/ac_define.h"
#include "GeneralKit.h"
#include "LuaObject.h"

#include "../GlobalSchedule/GlobalSchedule.h"
#include "../GlobalSchedule/IntervalSchedule.h"
#include "../Reward/Reward.h"
#include "../Player/Player.h"
#include "../Animation/Animation.h"
#include "DataManager.h"
#include "GameManager.h"

#include "GUIHeader.h"


extern CGameManger* GameMgr;
extern CDataManager* DataMgr;
extern CPlayerControll* GSelfCtl;