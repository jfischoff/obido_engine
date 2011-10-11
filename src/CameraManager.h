#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

class CameraGestures;

class CameraManager
{
public:
    static CameraManager* Constructor();
    CameraManager();
    CameraManager(unsigned int activeIndex, Camera** cameras, unsigned int count);
    CameraManager(const CameraManager& other);
    CameraManager& operator=(const CameraManager& other);
    void copy(const CameraManger& other);

    //CameraGestures methods
    void screenRotate(Vector2 start, Vector2 end);
    void dolly(double amount);
    void pan(Vector2 start, Vector2 end);
    
    void tilt(const Vector2& lastTouch0, const Vector2& lastTouch1,
	      const Vector2& currentTouch0, const Vector2& currentTouch1);

    void moveCamera(const Vector2& lastTouch0, const Vector2& lastTouch1,
		    const Vector2& currentTouch0, const Vector2& currentTouch1);

    
    
private:
    CameraGestures* m_CameraGestures;

};

#endif CAMERAMANAGER_H
