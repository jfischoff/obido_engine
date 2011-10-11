#include "Scene.h"

bool Scene::setup(string& message)
{

  //go through all the renderables and call setup
  unsigned int renderableCount = renderableCount();
  string message;
  for(unsigned int renderableIndex = 0; renderableIndex < renderableCount; renderableIndex++)
    {
      Renderable* renderable = getRenderable(renderableIndex);
     
      if(renderable->setup(message) == false) return false;      
    }

   return true;
}

void Scene::teardown()
{
  unsigned int renderableCount = renderableCount();
  string message;
  for(unsigned int renderableIndex = 0; renderableIndex < renderableCount; renderableIndex++)
    {
      Renderable* renderable = getRenderable(renderableIndex);
     
      renderable->teardown(message);      
    }

}

void Scene::update(double time)
{  
  unsigned int renderableCount = renderableCount();
  string message;
  for(unsigned int renderableIndex = 0; renderableIndex < renderableCount; renderableIndex++)
    {
      Renderable* renderable = getRenderable(renderableIndex);
     
      renderable->update(time);
    }
}


bool Scene3::draw(double time, string& message)
{  
  unsigned int renderableCount = renderableCount();
  string message;
  for(unsigned int renderableIndex = 0; renderableIndex < renderableCount; renderableIndex++)
    {
      Renderable* renderable = getRenderable(renderableIndex);
      
      if(renderable->draw(time, message) == false) return false;
      
    }

  return true;
}


