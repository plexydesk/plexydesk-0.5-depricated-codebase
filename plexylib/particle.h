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
