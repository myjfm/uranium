#ifndef URANIUM_SERVER_URANIUM_SCHEMA_SERVICE_IMPL_H_
#define URANIUM_SERVER_URANIUM_SCHEMA_SERVICE_IMPL_H_

#include "common/status.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

class UraniumSchemaServiceImpl
    : public api::UraniumSchemaService::Service {
 public:
  UraniumSchemaServiceImpl() {}
  virtual ~UraniumSchemaServiceImpl() {}
  Status Init() { return Status::OK(); }
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUM_SCHEMA_SERVICE_IMPL_H_

