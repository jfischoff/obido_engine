#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"
#include <ProjectionMatrix.h>
#include <Vector2.h>
#include <Vector3.h>
#include <Ray.h>
#include <iostream>

using namespace std;

//Okay this camera class is kind of screwy
//a few things
//I think the view matrix is unnecessary
//I think it is part of the projection matrix
//two, the compound transform should be just that
//right now there is duplication
//all inverses should use the actual inverse transforms...for now



class Camera
{
 public:
  static void* Constructor();
  Camera();
  Camera(const ProjectionMatrix& projectionMatrix, 
	 const Transform& transform, const Vector2& screenDim);
  Camera(const Camera& camera);
  Camera& operator=(const Camera& camera);
  void copy(const Camera& camera);

  virtual void deepCopy(const Camera* other);
  virtual void destroy();

  Matrix4x4 getMV() const;
  Matrix4x4 getMVP() const;

  //I think that I should change the camera to
  //encapsulate the projection 
  //or for now the scene can do this

  ProjectionMatrix& P();
  const ProjectionMatrix& PC() const;
	
  Transform& T();
  const Transform& TC() const;
  void setView(int width, int height);


  Matrix4x4 makeViewMatrix() const;
  Matrix4x4 makeInverseViewMatrix() const;

  Matrix4x4 getLocalMatrix();
  Matrix4x4 getInverseLocalMatrix();	

  Vector3 screenToWorld(const Vector2& screenPos) const;	
  void screenToWorld(float screenX, float screenY,
		     float& worldX, float& worldY, float& worldZ) const;
  void screenToWorld(const Vector2& screenPos, 
		     Vector3& worldPos) const;

  void Dolly(double offset);
	
  void appendOrbit(const Orientation& orientation);
	

  void viewToWorld(Vector3 viewVector, Vector3& worldVector) const;
	       
  Vector3 screenToDevice(const Vector2& screenPos) const;

  void appendPivot(const Vector3& trans);
  void appendOffset(const Vector3& trans);
  void appendRotation(const AngleAxis& angleAxis);
	
  void appendCenter(const Vector3& trans);
	
  void getRay(const Vector2& screenPos, Ray& ray) const;
  void getDeviceRay(const Vector2& screenPos, Ray& ray, double z=-1) const;
  Ray getViewRay(const Vector2& screenPos, double z=-1) const;
  Ray getModelRay(const Vector2& screenPos, double z=-1) const;
	
	
  float deviceToWorldZ(float deviceZ) const;
	
  //Json::Value toJson() const;
  //  void fromJson(Json::Value value);
	
  Float getHeight() const;
  Float getWidth() const;
	
  Vector3 getPosition() const;
	
  //friend ostream& operator<<(ostream& cout, const Camera& camera);
	
  Matrix4x4 getModelToView() const;
  Matrix4x4 getViewToModel() const;

  Matrix4x4 getViewToDevice() const;
  Matrix4x4 getDeviceToView() const;
	
  Matrix4x4 getDeviceToScreenDevice() const;
  Matrix4x4 getScreenDeviceToDevice() const;
  Matrix4x4 getScreenDeviceToScreen() const;
	
  Matrix4x4 getDeviceToScreen() const;
  Matrix4x4 getScreenToDevice() const;
	
  //Compound
  Matrix4x4 getModelToDevice() const;
  Matrix4x4 getDeviceToModel()const;
	
  Matrix4x4 getModelToScreen() const;
  Matrix4x4 getScreenToModel() const;
	
  Matrix4x4 getViewToScreen() const;
  Matrix4x4 getScreenToView() const;
		
  //Vector Transforms
  Vector3 modelToView(const Vector3& device) const;
  Vector3 viewToModel(const Vector3& device) const;
	
  Vector3 viewToDevice(const Vector3& view) const;
  Vector3 deviceToView(const Vector3& view) const;
	
  Vector3 deviceToScreenDevice(const Vector3& view) const;
  Vector3 screenDeviceToDevice(const Vector3& view) const;
	
  Vector3 deviceToScreen(const Vector3& device) const;
  Vector3 screenToDevice(const Vector3& device) const;
	
  //Compound Vector transforms

  Vector3 modelToDevice(const Vector3& device) const;
  Vector3 deviceToModel(const Vector3& world) const;
	
  Vector3 modelToScreen(const Vector3& model) const;
  Vector3 screenToModel(const Vector3& model) const;
	
  Vector3 viewToScreen(const Vector3& view) const;
  Vector3 screenToView(const Vector3& screen) const;
	
  Vector3 getOriginInViewCoords() const;
  Vector3 screenToViewAtViewDistance(const Vector2& screenCoord, 
				     double viewZDistance) const;
  string getName();
  void setName(string name);

  void setScreenDim(const Vector2& screenDim);
  const Vector2& getScreenDim() const;

  void setTransform(const Transform& transform);
  const Transform& getTransform() const;

  void setProjectionMatrix(const ProjectionMatrix& projectionMatrix);
  const ProjectionMatrix& getProjectionMatrix() const;

  void merge(const Camera& other){}
  void clear(){}

 private:
  string m_Name;
  Vector2 m_ScreenDim;
  Transform m_Transform;

	
  ProjectionMatrix m_ProjectionMatrix;

};

#endif //Camera_H
