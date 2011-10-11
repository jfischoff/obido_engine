#ifndef CAMERAORBIT_H
#define CAMERAORBIT_H

#include <Vector3.h>
class Camera;

class CameraOrbit
{
 public:
  static void* Constructor();

  void setSphericalCoords(float gamma, float phi){m_Gamma = gamma; m_Phi = phi;}
  void setRadius(float radius){m_Radius = radius;}
	
  void setUpVector(float upX, float upY, float upZ){m_UpX = upX; m_UpY = upY; m_UpZ = upZ;}

  void updateCamera(Camera& cam);

  void appendPhi(float phiDelta){m_Phi += phiDelta;}
  void appendGamma(float gammaDelta){m_Gamma += gammaDelta;}
  void appendRadius(float radiusDelta){m_Radius += radiusDelta;}
	
  void screenRotate(int startX, int startY, int endX, int endY,
		    Camera& camera);

  void SetOrigin(Vector3 origin);
 private:
	

  float m_Radius;
  float m_Gamma;
  float m_Phi;

  float m_UpX;
  float m_UpY;
  float m_UpZ;
	
  float m_CenterX;
  float m_CenterY;
  float m_CenterZ;

};

#endif //CAMERAORBIT_H
