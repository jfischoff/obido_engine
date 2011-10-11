#include "Camera.h"
#include <iostream>
#include <assert.h>
using namespace std;

void* Camera::Constructor()
{
  return new Camera();
}

Camera::Camera()
{
  m_ScreenDim = Vector2(1, 1);
}

Camera::Camera(const ProjectionMatrix& projectionMatrix, 
	       const Transform& transform, const Vector2& screenDim)
{
  m_ProjectionMatrix = projectionMatrix;
  m_Transform = transform;
  m_ScreenDim = screenDim;
}

Camera::Camera(const Camera& camera)
{
  copy(camera);	
}

Camera& Camera::operator=(const Camera& camera)
{
  copy(camera);
  return *this;
}

void Camera::copy(const Camera& camera)
{
  m_ProjectionMatrix = camera.m_ProjectionMatrix;
  m_Transform = camera.m_Transform;
  m_ScreenDim = camera.m_ScreenDim;
}

void Camera::deepCopy(const Camera* other)
{
  if(this == other) return;
  copy(*(Camera*)other);
}

void Camera::destroy()
{

}

ProjectionMatrix& Camera::P()
{
  return m_ProjectionMatrix;
}

const ProjectionMatrix& Camera::PC() const 
{
  return m_ProjectionMatrix;
}

Transform& Camera::T()
{
  return m_Transform;
}

const Transform& Camera::TC() const
{
  return m_Transform;
}

void Camera::setView(int width, int height)
{
  m_ScreenDim.setX(width);
  m_ScreenDim.setY(height);
}

Matrix4x4 Camera::makeViewMatrix() const
{
  Matrix4x4 output = Matrix4x4::Identity();

  output.setElement(0, 0, (float)getHeight() / (float)getWidth());
  //output.setElement(1, 1, 1.0 / (float)m_Height)//;

  return output;
}

Matrix4x4 Camera::makeInverseViewMatrix() const
{
  Matrix4x4 output = Matrix4x4::Identity();

  output.setElement(0, 0, (float)getWidth() / (float)getHeight() );

  return output;
}

Matrix4x4 Camera::getLocalMatrix()
{
  return m_Transform.toMatrix4x4();
}

Matrix4x4 Camera::getInverseLocalMatrix()
{
  return Matrix4x4::Inverse(m_Transform.toMatrix4x4());
}

Matrix4x4 Camera::getMV() const
{

  return getModelToView();
}

Matrix4x4 Camera::getMVP() const
{
  return getModelToDevice();
}

Vector3 Camera::screenToDevice(const Vector2& screenPos) const
{
  Vector3 screen3(screenPos.getX(), screenPos.getY(), PC().getNearZ());
  return screenToDevice(screen3);
	
}

Vector3 Camera::screenToWorld(const Vector2& screenPos) const
{
  Vector3 result;
  screenToWorld(screenPos, result);
  return result;
}

void Camera::screenToWorld(const Vector2& screenPos, 
			   Vector3& worldPos) const
{
  float x, y, z;
  screenToWorld(screenPos.getX(), screenPos.getY(), x, y, z);

  worldPos.setX(x);
  worldPos.setY(y);
  worldPos.setZ(z);
}

void Camera::screenToWorld(float screenX, float screenY,
			   float& worldX, float& worldY, float& worldZ) const
{

 
  //convert to device
  Vector3 screen3(screenX, screenY, PC().getNearZ());
  Vector3 result = screenToModel(screen3);
  worldX = result.getX();
  worldY = result.getY();
  worldZ = result.getZ();


}



void Camera::Dolly(double offset)
{
  //get the translation vector
  //Vector3 translationDirection = m_Transform.getOutVector();  
  //scale by the amount
  //Vector3 translationAmount = translationDirection.scale(offset);

  //translate
  m_Transform.appendOffset(Vector3(0, 0, offset));
}



void Camera::appendPivot(const Vector3& trans)
{
  m_Transform.appendPivot(trans);
}

void Camera::appendOffset(const Vector3& trans)
{
  m_Transform.appendOffset(trans);
}

Vector3 Camera::getPosition() const
{
  return m_Transform.getTransformedPosition();
}

void Camera::appendRotation(const AngleAxis& angleAxis)
{
  m_Transform.appendRotation(angleAxis);
}




