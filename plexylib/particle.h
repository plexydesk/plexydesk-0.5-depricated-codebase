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

#ifdef PARTICLE_3D
#define PARTICLE_3D

#include "plexy.h"
#include "vector3d.h"

namespace Plexy{

	class VISIBLE_SYM Particle {

	public:

		Particle() {		// Default Constructor
			Vector3D position;
			Vector3D velocity;
			mass =1;
		}

		void update () ;

	private:
		Vector3D position;
		Vector3D velocity;
		float mass;
	} ; // End Class Particle

#endif
