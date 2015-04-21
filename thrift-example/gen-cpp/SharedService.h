/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef SharedService_H
#define SharedService_H

#include <thrift/TDispatchProcessor.h>
#include "shared_types.h"

namespace shared {

class SharedServiceIf {
 public:
  virtual ~SharedServiceIf() {}
  virtual void getStruct(SharedStruct& _return, const int32_t key) = 0;
};

class SharedServiceIfFactory {
 public:
  typedef SharedServiceIf Handler;

  virtual ~SharedServiceIfFactory() {}

  virtual SharedServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(SharedServiceIf* /* handler */) = 0;
};

class SharedServiceIfSingletonFactory : virtual public SharedServiceIfFactory {
 public:
  SharedServiceIfSingletonFactory(const boost::shared_ptr<SharedServiceIf>& iface) : iface_(iface) {}
  virtual ~SharedServiceIfSingletonFactory() {}

  virtual SharedServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(SharedServiceIf* /* handler */) {}

 protected:
  boost::shared_ptr<SharedServiceIf> iface_;
};

class SharedServiceNull : virtual public SharedServiceIf {
 public:
  virtual ~SharedServiceNull() {}
  void getStruct(SharedStruct& /* _return */, const int32_t /* key */) {
    return;
  }
};

typedef struct _SharedService_getStruct_args__isset {
  _SharedService_getStruct_args__isset() : key(false) {}
  bool key :1;
} _SharedService_getStruct_args__isset;

class SharedService_getStruct_args {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

  SharedService_getStruct_args(const SharedService_getStruct_args&);
  SharedService_getStruct_args& operator=(const SharedService_getStruct_args&);
  SharedService_getStruct_args() : key(0) {
  }

  virtual ~SharedService_getStruct_args() throw();
  int32_t key;

  _SharedService_getStruct_args__isset __isset;

  void __set_key(const int32_t val);

  bool operator == (const SharedService_getStruct_args & rhs) const
  {
    if (!(key == rhs.key))
      return false;
    return true;
  }
  bool operator != (const SharedService_getStruct_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SharedService_getStruct_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const SharedService_getStruct_args& obj);
};


class SharedService_getStruct_pargs {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};


  virtual ~SharedService_getStruct_pargs() throw();
  const int32_t* key;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const SharedService_getStruct_pargs& obj);
};

typedef struct _SharedService_getStruct_result__isset {
  _SharedService_getStruct_result__isset() : success(false) {}
  bool success :1;
} _SharedService_getStruct_result__isset;

class SharedService_getStruct_result {
 public:

  static const char* ascii_fingerprint; // = "0B00C6A32956D0FB910392C197F6E622";
  static const uint8_t binary_fingerprint[16]; // = {0x0B,0x00,0xC6,0xA3,0x29,0x56,0xD0,0xFB,0x91,0x03,0x92,0xC1,0x97,0xF6,0xE6,0x22};

  SharedService_getStruct_result(const SharedService_getStruct_result&);
  SharedService_getStruct_result& operator=(const SharedService_getStruct_result&);
  SharedService_getStruct_result() {
  }

  virtual ~SharedService_getStruct_result() throw();
  SharedStruct success;

  _SharedService_getStruct_result__isset __isset;

  void __set_success(const SharedStruct& val);

  bool operator == (const SharedService_getStruct_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const SharedService_getStruct_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SharedService_getStruct_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const SharedService_getStruct_result& obj);
};

typedef struct _SharedService_getStruct_presult__isset {
  _SharedService_getStruct_presult__isset() : success(false) {}
  bool success :1;
} _SharedService_getStruct_presult__isset;

class SharedService_getStruct_presult {
 public:

  static const char* ascii_fingerprint; // = "0B00C6A32956D0FB910392C197F6E622";
  static const uint8_t binary_fingerprint[16]; // = {0x0B,0x00,0xC6,0xA3,0x29,0x56,0xD0,0xFB,0x91,0x03,0x92,0xC1,0x97,0xF6,0xE6,0x22};


  virtual ~SharedService_getStruct_presult() throw();
  SharedStruct* success;

  _SharedService_getStruct_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const SharedService_getStruct_presult& obj);
};

class SharedServiceClient : virtual public SharedServiceIf {
 public:
  SharedServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  SharedServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void getStruct(SharedStruct& _return, const int32_t key);
  void send_getStruct(const int32_t key);
  void recv_getStruct(SharedStruct& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class SharedServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<SharedServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (SharedServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_getStruct(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  SharedServiceProcessor(boost::shared_ptr<SharedServiceIf> iface) :
    iface_(iface) {
    processMap_["getStruct"] = &SharedServiceProcessor::process_getStruct;
  }

  virtual ~SharedServiceProcessor() {}
};

class SharedServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  SharedServiceProcessorFactory(const ::boost::shared_ptr< SharedServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< SharedServiceIfFactory > handlerFactory_;
};

class SharedServiceMultiface : virtual public SharedServiceIf {
 public:
  SharedServiceMultiface(std::vector<boost::shared_ptr<SharedServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~SharedServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<SharedServiceIf> > ifaces_;
  SharedServiceMultiface() {}
  void add(boost::shared_ptr<SharedServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void getStruct(SharedStruct& _return, const int32_t key) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->getStruct(_return, key);
    }
    ifaces_[i]->getStruct(_return, key);
    return;
  }

};

} // namespace

#endif
