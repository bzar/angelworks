#ifndef ANGELWORKSEVENT_H
#define ANGELWORKSEVENT_H

class AngelWorksEvent
{
public:
  virtual ~AngelWorksEvent() {}
  virtual void* getObject() { return this; }
};

template<typename T>
class AngelWorksEventWrapper : public AngelWorksEvent
{
public:
  AngelWorksEventWrapper(T* t): t(t) {}
  virtual ~AngelWorksEventWrapper() { delete t; }
  virtual void* getObject() { return t; }
  T* t;
};

#endif // ANGELWORKSEVENT_H
