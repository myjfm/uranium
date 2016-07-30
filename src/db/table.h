#ifndef URANIUM_STORE_TABLE_H_
#define URANIUM_STORE_TABLE_H_

#include "common/status.h"

namespace uranium {

class Table {
 public:
  Table() {}
  virtual ~Table() {}

  Status Init(const TableOptions& config) = 0;

};

}

#endif  // URANIUM_STORE_TABLE_H_

