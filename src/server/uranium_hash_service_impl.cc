// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#include "uranium_hash_service_impl.h"

#include <memory>

#include "common/status.h"
#include "db/table_manager.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

grpc::Status UraniumHashServiceImpl::HashGet(grpc::ServerContext* context,
                                             const api::HashGetRequest* request,
                                             api::HashGetResponse* response) {
  auto table = table_manager_->GetHashTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    auto s = table->Get(request->key().key(),
                        request->fields(),
                        response->mutable_fvs());
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashGetAll(
    grpc::ServerContext* context,
    const api::HashGetAllRequest* request,
    api::HashGetAllResponse* response) {
  auto table = table_manager_->GetHashTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    auto s = table->GetAll(request->key().key(), response->mutable_fvs());
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashGetAllFields(
    grpc::ServerContext* context,
    const api::HashGetAllFieldsRequest* request,
    api::HashGetAllFieldsResponse* response) {
  auto table = table_manager_->GetHashTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    auto s = table->GetAllFields(request->key().key(),
                                 response->mutable_fields());
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashGetAllValues(
    grpc::ServerContext* context,
    const api::HashGetAllValuesRequest* request,
    api::HashGetAllValuesResponse* response) {
  auto table = table_manager_->GetHashTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    auto s = table->GetAllValues(request->key().key(),
                                 response->mutable_values());
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashLength(
    grpc::ServerContext* context,
    const api::HashLengthRequest* request,
    api::HashLengthResponse* response) {
  auto table = table_manager_->GetHashTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    int64_t length;
    auto s = table->Length(request->key().key(), &length);
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashSet(
    grpc::ServerContext* context,
    const api::HashSetRequest* request,
    api::HashSetResponse* response) {
  auto table = table_manager_->GetHashTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    int64_t length;
    auto s = table->Set(request->key().key(), request->fvs());
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashSetX(
    grpc::ServerContext* context,
    const api::HashSetXRequest* request,
    api::HashSetXResponse* response) {
  auto table = table_manager_->GetHashTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    int64_t length;
    auto s = table->SetX(request->key().key(), request->fv());
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashRemove(
    grpc::ServerContext* context,
    const api::HashRemoveRequest* request,
    api::HashRemoveResponse* response) {
  auto table = table_manager_->GetHashTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    int64_t length;
    auto s = table->Remove(request->key().key(), request->fields());
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashExists(
    grpc::ServerContext* context,
    const api::HashExistsRequest* request,
    api::HashExistsResponse* response) {
  auto table = table_manager_->GetHashTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    bool exists;
    auto s = table->Exists(request->key().key(), request->field(), &exists);
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashRemoveAll(
    grpc::ServerContext* context,
    const api::HashRemoveAllRequest* request,
    api::HashRemoveAllResponse* response) {
  auto table = table_manager_->GetHashTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    int64_t length;
    auto s = table->RemoveAll(request->key().key());
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

}  // namespace uranium
