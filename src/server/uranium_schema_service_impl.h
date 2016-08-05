#ifndef URANIUM_SERVER_URANIUM_SCHEMA_SERVICE_IMPL_H_
#define URANIUM_SERVER_URANIUM_SCHEMA_SERVICE_IMPL_H_

#include <memory>
#include "common/status.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

class TableManager;

class UraniumSchemaServiceImpl
    : public api::UraniumSchemaService::Service {
 public:
  UraniumSchemaServiceImpl() {}
  virtual ~UraniumSchemaServiceImpl() {}

  UraniumSchemaServiceImpl(const UraniumSchemaServiceImpl&) = delete;
  UraniumSchemaServiceImpl& operator=(UraniumSchemaServiceImpl&) = delete;

  Status Init(std::shared_ptr<TableManager> table_manager) {
    assert(table_manager.get());
    table_manager_ = table_manager;
    return Status::OK();
  }

 private:
  std::shared_ptr<TableManager> table_manager_;
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUM_SERVICE_IMPL_H_

