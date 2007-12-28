/*
 * PlexyDesk
 *
 * Sci-Fi Desktop for the Hurd
 *
 * Authored By Siraj Razick <siraj@kde.org>
 * Authored By Venkata Mani <da.sharp@gmail.com>
 *
 * Copyright (C) 2006 OpenedHand
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

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
