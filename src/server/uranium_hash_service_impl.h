#ifndef URANIUM_SERVER_URANIUM_HASH_SERVICE_IMPL_H_
#define URANIUM_SERVER_URANIUM_HASH_SERVICE_IMPL_H_

#include <memory>

#include "common/status.h"
#include "db/table_manager.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

class UraniumHashServiceImpl
    : public api::UraniumHashService::Service {
 public:
  UraniumHashServiceImpl() {}
  virtual ~UraniumHashServiceImpl() {}

  UraniumHashServiceImpl(const UraniumHashServiceImpl&) = delete;
  UraniumHashServiceImpl& operator=(UraniumHashServiceImpl&) = delete;

  Status Init(std::shared_ptr<TableManager> table_manager) {
    assert(table_manager.get());
    table_manager_ = table_manager;
    return Status::OK();
  }

  virtual grpc::Status HashGet(grpc::ServerContext* context,
                               const api::HashGetRequest* request,
                               api::HashGetResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashGetAll(grpc::ServerContext* context,
                                  const api::HashGetAllRequest* request,
                                  api::HashGetAllResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashGetAllFields(
      grpc::ServerContext* context,
      const api::HashGetAllFieldsRequest* request,
      api::HashGetAllFieldsResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashGetAllValues(
      grpc::ServerContext* context,
      const api::HashGetAllValuesRequest* request,
      api::HashGetAllValuesResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashLength(grpc::ServerContext* context,
                                  const api::HashLengthRequest* request,
                                  api::HashLengthResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashSet(grpc::ServerContext* context,
                               const api::HashSetRequest* request,
                               api::HashSetResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashSetX(grpc::ServerContext* context,
                                const api::HashSetXRequest* request,
                                api::HashSetXResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashRemove(grpc::ServerContext* context,
                                  const api::HashRemoveRequest* request,
                                  api::HashRemoveResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashExists(grpc::ServerContext* context,
                                  const api::HashExistsRequest* request,
                                  api::HashExistsResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashRemoveAll(
      grpc::ServerContext* context,
      const api::HashRemoveAllRequest* request,
      api::HashRemoveAllResponse* response) override {
    return grpc::Status::OK;
  }

 private:
  std::shared_ptr<TableManager> table_manager_;
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUM_HASH_SERVICE_IMPL_H_
