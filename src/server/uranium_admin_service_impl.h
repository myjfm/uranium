#ifndef URANIUM_SERVER_URANIUM_ADMIN_SERVICE_IMPL_H_
#define URANIUM_SERVER_URANIUM_ADMIN_SERVICE_IMPL_H_

#include "common/status.h"
#include "network/cpp/common.pb.h"
#include "network/cpp/uranium_admin.grpc.pb.h"

namespace uranium {

class UraniumAdminServiceImpl : public admin::UraniumAdminService::Service {
 public:
  UraniumAdminServiceImpl() {}
  virtual ~UraniumAdminServiceImpl() {}
  Status Init() { return Status::OK(); }

  virtual grpc::Status CreateTable(grpc::ServerContext* context,
                                   const admin::TableOptions* request,
                                   common::Result* response) {
    return grpc::Status::OK;
  }
  virtual grpc::Status UpdateTable(grpc::ServerContext* context,
                                   const admin::TableOptions* request,
                                   common::Result* response) {
    return grpc::Status::OK;
  }
  virtual grpc::Status DropTable(grpc::ServerContext* context,
                                 const common::TableName* request,
                                 common::Result* response) {
    return grpc::Status::OK;
  }
  virtual grpc::Status GetTableOptions(grpc::ServerContext* context,
                                       const common::TableName* request,
                                       admin::TableOptions* response) {
    return grpc::Status::OK;
  }
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUM_ADMIN_SERVICE_IMPL_H_

