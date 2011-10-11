#ifndef ISCENE_H
#define ISCENE_H

#include <string>
using namespace std;

class IScene
{
  public:
    virtual bool setup(string& message) = 0;
    virtual void teardown() = 0;
    
    virtual void update(double time) = 0;
    virtual bool draw(double time, string& message) = 0;
      
};

#endif ISCENE_H
