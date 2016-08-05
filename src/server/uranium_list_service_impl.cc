#include "uranium_list_service_impl.h"

#include <memory>

#include "common/status.h"
#include "db/table_manager.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

grpc::Status UraniumListServiceImpl::ListLPush(
    grpc::ServerContext* context,
    const api::ListLPushRequest* request,
    api::ListLPushResponse* response) {
  auto table = table_manager_->GetListTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->LPush(request->key().key(), request->values());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

grpc::Status UraniumListServiceImpl::ListLPop(
    grpc::ServerContext* context,
    const api::ListLPopRequest* request,
    api::ListLPopResponse* response) {
  auto table = table_manager_->GetListTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->LPop(request->key().key(), response->mutable_value());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

grpc::Status UraniumListServiceImpl::ListRPush(
    grpc::ServerContext* context,
    const api::ListRPushRequest* request,
    api::ListRPushResponse* response) {
  auto table = table_manager_->GetListTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->RPush(request->key().key(), request->values());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

grpc::Status UraniumListServiceImpl::ListRPop(
    grpc::ServerContext* context,
    const api::ListRPopRequest* request,
    api::ListRPopResponse* response) {
  auto table = table_manager_->GetListTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->RPop(request->key().key(), response->mutable_value());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

grpc::Status UraniumListServiceImpl::ListLPushX(
    grpc::ServerContext* context,
    const api::ListLPushXRequest* request,
    api::ListLPushXResponse* response) {
  auto table = table_manager_->GetListTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->LPushX(request->key().key(), request->values());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

grpc::Status UraniumListServiceImpl::ListRPushX(
    grpc::ServerContext* context,
    const api::ListRPushXRequest* request,
    api::ListRPushXResponse* response) {
  auto table = table_manager_->GetListTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->RPushX(request->key().key(), request->values());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

grpc::Status UraniumListServiceImpl::ListIndex(
    grpc::ServerContext* context,
    const api::ListIndexRequest* request,
    api::ListIndexResponse* response) {
  auto table = table_manager_->GetListTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->Index(request->key().key(),
                          request->index(),
                          response->mutable_value());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

grpc::Status UraniumListServiceImpl::ListSet(
    grpc::ServerContext* context,
    const api::ListSetRequest* request,
    api::ListSetResponse* response) {
  auto table = table_manager_->GetListTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->Set(request->key().key(),
                        request->index(),
                        request->value().value());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

grpc::Status UraniumListServiceImpl::ListRange(
    grpc::ServerContext* context,
    const api::ListRangeRequest* request,
    api::ListRangeResponse* response) {
  auto table = table_manager_->GetListTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->Range(request->key().key(),
                          request->start(),
                          request->end(),
                          response->mutable_values());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

grpc::Status UraniumListServiceImpl::ListLength(
    grpc::ServerContext* context,
    const api::ListLengthRequest* request,
    api::ListLengthResponse* response) {
  auto table =
      table_manager_->GetListTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    int64_t length = 0;
    auto s = table->Length(request->key().key(), &length);
    if (s.ok()) {
      response->set_length(length);
    }
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

grpc::Status UraniumListServiceImpl::ListRemoveAll(
    grpc::ServerContext* context,
    const api::ListRemoveAllRequest* request,
    api::ListRemoveAllResponse* response) {
  auto table = table_manager_->GetListTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
    return grpc::Status::OK;
  } else {
    auto s = table->RemoveAll(request->key().key());
    Status::RESULT(s, response->mutable_result());
    return grpc::Status::OK;
  }
}

}  // namespace uranium
