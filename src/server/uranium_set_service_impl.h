#ifndef URANIUM_SERVER_URANIUM_SET_SERVICE_IMPL_H_
#define URANIUM_SERVER_URANIUM_SET_SERVICE_IMPL_H_

#include <memory>

#include "common/status.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

class TableManager;

class UraniumSetServiceImpl
    : public api::UraniumSetService::Service {
 public:
  UraniumSetServiceImpl() {}
  virtual ~UraniumSetServiceImpl() {}

  UraniumSetServiceImpl(const UraniumSetServiceImpl&) = delete;
  UraniumSetServiceImpl& operator=(UraniumSetServiceImpl&) = delete;

  Status Init(std::shared_ptr<TableManager> table_manager) {
    assert(table_manager.get());
    table_manager_ = table_manager;
    return Status::OK();
  }

  virtual grpc::Status SetAdd(grpc::ServerContext* context,
                              const api::SetAddRequest* request,
                              api::SetAddResponse* response) override;

  virtual grpc::Status SetLength(grpc::ServerContext* context,
                                 const api::SetLengthRequest* request,
                                 api::SetLengthResponse* response) override;

  virtual grpc::Status SetIsMember(
      grpc::ServerContext* context,
      const api::SetIsMemberRequest* request,
      api::SetIsMemberResponse* response) override;

  virtual grpc::Status SetGetAll(grpc::ServerContext* context,
                                 const api::SetGetAllRequest* request,
                                 api::SetGetAllResponse* response) override;

  virtual grpc::Status SetRemove(grpc::ServerContext* context,
                                 const api::SetRemoveRequest* request,
                                 api::SetRemoveResponse* response) override;

  virtual grpc::Status SetRemoveAll(
      grpc::ServerContext* context,
      const api::SetRemoveAllRequest* request,
      api::SetRemoveAllResponse* response) override;

 private:
  std::shared_ptr<TableManager> table_manager_;
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUM_SET_SERVICE_IMPL_H_