void Camera::getRay(const Vector2& screenPos, Ray& ray) const
{
  //check the math
  assert(0);
  //get start
  Vector3 startPos;
  screenToWorld(screenPos, startPos);

	
  //how do I figure out what the z position is?
	
  //I could put in another z value
  //and solve for x and y
  //I think what I need to do is multiply a device coord times the projection matrix and see what I get
	
  //then make the ray
	
  //If I did my math write it is 
  Matrix4x4 projectionMatrix = m_ProjectionMatrix.getMatrix();
	
  float a = projectionMatrix.getElement(0, 0);
  float b = projectionMatrix.getElement(1, 1);
  float c = projectionMatrix.getElement(2, 2);
  float d = projectionMatrix.getElement(2, 3);
	
  Vector3 slope = Vector3(a / (2 * d), b / (2 * d), -d);
	
  ray = Ray(startPos, slope);
	
}

void Camera::getDeviceRay(const Vector2& screenPos, Ray& ray, double z) const
{
	
  //assume it is at z = -1
	
  Vector3 startDevice = screenToDevice(screenPos);
  startDevice.setZ(z);
  Vector3 endDevice(startDevice.getX(), startDevice.getY(), 1.0);
	
  ray = Ray(startDevice, endDevice);
}

Ray Camera::getViewRay(const Vector2& screenPos, double z) const
{
  Vector3 zPoint(0, 0, 1);
  double deviceZ = viewToDevice(zPoint).getZ();
	
  Ray deviceRay;
  getDeviceRay(screenPos, deviceRay, deviceZ);
	
  Matrix4x4 deviceToView = getDeviceToView();
  return deviceRay.transform(deviceToView);
}

Ray Camera::getModelRay(const Vector2& screenPos, double z) const
{
  Matrix4x4 viewToModel = getViewToModel();

  return getViewRay(screenPos, z).transform(viewToModel);
}

float Camera::deviceToWorldZ(float deviceZ) const
{
  //get the near far range
  //or use the existing conversion
	
  Vector3 device(1.0, 1.0, deviceZ);
  Vector3 world = deviceToModel(device);
  return world.getZ();
}

Float Camera::getHeight() const
{
  return m_ScreenDim.getY();
}

Float Camera::getWidth() const
{
  return m_ScreenDim.getX();	
}


Matrix4x4 Camera::getModelToView() const
{
  return m_Transform.toInverseMatrix4x4();
}

Matrix4x4 Camera::getViewToModel() const
{
  return m_Transform.toMatrix4x4();
}

Matrix4x4 Camera::getViewToDevice() const
{
  return m_ProjectionMatrix.getMatrix() * makeViewMatrix();
}

Matrix4x4 Camera::getDeviceToView() const
{
  return makeInverseViewMatrix() * Matrix4x4::Inverse(m_ProjectionMatrix.getMatrix());
}

Matrix4x4 Camera::getDeviceToScreenDevice() const
{
  return makeViewMatrix();
}

Matrix4x4 Camera::getScreenDeviceToDevice() const
{
  return makeInverseViewMatrix();
}

Matrix4x4 Camera::getScreenDeviceToScreen() const
{
  //I need to scale everything to the size of the screen
  //then move it to the right corner
	
  float halfHeight = getHeight() / 2.0;
  float halfWidth = getWidth() / 2.0;
	
  Matrix4x4 scaleMat = Matrix4x4::Identity();
  scaleMat.setElement(0, 0, halfWidth);
  scaleMat.setElement(1, 1, -halfHeight);
	
  Matrix4x4 translateMat = Matrix4x4::Identity();
  translateMat.setElement(3, 0, halfWidth);
  translateMat.setElement(3, 1, halfHeight);
	
  return Matrix4x4::Multiply(scaleMat, translateMat);
	
}

Matrix4x4 Camera::getDeviceToScreen() const
{
  return getDeviceToScreenDevice() * getScreenDeviceToScreen();
}

Matrix4x4 Camera::getScreenToDevice() const
{
  return !getDeviceToScreen();
}

Matrix4x4 Camera::getModelToDevice() const
{
  return getModelToView() * getViewToDevice();
}

Matrix4x4 Camera::getDeviceToModel() const
{
  return Matrix4x4::Inverse(getModelToDevice());
}

Matrix4x4 Camera::getModelToScreen() const
{
  return getModelToDevice() * getDeviceToScreen();
}

