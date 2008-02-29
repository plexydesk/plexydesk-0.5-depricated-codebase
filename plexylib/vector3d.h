/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/
/*
 * PlexyDesk
 *
 * Sci-Fi Desktop for the Hurd
 *
 * Authored By Siraj Razick <siraj@kde.org>
 * Authored By Mani Shankar <da.sharp.17@gmail.com> 
 *
 * Copyright (C) 2006 PlexyDeskTeam 
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

namespace PlexyDesk {


	class  VISIBLE_SYM Vector3D 
	{
	public:
		
		/**
		 * Initialize the vector class, X , Y , and Z is set to 0.0 by default
		 */
		Vector3D():x(0.0f),y(0.0f),z(0.0f) {}
	
		/**
		 * Create a user defined Vector
		 * @param X X Value of the Vector 
		 * @param Y Y Valule of the Vector
		 * @param Z Z Value of the Vector
		 */
		Vector3D(float X, float Y, float Z):x(X),y(Y),z(Z) {}

		Vector3D add (Vector3D) ;

		Vector3D substract(Vector3D);
	
		double magnitude();

		double dot(Vector3D);
                Vector3D mult(float scalar);
	
		Vector3D cross(Vector3D);

		Vector3D unitVector3D();

                /**
                 * Over loads the - operator there for it lets you subsctract two vectors, as if you are working with 
		 * any pre-defined data type such as a float, example a = b -c , where a , b , and c are vector objects.
                 * @param vector 
                 * @return 
                 */
                Vector3D operator-(const Vector3D& vector) const
                {
                    return Vector3D(x-vector.x,y-vector.y,z-vector.z);
                }

                Vector3D operator+ (const Vector3D& vector) const {
                    return Vector3D(x+vector.x,y+vector.y,z+vector.z);
                }

                
	private:
		float x;		//x component
		float y;		//y component
		float z;		//z component
	
	};// End Class Vector3D

}

#endif
