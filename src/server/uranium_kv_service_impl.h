#ifndef URANIUM_SERVER_URANIUM_KV_SERVICE_IMPL_H_
#define URANIUM_SERVER_URANIUM_KV_SERVICE_IMPL_H_

#include <memory>

#include "common/status.h"
#include "db/table_manager.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

class UraniumKVServiceImpl
    : public api::UraniumKVService::Service {
 public:
  UraniumKVServiceImpl() {}
  virtual ~UraniumKVServiceImpl() {}

  UraniumKVServiceImpl(const UraniumKVServiceImpl&) = delete;
  UraniumKVServiceImpl& operator=(UraniumKVServiceImpl&) = delete;

  Status Init(std::shared_ptr<TableManager> table_manager) {
    assert(table_manager.get());
    table_manager_ = table_manager;
    return Status::OK();
  }

  virtual grpc::Status KVGet(grpc::ServerContext* context,
                             const api::KVGetRequest* request,
                             api::KVGetResponse* response) override {
    auto table =
        table_manager_->GetKVTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->Get(request->keys(), response->mutable_kvs());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

  virtual grpc::Status KVSet(grpc::ServerContext* context,
                             const api::KVSetRequest* request,
                             api::KVSetResponse* response) override {
    auto table =
        table_manager_->GetKVTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->Set(request->kvs());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

  virtual grpc::Status KVRemove(grpc::ServerContext* context,
                                const api::KVRemoveRequest* request,
                                api::KVRemoveResponse* response) override {
    auto table =
        table_manager_->GetKVTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->Remove(request->keys());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

 private:
  std::shared_ptr<TableManager> table_manager_;
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUM_KV_SERVICE_IMPL_H_