Matrix4x4 Camera::getScreenToModel() const
{
  return Matrix4x4::Inverse(getModelToScreen());
}

Matrix4x4 Camera::getViewToScreen() const
{
  return getViewToDevice() * getDeviceToScreen();
}

Matrix4x4 Camera::getScreenToView() const
{
  return Matrix4x4::Inverse(getViewToScreen());
}

//Vector Transforms
Vector3 Camera::modelToView(const Vector3& model) const
{
  Matrix4x4 mat = getModelToView();
  return HomogenusPosition(model) * mat;
}

Vector3 Camera::viewToModel(const Vector3& view) const
{
  Matrix4x4 mat = getViewToModel();
  return HomogenusPosition(view) * mat;
}

Vector3 Camera::viewToDevice(const Vector3& view) const
{
  Matrix4x4 mat = getViewToDevice();
  HomogenusPosition homoVec = HomogenusPosition(view);
  HomogenusPosition resultHomo = homoVec * mat;
  
  return resultHomo;
}

Vector3 Camera::deviceToView(const Vector3& view) const
{
  Matrix4x4 mat = getDeviceToView();
  return HomogenusPosition(view) * mat;
}

Vector3 Camera::deviceToScreenDevice(const Vector3& view) const
{
  Matrix4x4 mat = getDeviceToScreenDevice();
  return HomogenusPosition(view) * mat;
}

Vector3 Camera::screenDeviceToDevice(const Vector3& view) const
{
  Matrix4x4 mat = getScreenDeviceToDevice();
  return HomogenusPosition(view) * mat;
}

//Compound Vector transforms

Vector3 Camera::modelToDevice(const Vector3& model) const
{
  Matrix4x4 mat = getModelToDevice();
  return HomogenusPosition(model) * mat;
}

Vector3 Camera::deviceToModel(const Vector3& device) const
{
  Matrix4x4 mat = getDeviceToModel();
  return HomogenusPosition(device) * mat;
}

Vector3 Camera::modelToScreen(const Vector3& model) const
{
  Matrix4x4 mat = getModelToScreen();
  return HomogenusPosition(model) * mat;
}

Vector3 Camera::screenToModel(const Vector3& screen) const
{
  Matrix4x4 mat = getScreenToModel();
  return HomogenusPosition(screen) * mat;
}

Vector3 Camera::deviceToScreen(const Vector3& device) const
{
  Matrix4x4 mat = getDeviceToScreen();
  return mat * HomogenusPosition(device);
}

Vector3 Camera::screenToDevice(const Vector3& device) const
{
  Matrix4x4 mat = getScreenToDevice();
  return HomogenusPosition(device) * mat;
}

Vector3 Camera::viewToScreen(const Vector3& view) const
{
  Matrix4x4 mat = getViewToScreen();
  return HomogenusPosition(view) * mat;
}

Vector3 Camera::screenToView(const Vector3& device) const
{
  Matrix4x4 mat = getScreenToView();
  return HomogenusPosition(device) * mat;
}

void Camera::appendOrbit(const Orientation& orientation)
{
  m_Transform += orientation;
}

Vector3 Camera::getOriginInViewCoords() const
{
  return modelToView(Vector3::Zero());
}

Vector3 Camera::screenToViewAtViewDistance(const Vector2& screenCoord, 
					   double viewZDistance) const
{
  //convert the view distance to display coordinates
  double displayZ = viewToDevice(Vector3(0, 0, viewZDistance)).getZ();
	
  return screenToView(Vector3(screenCoord.getX(), screenCoord.getY(), displayZ));
}

string Camera::getName()
{
  return m_Name;
}

void Camera::setName(string name)
{
  m_Name = name; 
}

void Camera::setScreenDim(const Vector2& screenDim)
{
  m_ScreenDim = screenDim;
}

const Vector2& Camera::getScreenDim() const
{
  return m_ScreenDim;
}

void Camera::setTransform(const Transform& transform)
{
  m_Transform = transform;
}

const Transform& Camera::getTransform() const
{
  return m_Transform;
}

void Camera::setProjectionMatrix(const ProjectionMatrix& projectionMatrix)
{
  m_ProjectionMatrix = projectionMatrix;
}

const ProjectionMatrix& Camera::getProjectionMatrix() const
{
  return m_ProjectionMatrix;
}


















