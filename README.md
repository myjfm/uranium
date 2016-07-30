## uranium: 分布式高性能持久化NoSQL存储系统

uranium是一个持久化的分布式NoSQL存储系统，既支持schemaless的类redis式的数据模型，
也支持schema的类MySQL式的数据模型。

uranium单机存储引擎构建于RocksDB之上，在其上封装丰富的数据类型，以及封装对schema
的支持。

uranium采用gRPC为通信框架，对外提供protobuf 3.0格式的接口。

uranium支持分布式，集群水平扩展，自动负载均衡等特性。

