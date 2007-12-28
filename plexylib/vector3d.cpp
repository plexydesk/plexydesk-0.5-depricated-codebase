#include <math.h>
#include "vector3d.h"
namespace Plexy
{
	Vector3D Vector3D::add (Vector3D v) {		//adds v to "this"
		Vector3D result;
		result.x = x+v.x;
		result.y = y+v.y;
		result.z = z+v.z;

		return result;
	} //End add

	Vector3D Vector3D::substract(Vector3D v){		//subtracts v from "this"
		Vector3D result;
		result.x = x-v.x;
		result.y = y-v.y;
		result.z = z-v.z;

		return result;
	} //End Substract

	double Vector3D::magnitude(){		//returns Magnitune of Vector
		return sqrt(x*x+y*y+z*z);
	} //End magnitude

	double Vector3D::dot(Vector3D v){		//gives dot product with v
		return (x*v.x+y*v.y+z*v.z);
	} //End dot
	
	Vector3D Vector3D::cross(Vector3D v){		//gives cross product with v
		Vector3D result;

		result.x = y*v.z-z*v.y;
		result.y = v.x*z-x*v.z;
		result.z = x*v.y-y*v.x;

		return result;
	} //End cross

	Vector3D Vector3D::unitVector3D() {				//gives unit vector in the direction of "this"
		Vector3D result;
		result.x = x/this->magnitude();
		result.y = y/this->magnitude();
		result.z = z/this->magnitude();
		
		return result;
	} //End unitVector3D

} //namespace Plexy
