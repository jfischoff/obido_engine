#ifndef SCENE_H
#define SCENE_H

#include "IScene.h"
#include "CameraManager.h"
#include <vector>
using namespace std;

//need to add animation manager

class Scene : public IScene, IUniformMapper
{
public:
    virtual bool setup(string& message);
    virtual void teardown();
    
    virtual void update(double time);
    virtual bool draw(double time, string& message);

    //Camera Manager Methods
    void setActiveCamera(uint index);

    void screenRotate(Vector2 start, Vector2 end);
    void dolly(double amount);
    void pan(Vector2 start, Vector2 end);
    
    void tilt(const Vector2& lastTouch0, const Vector2& lastTouch1,
	      const Vector2& currentTouch0, const Vector2& currentTouch1);

    void moveCamera(const Vector2& lastTouch0, const Vector2& lastTouch1,
		    const Vector2& currentTouch0, const Vector2& currentTouch1);

    void addRenderable(Renderable* renderable);
    Renderable* getRenderable(unsigned int renderableIndex);
    const Renderable* getRenderable(unsigned int renderableIndex) const;
    unsigned int renderableCount() const;

private:
    CameraManager* m_CameraManager;
    vector<Renderable*> m_Renderables;
    //need to add the materials to this
    UniformMapperNode m_UniformMapperNode;
    SceneGraph m_SceneGraph;
    AnimationManager m_AnimationManager;
    
};

#endif SCENE_H
