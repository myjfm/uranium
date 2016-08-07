add_executable(list_client list_client.cc)
target_link_libraries(list_client ${GFLAGS} network common)
