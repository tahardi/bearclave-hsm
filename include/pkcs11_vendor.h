/* include/pkcs11_vendor.h */
#ifndef BEARSM_PKCS11_VENDOR_H
#define BEARSM_PKCS11_VENDOR_H

/* Minimal “typical UNIX” PKCS#11 platform macros */
#ifndef CK_PTR
#define CK_PTR *
#endif

#ifndef CK_DECLARE_FUNCTION
#define CK_DECLARE_FUNCTION(returnType, name) returnType(name)
#endif

#ifndef CK_DECLARE_FUNCTION_POINTER
#define CK_DECLARE_FUNCTION_POINTER(returnType, name) returnType(*(name))
#endif

#ifndef CK_CALLBACK_FUNCTION
#define CK_CALLBACK_FUNCTION(returnType, name) returnType(*(name))
#endif

#ifndef NULL_PTR
#define NULL_PTR 0
#endif

#include "pkcs11.h"

#endif /* BEARSM_PKCS11_VENDOR_H */
