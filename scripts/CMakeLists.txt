include_directories(${XFIREDB_INCLUDE_DIR} ${XFIREDB_SQLITE_INCLUDE_DIR})

if(${UNIX})
	add_executable (xfiredb-dump
		dump.c)

	target_link_libraries (xfiredb-dump LINK_PUBLIC xfiredbengine)
	install (TARGETS xfiredb-dump
		RUNTIME DESTINATION bin)
endif(${UNIX})

