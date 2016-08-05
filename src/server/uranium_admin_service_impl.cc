// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#include "uranium_admin_service_impl.h"

#include "common/status.h"
#include "db/table_manager.h"
#include "network/cpp/common.pb.h"
#include "network/cpp/uranium_admin.grpc.pb.h"

namespace uranium {

grpc::Status UraniumAdminServiceImpl::CreateTable(
    grpc::ServerContext* context,
    const admin::TableOptions* request,
    common::Result* response) {
  assert(table_manager_.get());
  Status s;
  switch (request->table_type()) {
    case common::TableType::KV:
      if(!request->has_kv_table_options()) {
        response->set_status(common::Status::INVALID_ARGUMENT);
        response->set_message("kv_table_options not given");
        break;
      }
      s = table_manager_->CreateKVTable(request->table_name().name(),
                                        request->kv_table_options());
      break;
    case common::TableType::LIST:
      if(!request->has_list_table_options()) {
        response->set_status(common::Status::INVALID_ARGUMENT);
        response->set_message("list_table_options not given");
        break;
      }
      s = table_manager_->CreateListTable(request->table_name().name(),
                                          request->list_table_options());
      break;
    case common::TableType::HASH:
      if(!request->has_hash_table_options()) {
        response->set_status(common::Status::INVALID_ARGUMENT);
        response->set_message("hash_table_options not given");
        break;
      }
      s = table_manager_->CreateHashTable(request->table_name().name(),
                                          request->hash_table_options());
      break;
    case common::TableType::SET:
      if(!request->has_set_table_options()) {
        response->set_status(common::Status::INVALID_ARGUMENT);
        response->set_message("set_table_options not given");
        break;
      }
      s = table_manager_->CreateSetTable(request->table_name().name(),
                                         request->set_table_options());
      break;
    case common::TableType::SCHEMA:
      if(!request->has_schema_table_options()) {
        response->set_status(common::Status::INVALID_ARGUMENT);
        response->set_message("schema_table_options not given");
        break;
      }
      s = table_manager_->CreateSchemaTable(request->table_name().name(),
                                            request->schema_table_options());
      break;
    default:
      assert(false);
  }
  Status::RESULT(s, response);
  return grpc::Status::OK;
}

grpc::Status UraniumAdminServiceImpl::UpdateTable(
    grpc::ServerContext* context,
    const admin::TableOptions* request,
    common::Result* response) {
  assert(table_manager_.get());
  Status s;
  switch (request->table_type()) {
    case common::TableType::KV:
      if(!request->has_kv_table_options()) {
        response->set_status(common::Status::INVALID_ARGUMENT);
        response->set_message("kv_table_options not given");
        break;
      }
      s = table_manager_->UpdateKVTable(request->table_name().name(),
                                        request->kv_table_options());
      break;
    case common::TableType::LIST:
      if(!request->has_list_table_options()) {
        response->set_status(common::Status::INVALID_ARGUMENT);
        response->set_message("list_table_options not given");
        break;
      }
      s = table_manager_->UpdateListTable(request->table_name().name(),
                                          request->list_table_options());
      break;
    case common::TableType::HASH:
      if(!request->has_hash_table_options()) {
        response->set_status(common::Status::INVALID_ARGUMENT);
        response->set_message("hash_table_options not given");
        break;
      }
      s = table_manager_->UpdateHashTable(request->table_name().name(),
                                          request->hash_table_options());
      break;
    case common::TableType::SET:
      if(!request->has_set_table_options()) {
        response->set_status(common::Status::INVALID_ARGUMENT);
        response->set_message("set_table_options not given");
        break;
      }
      s = table_manager_->UpdateSetTable(request->table_name().name(),
                                         request->set_table_options());
      break;
    case common::TableType::SCHEMA:
      if(!request->has_schema_table_options()) {
        response->set_status(common::Status::INVALID_ARGUMENT);
        response->set_message("schema_table_options not given");
        break;
      }
      s = table_manager_->UpdateSchemaTable(request->table_name().name(),
                                            request->schema_table_options());
      break;
    default:
      assert(false);
  }
  Status::RESULT(s, response);
  return grpc::Status::OK;
}

grpc::Status UraniumAdminServiceImpl::DropTable(
    grpc::ServerContext* context,
    const admin::DropTableRequest* request,
    common::Result* response) {
  assert(table_manager_.get());
  Status s;
  switch (request->table_type()) {
    case common::TableType::KV:
      s = table_manager_->DropKVTable(request->table_name().name());
      break;
    case common::TableType::LIST:
      s = table_manager_->DropListTable(request->table_name().name());
      break;
    case common::TableType::HASH:
      s = table_manager_->DropHashTable(request->table_name().name());
      break;
    case common::TableType::SET:
      s = table_manager_->DropSetTable(request->table_name().name());
      break;
    case common::TableType::SCHEMA:
      s = table_manager_->DropSchemaTable(request->table_name().name());
      break;
    default:
      assert(false);
      break;
  }
  Status::RESULT(s, response);
  return grpc::Status::OK;
}

grpc::Status UraniumAdminServiceImpl::GetTableOptions(
    grpc::ServerContext* context,
    const admin::GetTableOptionsRequest* request,
    admin::GetTableOptionsResponse* response) {
  assert(table_manager_.get());
  Status s;
  switch (request->table_type()) {
    case common::TableType::KV:
      s = table_manager_->GetKVTableOptions(
          request->table_name().name(),
          response->mutable_options()->mutable_kv_table_options());
      break;
    case common::TableType::LIST:
      s = table_manager_->GetListTableOptions(
          request->table_name().name(),
          response->mutable_options()->mutable_list_table_options());
      break;
    case common::TableType::HASH:
      s = table_manager_->GetHashTableOptions(
          request->table_name().name(),
          response->mutable_options()->mutable_hash_table_options());
      break;
    case common::TableType::SET:
      s = table_manager_->GetSetTableOptions(
          request->table_name().name(),
          response->mutable_options()->mutable_set_table_options());
      break;
    case common::TableType::SCHEMA:
      s = table_manager_->GetSchemaTableOptions(
          request->table_name().name(),
          response->mutable_options()->mutable_schema_table_options());
      break;
    default:
      assert(false);
      break;
  }
  Status::RESULT(s, response->mutable_result());
  return grpc::Status::OK;
}

}  // namespace uranium
