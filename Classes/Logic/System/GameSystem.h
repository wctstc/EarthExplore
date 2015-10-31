#pragma once

#define OP(T) \
  C##T* Get##T(){return C##T::GetInstance();}

#define CREATE_FUN OP(Reward) \


public:
  CREATE_FUN