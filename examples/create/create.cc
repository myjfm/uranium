#include <iostream>
#include <gflags/gflags.h>
#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include "network/cpp/uranium_admin.pb.h"
#include "network/cpp/uranium_admin.grpc.pb.h"

DEFINE_string(host, "localhost:10000", "the uranium server address");
DEFINE_string(type, "kv", "table type: kv | list | hash | set | schema");

int main(int argc, char** argv) {
  gflags::SetUsageMessage("Usage: ./create --host= --type=");
  gflags::SetVersionString("0.0.1");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::shared_ptr<grpc::Channel> channel =
    grpc::CreateChannel(FLAGS_host, grpc::InsecureChannelCredentials());
  std::unique_ptr<uranium::admin::UraniumAdminService::Stub> stub =
    uranium::admin::UraniumAdminService::NewStub(channel);
  uranium::admin::TableOptions opt;
  if (FLAGS_type == "kv") {
    opt.mutable_table_name()->mutable_name()->append("test_kv_table");
    opt.set_table_type(uranium::common::TableType::KV);
    opt.mutable_kv_table_options()->set_type(uranium::admin::StorageType::FLAT);
  } else if (FLAGS_type == "list") {
    opt.mutable_table_name()->mutable_name()->append("test_list_table");
    opt.set_table_type(uranium::common::TableType::LIST);
    opt.mutable_list_table_options()->set_type(uranium::admin::StorageType::FLAT);
  } else if (FLAGS_type == "hash") {
    opt.mutable_table_name()->mutable_name()->append("test_hash_table");
    opt.set_table_type(uranium::common::TableType::HASH);
    opt.mutable_hash_table_options()->set_type(uranium::admin::StorageType::FLAT);
  } else if (FLAGS_type == "set") {
    opt.mutable_table_name()->mutable_name()->append("test_set_table");
    opt.set_table_type(uranium::common::TableType::SET);
    opt.mutable_set_table_options()->set_type(uranium::admin::StorageType::FLAT);
  } else if (FLAGS_type == "schema") {
    opt.mutable_table_name()->mutable_name()->append("test_schema_table");
    opt.set_table_type(uranium::common::TableType::SCHEMA);
    opt.mutable_schema_table_options()->set_type(uranium::admin::StorageType::FLAT);
  } else {
    std::cerr << "unknown table type: " << FLAGS_type << std::endl;
    gflags::ShutDownCommandLineFlags();
    return -1;
  }

  uranium::admin::CommonTableOptions copt;
  copt.set_num_levels(3);
  opt.mutable_common_table_options()->CopyFrom(copt);
  grpc::ClientContext context;
  uranium::common::Result result;
  grpc::Status s = stub->CreateTable(&context, opt, &result);
  if (!s.ok()) {
    std::cout << "grpc call failed!" << std::endl;
    gflags::ShutDownCommandLineFlags();
    return -1;
  }
  if (result.status() != uranium::common::Status::OK) {
    std::cout << "CreateTable failed: " << result.message() << std::endl;
    gflags::ShutDownCommandLineFlags();
    return -1;
  }
  std::cout << "CreateTable successfully!" << std::endl;
  gflags::ShutDownCommandLineFlags();
  return 0;
}
