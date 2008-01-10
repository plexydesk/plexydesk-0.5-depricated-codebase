/*
 * PlexyDesk
 *
 * Sci-Fi Desktop for the Hurd
 *
 * Authored By Siraj Razick <siraj@kde.org>
 * Authored By Mani Shankar <da.sharp.17@gmail.com> 
 *
 * Copyright (C) 2008 PlexyDeskTeam
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

        Vector3D Vector3D::mult(float scalar) {         //multiplication with a scalar
            x = x*scalar;
            y = y*scalar;
            z = z*scalar;

            return Vector3D(x,y,z);
        } //end mult


} //namespace Plexy
