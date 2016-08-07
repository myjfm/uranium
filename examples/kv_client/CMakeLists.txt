add_executable(kv_client kv_client.cc)
target_link_libraries(kv_client ${GFLAGS} network common)
