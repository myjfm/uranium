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
    return grpc::Status::OK;
  }
  virtual grpc::Status UpdateTable(grpc::ServerContext* context,
                                   const admin::TableOptions* request,
                                   common::Result* response) override {
    return grpc::Status::OK;
  }
  virtual grpc::Status DropTable(grpc::ServerContext* context,
                                 const common::TableName* request,
                                 common::Result* response) override {
    return grpc::Status::OK;
  }
  virtual grpc::Status GetTableOptions(grpc::ServerContext* context,
                                       const common::TableName* request,
                                       admin::TableOptions* response) override {
    return grpc::Status::OK;
  }

 private:
  std::shared_ptr<TableManager> table_manager_;
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUM_ADMIN_SERVICE_IMPL_H_

