#include <iostream>
#include <gflags/gflags.h>
#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include "network/cpp/uranium.pb.h"
#include "network/cpp/uranium.grpc.pb.h"

DEFINE_string(host, "localhost:10000", "the uranium server address");
DEFINE_string(action, "", "action you want to do");
DEFINE_string(table, "", "the table name");
DEFINE_string(key, "", "the key");
DEFINE_string(value, "", "the value of the specified key");

int main(int argc, char** argv) {
  gflags::SetUsageMessage("Usage: ./kv_client ");
  gflags::SetVersionString("0.0.1");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::cout << "----------- arguments -----------" << std::endl;
  std::cout << "host: " << FLAGS_host << std::endl;
  std::cout << "action: " << FLAGS_action << std::endl;
  std::cout << "table: " << FLAGS_table << std::endl;
  std::cout << "key: " << FLAGS_key << std::endl;
  std::cout << "value: " << FLAGS_value << std::endl;
  std::cout << "----------- arguments -----------" << std::endl;
  std::shared_ptr<grpc::Channel> channel =
    grpc::CreateChannel(FLAGS_host, grpc::InsecureChannelCredentials());
  std::unique_ptr<uranium::api::UraniumKVService::Stub> stub =
    uranium::api::UraniumKVService::NewStub(channel);
  if (FLAGS_action == "get") {
    uranium::api::KVGetRequest request;
    request.mutable_table_name()->mutable_name()->append(FLAGS_table);
    request.add_keys()->mutable_key()->append(FLAGS_key);
    grpc::ClientContext context;
    uranium::api::KVGetResponse response;
    grpc::Status s = stub->KVGet(&context, request, &response);
    if (!s.ok()) {
      std::cout << "grpc call failed!" << std::endl;
      gflags::ShutDownCommandLineFlags();
      return -1;
    }
    if (response.result().status() != uranium::common::Status::OK) {
      std::cout << "ERROR: " << response.result().message() << std::endl;
      gflags::ShutDownCommandLineFlags();
      return -1;
    }
    std::cout << "----------- result -----------" << std::endl;
    for (auto itr = response.kvs().begin(); itr != response.kvs().end(); ++itr) {
      std::cout << "key: " << itr->key().key() << std::endl;
      std::cout << "value: " << itr->value().value() << std::endl;
    }
    std::cout << "----------- result -----------" << std::endl;
  } else if (FLAGS_action == "set") {
    uranium::api::KVSetRequest request;
    request.mutable_table_name()->mutable_name()->append(FLAGS_table);
    uranium::api::KeyValue* kv = request.add_kvs();
    kv->mutable_key()->set_key(FLAGS_key);
    kv->mutable_value()->set_value(FLAGS_value);
    grpc::ClientContext context;
    uranium::api::KVSetResponse response;
    grpc::Status s = stub->KVSet(&context, request, &response);
    if (!s.ok()) {
      std::cout << "grpc call failed!" << std::endl;
      gflags::ShutDownCommandLineFlags();
      return -1;
    }
    if (response.result().status() != uranium::common::Status::OK) {
      std::cout << "ERROR: " << response.result().message() << std::endl;
    }
  } else {
    std::cout << "invalid action: " << FLAGS_action << std::endl;
    gflags::ShutDownCommandLineFlags();
    return -1;
  }
  gflags::ShutDownCommandLineFlags();
  return 0;
}
