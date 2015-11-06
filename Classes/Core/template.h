#pragma once

// class FArray
// {
// public:
//   void* _data;
//   int _size;
//   int _maxsize;
// };
// 
// template<class T>
// class TArray : public FArray
// {
// public:
//   TArray()
//   {
// 
//   }
// };

template<class T>
class TArray
{
private:
  void* _data;
  int _size;
  int _maxsize;
public:
  TArray(int set)
  {
    _size = set;
    _maxsize = SizeStrategy(_size);
    Relloc();
  }
public:
  virtual ~TArray()
  {
    for (int i = 0; i < _size; i++)
    {
      ((T*)_data)[i].~T();
    }
    if (_data)
      free(_data);
    _data = NULL;
  }
  const T& operator()( int i ) const
  {
    //check(IsValidIndex(i));
    return ((T*)_data)[i];
  }
  T& operator()( int i )
  {
    //check(IsValidIndex(i));
    return ((T*)_data)[i];
  }
  TArray()
  {
    _data = NULL;
    _size = 0;
    _maxsize = SizeStrategy(_size);
    Relloc();
  }
  TArray( TArray& v)
  {
    if (!v._size)
      return;
    _size = v.Num();
    _maxsize = v._maxsize;
    _data = malloc(_maxsize*sizeof(T));
    memcpy(&(((T*)_data)[0]), &v(0), v.Num()*sizeof(T));
    /* ·½Ê½2
    for (int i = 0; i < v.Num(); i++)
      AddItem(v(i));
    */
  }
  void Relloc()
  {
    void* tptr = malloc(_maxsize*sizeof(T));
    if (_data)
    {
      do
      {
        int tempsize = _size-1;
        if (!tempsize)
          break;
        memcpy(&(((T*)tptr)[0]), &(((T*)_data)[0]), tempsize*sizeof(T));
//         for (int i = 0; i < tempsize; i++)
//         {
//           ((T*)_data)[i].~T();
//         }
      }while(0);
      free(_data);
    }
    _data = tptr;
  }
  void Empty()
  {
    for (int i = 0; i < _size; i++)
    {
      ((T*)_data)[i].~T();
    }
    _size = 0;
  }
  void SetSize(int newSize)
  {
  }
  int SizeStrategy(int set)
  {
    //return set + 1;
    return set + 3*set/8 + 32;
  }
  void pop_back()
  {
    if (_size)
    {
      ((T*)_data)[_size-1].~T();
      _size--;
    }
  }
  int Add(int num=1)
  {
    int ret = _size;
    if ((_size+=num)>_maxsize)
    {
      _maxsize = SizeStrategy(_size);
      Relloc();
    }
    return ret;
  }
  void Remove(int idx, int num =1)
  {
    if (idx<0||idx+num>=_size)
      return;
    for (int i = idx; i < idx+num; i++)
    {
      ((T*)_data)[i].~T();
    }
    for (int i = idx+num; i < _size; i++)
    {
      ((T*)_data)[i-num] = ((T*)_data)[i];
      ((T*)_data)[i].~T();
    }
    _size -= num;
  }
  int AddItem(const T& Item)
  {
    new(*this) T(Item);
    return Num()-1;
  }
  void Push(const T& Item)
  {
    AddItem(Item);
  }
  void push_back(const T& Item)
  {
    AddItem(Item);
  }
  int Num() const
  {
    return _size;
  }
  bool IsValidIndex(int Index) const
  {
    return Index>=0&&Index<_size;
  }
  public:
    class TIterator
    {
      TIterator(TArray<T>& v):vec(v),Index(0)
      {
      }
    private:
      TArray<T>& vec;
      int Index;
    };
};

template<class T>
void* operator new(size_t set, TArray<T>& v)
{
  int index = v.Add(1);
  return &v(index);
}
