// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
#ifndef URANIUM_SERVER_URANIUM_ADMIN_SERVICE_IMPL_H_
#define URANIUM_SERVER_URANIUM_ADMIN_SERVICE_IMPL_H_

#include <memory>

#include "common/status.h"
#include "network/cpp/common.pb.h"
#include "network/cpp/uranium_admin.grpc.pb.h"

namespace uranium {

class TableManager;

class UraniumAdminServiceImpl : public admin::UraniumAdminService::Service {
 public:
  UraniumAdminServiceImpl() {}
  virtual ~UraniumAdminServiceImpl() {}

  UraniumAdminServiceImpl(const UraniumAdminServiceImpl&) = delete;
  UraniumAdminServiceImpl& operator=(const UraniumAdminServiceImpl&) = delete;

  Status Init(std::shared_ptr<TableManager> table_manager) {
    assert(table_manager.get());
    table_manager_ = table_manager;
    return Status::OK();
  }

  virtual grpc::Status CreateTable(grpc::ServerContext* context,
                                   const admin::TableOptions* request,
                                   common::Result* response) override;

  virtual grpc::Status UpdateTable(grpc::ServerContext* context,
                                   const admin::TableOptions* request,
                                   common::Result* response) override;

  virtual grpc::Status DropTable(grpc::ServerContext* context,
                                 const admin::DropTableRequest* request,
                                 common::Result* response) override;

  virtual grpc::Status GetTableOptions(
      grpc::ServerContext* context,
      const admin::GetTableOptionsRequest* request,
      admin::GetTableOptionsResponse* response) override;

 private:
  std::shared_ptr<TableManager> table_manager_;
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUM_ADMIN_SERVICE_IMPL_H_
