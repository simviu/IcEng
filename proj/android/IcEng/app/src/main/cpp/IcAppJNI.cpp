#include "MyTestApp.h"
#include "IcEngJNI.h"

//---- The IMPLEMENT_ICAPP macro is simply an implementation of
// JNI function for IcJNI.createIcApp(), in JNI format.
// Just pass your IcApp derieved class as para of this macro.
IMPLEMENT_ICAPP(MyTestApp)
