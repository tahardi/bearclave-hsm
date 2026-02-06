#ifndef BEARCLAVE_PKCS11_H
#define BEARCLAVE_PKCS11_H

/* According to pkcs11.h, we must define 5 platform-specific macros:
 *
 * CK_PTR
 * CK_DECLARE_FUNCTION
 * CK_DECLARE_FUNCTION_POINTER
 * CK_CALLBACK_FUNCTION
 * NULL_PTR
 */
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

#include "pkcs11/pkcs11.h"

#endif // BEARCLAVE_PKCS11_H
