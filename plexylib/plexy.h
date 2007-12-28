
/* visibility options
 * note only support GCC 4.x and not Windows 
 * */

#ifndef __PLEXY_EXPORTS
#define __PLEXY_EXPORTS

#ifdef __GNUC_
#define PACK __attribute__((__packed__))
#endif 

#define __SYSTEM_HAVE_GCC_VISIBILITY

#ifdef __SYSTEM_HAVE_GCC_VISIBILITY
#define HIDDEN_SYM __attribute__ ((visibility("hidden")))
#define VISIBLE_SYM __attribute__ ((visibility("default")))
#endif

//TODO
//add dllexport for windows


namespace Plexy
{



}


#endif
