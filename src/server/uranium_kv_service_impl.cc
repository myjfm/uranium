#include "uranium_kv_service_impl.h"

#include <memory>

#include "common/status.h"
#include "db/table_manager.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

grpc::Status UraniumKVServiceImpl::KVGet(grpc::ServerContext* context,
                                         const api::KVGetRequest* request,
                                         api::KVGetResponse* response) {
  auto table = table_manager_->GetKVTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->Get(request->keys(), response->mutable_kvs());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

grpc::Status UraniumKVServiceImpl::KVSet(grpc::ServerContext* context,
                                         const api::KVSetRequest* request,
                                         api::KVSetResponse* response) {
  auto table = table_manager_->GetKVTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->Set(request->kvs());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

grpc::Status UraniumKVServiceImpl::KVRemove(grpc::ServerContext* context,
                                            const api::KVRemoveRequest* request,
                                            api::KVRemoveResponse* response) {
  auto table = table_manager_->GetKVTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->Remove(request->keys());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

}  // namespace uranium
