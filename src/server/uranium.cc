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
#include "db/table_manager.h"
#include "db/table_manager_impl.h"
#include "network/cpp/uranium.grpc.pb.h"
#include "server/uranium_kv_service_impl.h"
#include "server/uranium_list_service_impl.h"
#include "server/uranium_hash_service_impl.h"
#include "server/uranium_set_service_impl.h"
#include "server/uranium_schema_service_impl.h"
#include "server/uranium_admin_service_impl.h"

DEFINE_int32(port, -1, "What port the uranium to listen on.");
DEFINE_string(db_paths, "", "the directorys all tables are in, quote separated");

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
  gflags::SetUsageMessage("Usage: ./uranium ");
  gflags::SetVersionString("0.0.1");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::string server_address = "localhost:";
  server_address += std::to_string(FLAGS_port);

  auto table_manager =
    std::shared_ptr<uranium::TableManager>(new uranium::TableManagerImpl());
  uranium::Status status = table_manager->Init(FLAGS_db_paths);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return -1;
  }

  uranium::UraniumKVServiceImpl kv_service;
  status = kv_service.Init(table_manager);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return -1;
  }

  uranium::UraniumListServiceImpl list_service;
  status = list_service.Init(table_manager);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return -1;
  }

  uranium::UraniumHashServiceImpl hash_service;
  status = hash_service.Init(table_manager);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return -1;
  }

  uranium::UraniumSetServiceImpl set_service;
  status = set_service.Init(table_manager);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return -1;
  }

  uranium::UraniumSchemaServiceImpl schema_service;
  status = schema_service.Init(table_manager);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return -1;
  }

  uranium::UraniumAdminServiceImpl admin_service;
  status = admin_service.Init(table_manager);
  if (!status.ok()) {
    std::cout << status.ToString() << std::endl;
    return -1;
  }

  grpc::ServerBuilder server_builder;
  server_builder.AddListeningPort(server_address,
                                  grpc::InsecureServerCredentials());
  server_builder.RegisterService(&kv_service);
  server_builder.RegisterService(&list_service);
  server_builder.RegisterService(&hash_service);
  server_builder.RegisterService(&set_service);
  server_builder.RegisterService(&schema_service);
  server_builder.RegisterService(&admin_service);
  std::unique_ptr<grpc::Server> server(server_builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();

  gflags::ShutDownCommandLineFlags();
  return 0;
}

