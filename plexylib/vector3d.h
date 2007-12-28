#ifndef VECTOR_3D
#define VECTOR_3D

#include "plexy.h"

namespace Plexy{


	class  VISIBLE_SYM Vector3D 
	{
	public:
		
		Vector3D() {		//Default constructor. Creates Null Vector
			x = 0;
			y = 0;
			z = 0;
		}
	
		Vector3D(float a, float b, float c) {		//Constructor to create vector with componenets a,b,c
			x = a;
			y = b;
			z = c;
		}

		Vector3D add (Vector3D) ;

		Vector3D substract(Vector3D);
	
		double magnitude();

		double dot(Vector3D);
	
		Vector3D cross(Vector3D);

		Vector3D unitVector3D();

                Vector3D operator-(const Vector3D& vector) const
                {
                    return Vector3D(x-vector.x,y-vector.y,z-vector.z);
                }

                
	private:
		float x;		//x component
		float y;		//y component
		float z;		//z component
	
	};// End Class Vector3D

}

#endif
