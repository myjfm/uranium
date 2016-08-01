#ifndef URANIUM_SERVER_URANIUM_ADMIN_SERVICE_IMPL_H_
#define URANIUM_SERVER_URANIUM_ADMIN_SERVICE_IMPL_H_

#include <memory>

#include "common/status.h"
#include "db/table_manager.h"
#include "network/cpp/common.pb.h"
#include "network/cpp/uranium_admin.grpc.pb.h"

namespace uranium {

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
                                   common::Result* response) override {
    assert(table_manager_.get());
    auto s = table_manager_->CreateTable(*request);
    if (!s.ok()) {
      response->set_status(common::Status::INTERNAL_ERROR);
    } else {
      response->set_status(common::Status::OK);
    }
    return grpc::Status::OK;
  }
  virtual grpc::Status UpdateTable(grpc::ServerContext* context,
                                   const admin::TableOptions* request,
                                   common::Result* response) override {
    assert(table_manager_.get());
    auto s = table_manager_->UpdateTable(*request);
    if (!s.ok()) {
      response->set_status(common::Status::INTERNAL_ERROR);
    } else {
      response->set_status(common::Status::OK);
    }
    return grpc::Status::OK;
  }
  virtual grpc::Status DropTable(grpc::ServerContext* context,
                                 const common::TableName* request,
                                 common::Result* response) override {
    assert(table_manager_.get());
    auto s = table_manager_->DropTable(request->name());
    if (!s.ok()) {
      response->set_status(common::Status::INTERNAL_ERROR);
    } else {
      response->set_status(common::Status::OK);
    }
    return grpc::Status::OK;
  }
  virtual grpc::Status GetTableOptions(
      grpc::ServerContext* context,
      const common::TableName* request,
      admin::GetTableOptionsResponse* response) override {
    assert(table_manager_.get());
    auto s = table_manager_->GetTableOptions(request->name(),
                                             response->mutable_options());
    if (!s.ok()) {
      response->set_status(common::Status::INTERNAL_ERROR);
    } else {
      response->set_status(common::Status::OK);
    }
    return grpc::Status::OK;
  }

 private:
  std::shared_ptr<TableManager> table_manager_;
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUM_ADMIN_SERVICE_IMPL_H_

