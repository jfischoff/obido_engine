#include "CameraOrbit.h"
#include <Vector3.h>
#include <Vector2.h>
#include <assert.h>
#include "Camera.h"
#include <iostream>
using namespace std;

void* CameraOrbit::Constructor()
{
  return new CameraOrbit();
}

void CameraOrbit::updateCamera(Camera& cam)
{
	//convert spherical coords to cart

	Float x = m_Radius * sin(m_Phi) * cos(m_Gamma);
	Float y = m_Radius * sin(m_Phi) * sin(m_Gamma);
	Float z = m_Radius * cos(m_Phi);

	cam.T().setPivot(Vector3(x, y, z));

	//compute rotation matrix

	Matrix3x3 rotation = Matrix3x3::Identity();

	//I need to normalize the axis

	float upMag = sqrt(m_UpX*m_UpX + m_UpY*m_UpY + m_UpZ*m_UpZ);

	float n10 = m_UpX / upMag;
	float n11 = m_UpY / upMag;
	float n12 = m_UpZ / upMag;

	rotation.setElement(1, 0, n10);
	rotation.setElement(1, 1, n11);
	rotation.setElement(1, 2, n12);
	
	float m20 = x;
	float m21 = y;
	float m22 = z;

	float eveVectMag = sqrt(m20*m20 + m21*m21 + m22*m22);

	float n20 = m20 / eveVectMag;
	float n21 = m21 / eveVectMag;
	float n22 = m22 / eveVectMag;

	rotation.setElement(2, 0, n20);
	rotation.setElement(2, 1, n21);
	rotation.setElement(2, 2, n22);

	//cross for x
	float m00 = n11*n22 - n12*n21;
	float m01 = n12*n20 - n10*n22;
	float m02 = n10*n21 - n11*n20;

	float mag = sqrt(m00*m00 + m01*m01 + m02*m02); 

	rotation.setElement(0, 0, m00 / mag);
	rotation.setElement(0, 1, m01 / mag);
	rotation.setElement(0, 2, m02 / mag);

	//cross for y
	float m10 = m01*n22 - m02*n21; 
	float m11 = m02*n20 - m00*n22; 
	float m12 = m00*n21 - m01*n20; 
	
	float magY = sqrt(m10*m10 + m11*m11 + m12*m12); 

	rotation.setElement(1, 0, m10 / magY);
	rotation.setElement(1, 1, m11 / magY);
	rotation.setElement(1, 2, m12 / magY);

	cam.T().setRotationMatrix(RotationMatrix(rotation));
}

void CameraOrbit::screenRotate(int startX, int startY, 
			       int endX, int endY, Camera& camera)
{
  if(startX == endX && startY == endY)
    return;
	

  Vector3 deviceCenter(0, 0, 0);
  Vector3 center(m_CenterX, m_CenterY, m_CenterZ);

  //convert from view coordinates to device coordinates

  Vector2 start(startX, startY);
  Vector3 startWorld = camera.screenToWorld(start);
  Vector3 startDevice = camera.screenToDevice(start);
  

  Vector2 end(endX, endY);
  Vector3 endWorld = camera.screenToWorld(end);
  Vector3 endDevice = camera.screenToDevice(end);

  //draw a point to the center
  Vector3 centerVec = startWorld - center;
  Vector3 unnormalizedCenterVec = startWorld - center;
  centerVec = centerVec.normalize();

  Vector3 centerVec1 = endWorld - center;
  centerVec1 = centerVec1.normalize();
	
  Vector3 rotationAxis = Vector3::Cross(centerVec, centerVec1);
  rotationAxis = rotationAxis.normalize();


  Vector3 deviceCenterVec = startDevice - deviceCenter;
  deviceCenterVec = deviceCenterVec.normalize();
  Vector3 deviceCenterVec1 = endDevice - deviceCenter;
  deviceCenterVec1 = deviceCenterVec1.normalize();
  
  float angle = -3.0* deviceCenterVec.angle(deviceCenterVec1);

  //cout << "angle " << angle << "\n";

  AngleAxis rotationAxisAngle(rotationAxis.getX(), 
			      rotationAxis.getY(), rotationAxis.getZ(), angle);

  //Line centerVecToOrigin(unnormalizedCenterVec, center);
  //cout << "unnormalizedCenterVec = " << unnormalizedCenterVec << "\n";
  //DebugViewer::Instance()->addLine("centerVec", centerVecToOrigin);
  //DebugViewer::Instance()->addVector3("centerVec1", centerVec);
  //DebugViewer::Instance()->addVector3("rotationAxis", rotationAxis);

  assert(isnan(rotationAxis.getX()) == 0);
  assert(isnan(rotationAxis.getY()) == 0);
  assert(isnan(rotationAxis.getZ()) == 0);
  assert(isnan(angle) == 0);

  //remove for testing
  float distance = deviceCenterVec.distance(deviceCenterVec1);
  if(distance != 0.0f)
    {
		camera.appendOrbit(rotationAxisAngle);
    }
}

void CameraOrbit::SetOrigin(Vector3 origin)
{
  m_CenterX = origin.getX();
  m_CenterY = origin.getY();
  m_CenterZ = origin.getZ();
}
