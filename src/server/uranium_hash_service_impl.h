// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#ifndef URANIUM_SERVER_URANIUM_HASH_SERVICE_IMPL_H_
#define URANIUM_SERVER_URANIUM_HASH_SERVICE_IMPL_H_

#include <memory>

#include "common/status.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

class TableManager;

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
                               api::HashGetResponse* response) override;

  virtual grpc::Status HashGetAll(grpc::ServerContext* context,
                                  const api::HashGetAllRequest* request,
                                  api::HashGetAllResponse* response) override;

  virtual grpc::Status HashGetAllFields(
      grpc::ServerContext* context,
      const api::HashGetAllFieldsRequest* request,
      api::HashGetAllFieldsResponse* response) override;

  virtual grpc::Status HashGetAllValues(
      grpc::ServerContext* context,
      const api::HashGetAllValuesRequest* request,
      api::HashGetAllValuesResponse* response) override;

  virtual grpc::Status HashLength(grpc::ServerContext* context,
                                  const api::HashLengthRequest* request,
                                  api::HashLengthResponse* response) override;

  virtual grpc::Status HashSet(grpc::ServerContext* context,
                               const api::HashSetRequest* request,
                               api::HashSetResponse* response) override;

  virtual grpc::Status HashSetX(grpc::ServerContext* context,
                                const api::HashSetXRequest* request,
                                api::HashSetXResponse* response) override;

  virtual grpc::Status HashRemove(grpc::ServerContext* context,
                                  const api::HashRemoveRequest* request,
                                  api::HashRemoveResponse* response) override;

  virtual grpc::Status HashExists(grpc::ServerContext* context,
                                  const api::HashExistsRequest* request,
                                  api::HashExistsResponse* response) override;

  virtual grpc::Status HashRemoveAll(
      grpc::ServerContext* context,
      const api::HashRemoveAllRequest* request,
      api::HashRemoveAllResponse* response) override;

 private:
  std::shared_ptr<TableManager> table_manager_;
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUM_HASH_SERVICE_IMPL_H_

