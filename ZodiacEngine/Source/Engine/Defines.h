#pragma once

#if defined(_MSC_VER_) && defined(_DEBUG)
    #define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) //For detecting memory leaks
#endif
