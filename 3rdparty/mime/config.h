#ifndef MIMECONFIG_H__
#define MIMECONFIG_H__

#if defined(_WIN32)
  #if defined(mimetype_EXPORTS)
    #define  MIME_EXPORT __declspec(dllexport)
  #else
    #define  MIME_EXPORT __declspec(dllimport)
  #endif
#else
    #ifdef __SYSTEM_HAVE_GCC_VISIBILITY
        #define HIDDEN_SYM __attribute__ ((visibility("hidden")))
        #define VISIBLE_SYM __attribute__ ((visibility("default")))
        #define MIME_EXPORT __attribute__ ((visibility("default")))
    #endif
#endif

#endif //MIMECONFIG_H__
