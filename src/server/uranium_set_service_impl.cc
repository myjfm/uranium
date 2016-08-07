// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
#include "uranium_set_service_impl.h"
#include <memory>

#include "common/status.h"
#include "db/table_manager.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

grpc::Status UraniumSetServiceImpl::SetAdd(grpc::ServerContext* context,
                                           const api::SetAddRequest* request,
                                           api::SetAddResponse* response) {
  auto table = table_manager_->GetSetTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    auto s = table->Add(request->key().key(), request->values());
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumSetServiceImpl::SetLength(
    grpc::ServerContext* context,
    const api::SetLengthRequest* request,
    api::SetLengthResponse* response) {
  auto table = table_manager_->GetSetTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    int64_t length;
    auto s = table->Length(request->key().key(), &length);
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumSetServiceImpl::SetIsMember(
    grpc::ServerContext* context,
    const api::SetIsMemberRequest* request,
    api::SetIsMemberResponse* response) {
  auto table = table_manager_->GetSetTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    bool yes;
    auto s = table->IsMember(request->key().key(),
                             request->value().value(),
                             &yes);
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumSetServiceImpl::SetGetAll(
    grpc::ServerContext* context,
    const api::SetGetAllRequest* request,
    api::SetGetAllResponse* response) {
  auto table = table_manager_->GetSetTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    bool yes;
    auto s = table->GetAll(request->key().key(), response->mutable_values());
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumSetServiceImpl::SetRemove(
    grpc::ServerContext* context,
    const api::SetRemoveRequest* request,
    api::SetRemoveResponse* response) {
  auto table = table_manager_->GetSetTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    bool yes;
    auto s = table->Remove(request->key().key(), request->values());
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

grpc::Status UraniumSetServiceImpl::SetRemoveAll(
    grpc::ServerContext* context,
    const api::SetRemoveAllRequest* request,
    api::SetRemoveAllResponse* response) {
  auto table = table_manager_->GetSetTable(request->table_name().name());
  if (!table.get()) {
    Status::TABLE_NOT_FOUND(response->mutable_result());
  } else {
    bool yes;
    auto s = table->RemoveAll(request->key().key());
    Status::RESULT(s, response->mutable_result());
  }
  return grpc::Status::OK;
}

}  // namespace uranium
