set(CXX_COMPILE_FLAGS
	"-Wall -O2 -std=gnu++98")
set(C_COMPILE_FLAGS
	"-Wall -O2 -std=gnu89 -lm")

if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	set(C_COMPILE_FLAGS "${C_COMPILE_FLAGS} -pthread")
	set(CXX_COMPILE_FLAGS "${CXX_COMPILE_FLAGS} -pthread")
endif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

set(DEBUG_FLAGS "")
if(XFIRE_DEBUG)
	set(DEBUG_FLAGS -g)
endif(XFIRE_DEBUG)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_COMPILE_FLAGS} ${DEBUG_FLAGS}")
