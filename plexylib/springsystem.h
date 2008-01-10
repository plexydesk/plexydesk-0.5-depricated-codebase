#ifndef SPRING_SYSTEM
#define SPRING_SYSTEM
#include "particle.h"
#include "plexy.h"
#include "vector3d.h
"
namespace Plexy
{
    class VISIBLE_SYM SpringSystem
    {
        private:
            /*
             * it owuld be a data structure,
             * Which we haven't yet Implemeted
             * First have to make an algorithm
             * on how to create a network as
             * described in the doc, then will
             * add the DS and the constructor
             */

            float k;

            Particle caught;

        public:

            void dragSystem();
            
            void dragCaught();


    };
} // namespace plexy{

#endif
