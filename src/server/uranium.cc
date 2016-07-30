#include <iostream>
#include <string>
#include <memory>

#include <gflags/gflags.h>
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/security/server_credentials.h>

#include "common/status.h"
#include "network/cpp/uranium.grpc.pb.h"
#include "server/uranium_schema_service_impl.h"
#include "server/uranium_schemaless_service_impl.h"
#include "server/uranium_admin_service_impl.h"

DEFINE_int32(port, -1, "What port the uranium to listen on.");

static bool ValidatePort(const char* flagname, int32_t value) {
  if (value > 0 && value <= 65535) {  // value is ok
    return true;
  }
  std::cout << "Invalid value for --" << flagname << " " << value << std::endl;
  return false;
}

static const bool port_dummy = gflags::RegisterFlagValidator(
    &FLAGS_port,
    &ValidatePort);

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::string server_address = "localhost:";
  server_address += std::to_string(FLAGS_port);

  uranium::UraniumSchemalessServiceImpl schemaless_service;
  uranium::Status status = schemaless_service.Init();
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return -1;
  }

  uranium::UraniumSchemaServiceImpl schema_service;
  status = schema_service.Init();
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return -1;
  }

  uranium::UraniumAdminServiceImpl admin_service;
  status = admin_service.Init();
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return -1;
  }

  grpc::ServerBuilder server_builder;
  server_builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  server_builder.RegisterService(&schemaless_service);
  server_builder.RegisterService(&schema_service);
  server_builder.RegisterService(&admin_service);
  std::unique_ptr<grpc::Server> server(server_builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
  return 0;
}

