/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Calculator_H
#define Calculator_H

#include <thrift/TDispatchProcessor.h>
#include "tutorial_types.h"
#include "SharedService.h"

namespace tutorial {

class CalculatorIf : virtual public  ::shared::SharedServiceIf {
 public:
  virtual ~CalculatorIf() {}

  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   */
  virtual void ping() = 0;
  virtual int32_t add(const int32_t num1, const int32_t num2) = 0;
  virtual int32_t calculate(const int32_t logid, const Work& w) = 0;

  /**
   * This method has a oneway modifier. That means the client only makes
   * a request and does not listen for any response at all. Oneway methods
   * must be void.
   */
  virtual void zip() = 0;
};

class CalculatorIfFactory : virtual public  ::shared::SharedServiceIfFactory {
 public:
  typedef CalculatorIf Handler;

  virtual ~CalculatorIfFactory() {}

  virtual CalculatorIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler( ::shared::SharedServiceIf* /* handler */) = 0;
};

class CalculatorIfSingletonFactory : virtual public CalculatorIfFactory {
 public:
  CalculatorIfSingletonFactory(const boost::shared_ptr<CalculatorIf>& iface) : iface_(iface) {}
  virtual ~CalculatorIfSingletonFactory() {}

  virtual CalculatorIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler( ::shared::SharedServiceIf* /* handler */) {}

 protected:
  boost::shared_ptr<CalculatorIf> iface_;
};

class CalculatorNull : virtual public CalculatorIf , virtual public  ::shared::SharedServiceNull {
 public:
  virtual ~CalculatorNull() {}
  void ping() {
    return;
  }
  int32_t add(const int32_t /* num1 */, const int32_t /* num2 */) {
    int32_t _return = 0;
    return _return;
  }
  int32_t calculate(const int32_t /* logid */, const Work& /* w */) {
    int32_t _return = 0;
    return _return;
  }
  void zip() {
    return;
  }
};


class Calculator_ping_args {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  Calculator_ping_args(const Calculator_ping_args&);
  Calculator_ping_args& operator=(const Calculator_ping_args&);
  Calculator_ping_args() {
  }

  virtual ~Calculator_ping_args() throw();

  bool operator == (const Calculator_ping_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Calculator_ping_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Calculator_ping_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Calculator_ping_args& obj);
};


class Calculator_ping_pargs {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~Calculator_ping_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Calculator_ping_pargs& obj);
};


class Calculator_ping_result {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  Calculator_ping_result(const Calculator_ping_result&);
  Calculator_ping_result& operator=(const Calculator_ping_result&);
  Calculator_ping_result() {
  }

  virtual ~Calculator_ping_result() throw();

  bool operator == (const Calculator_ping_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Calculator_ping_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Calculator_ping_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Calculator_ping_result& obj);
};


class Calculator_ping_presult {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~Calculator_ping_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const Calculator_ping_presult& obj);
};

typedef struct _Calculator_add_args__isset {
  _Calculator_add_args__isset() : num1(false), num2(false) {}
  bool num1 :1;
  bool num2 :1;
} _Calculator_add_args__isset;

class Calculator_add_args {
 public:

  static const char* ascii_fingerprint; // = "989D1F1AE8D148D5E2119FFEC4BBBEE3";
  static const uint8_t binary_fingerprint[16]; // = {0x98,0x9D,0x1F,0x1A,0xE8,0xD1,0x48,0xD5,0xE2,0x11,0x9F,0xFE,0xC4,0xBB,0xBE,0xE3};

  Calculator_add_args(const Calculator_add_args&);
  Calculator_add_args& operator=(const Calculator_add_args&);
  Calculator_add_args() : num1(0), num2(0) {
  }

  virtual ~Calculator_add_args() throw();
  int32_t num1;
  int32_t num2;

  _Calculator_add_args__isset __isset;

  void __set_num1(const int32_t val);

  void __set_num2(const int32_t val);

  bool operator == (const Calculator_add_args & rhs) const
  {
    if (!(num1 == rhs.num1))
      return false;
    if (!(num2 == rhs.num2))
      return false;
    return true;
  }
  bool operator != (const Calculator_add_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Calculator_add_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Calculator_add_args& obj);
};


class Calculator_add_pargs {
 public:

  static const char* ascii_fingerprint; // = "989D1F1AE8D148D5E2119FFEC4BBBEE3";
  static const uint8_t binary_fingerprint[16]; // = {0x98,0x9D,0x1F,0x1A,0xE8,0xD1,0x48,0xD5,0xE2,0x11,0x9F,0xFE,0xC4,0xBB,0xBE,0xE3};


  virtual ~Calculator_add_pargs() throw();
  const int32_t* num1;
  const int32_t* num2;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Calculator_add_pargs& obj);
};

typedef struct _Calculator_add_result__isset {
  _Calculator_add_result__isset() : success(false) {}
  bool success :1;
} _Calculator_add_result__isset;

class Calculator_add_result {
 public:

  static const char* ascii_fingerprint; // = "32183C4A04E706C58ED2F62566DDD8DE";
  static const uint8_t binary_fingerprint[16]; // = {0x32,0x18,0x3C,0x4A,0x04,0xE7,0x06,0xC5,0x8E,0xD2,0xF6,0x25,0x66,0xDD,0xD8,0xDE};

  Calculator_add_result(const Calculator_add_result&);
  Calculator_add_result& operator=(const Calculator_add_result&);
  Calculator_add_result() : success(0) {
  }

  virtual ~Calculator_add_result() throw();
  int32_t success;

  _Calculator_add_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const Calculator_add_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Calculator_add_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Calculator_add_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Calculator_add_result& obj);
};

typedef struct _Calculator_add_presult__isset {
  _Calculator_add_presult__isset() : success(false) {}
  bool success :1;
} _Calculator_add_presult__isset;

class Calculator_add_presult {
 public:

  static const char* ascii_fingerprint; // = "32183C4A04E706C58ED2F62566DDD8DE";
  static const uint8_t binary_fingerprint[16]; // = {0x32,0x18,0x3C,0x4A,0x04,0xE7,0x06,0xC5,0x8E,0xD2,0xF6,0x25,0x66,0xDD,0xD8,0xDE};


  virtual ~Calculator_add_presult() throw();
  int32_t* success;

  _Calculator_add_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const Calculator_add_presult& obj);
};

typedef struct _Calculator_calculate_args__isset {
  _Calculator_calculate_args__isset() : logid(false), w(false) {}
  bool logid :1;
  bool w :1;
} _Calculator_calculate_args__isset;

class Calculator_calculate_args {
 public:

  static const char* ascii_fingerprint; // = "96E0ABD186230859F1AE693D9C9E8724";
  static const uint8_t binary_fingerprint[16]; // = {0x96,0xE0,0xAB,0xD1,0x86,0x23,0x08,0x59,0xF1,0xAE,0x69,0x3D,0x9C,0x9E,0x87,0x24};

  Calculator_calculate_args(const Calculator_calculate_args&);
  Calculator_calculate_args& operator=(const Calculator_calculate_args&);
  Calculator_calculate_args() : logid(0) {
  }

  virtual ~Calculator_calculate_args() throw();
  int32_t logid;
  Work w;

  _Calculator_calculate_args__isset __isset;

  void __set_logid(const int32_t val);

  void __set_w(const Work& val);

  bool operator == (const Calculator_calculate_args & rhs) const
  {
    if (!(logid == rhs.logid))
      return false;
    if (!(w == rhs.w))
      return false;
    return true;
  }
  bool operator != (const Calculator_calculate_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Calculator_calculate_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Calculator_calculate_args& obj);
};


class Calculator_calculate_pargs {
 public:

  static const char* ascii_fingerprint; // = "96E0ABD186230859F1AE693D9C9E8724";
  static const uint8_t binary_fingerprint[16]; // = {0x96,0xE0,0xAB,0xD1,0x86,0x23,0x08,0x59,0xF1,0xAE,0x69,0x3D,0x9C,0x9E,0x87,0x24};


  virtual ~Calculator_calculate_pargs() throw();
  const int32_t* logid;
  const Work* w;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Calculator_calculate_pargs& obj);
};

typedef struct _Calculator_calculate_result__isset {
  _Calculator_calculate_result__isset() : success(false), ouch(false) {}
  bool success :1;
  bool ouch :1;
} _Calculator_calculate_result__isset;

class Calculator_calculate_result {
 public:

  static const char* ascii_fingerprint; // = "251401227E6B35383D17ACC8BC9947BF";
  static const uint8_t binary_fingerprint[16]; // = {0x25,0x14,0x01,0x22,0x7E,0x6B,0x35,0x38,0x3D,0x17,0xAC,0xC8,0xBC,0x99,0x47,0xBF};

  Calculator_calculate_result(const Calculator_calculate_result&);
  Calculator_calculate_result& operator=(const Calculator_calculate_result&);
  Calculator_calculate_result() : success(0) {
  }

  virtual ~Calculator_calculate_result() throw();
  int32_t success;
  InvalidOperation ouch;

  _Calculator_calculate_result__isset __isset;

  void __set_success(const int32_t val);

  void __set_ouch(const InvalidOperation& val);

  bool operator == (const Calculator_calculate_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(ouch == rhs.ouch))
      return false;
    return true;
  }
  bool operator != (const Calculator_calculate_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Calculator_calculate_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Calculator_calculate_result& obj);
};

typedef struct _Calculator_calculate_presult__isset {
  _Calculator_calculate_presult__isset() : success(false), ouch(false) {}
  bool success :1;
  bool ouch :1;
} _Calculator_calculate_presult__isset;

class Calculator_calculate_presult {
 public:

  static const char* ascii_fingerprint; // = "251401227E6B35383D17ACC8BC9947BF";
  static const uint8_t binary_fingerprint[16]; // = {0x25,0x14,0x01,0x22,0x7E,0x6B,0x35,0x38,0x3D,0x17,0xAC,0xC8,0xBC,0x99,0x47,0xBF};


  virtual ~Calculator_calculate_presult() throw();
  int32_t* success;
  InvalidOperation ouch;

  _Calculator_calculate_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const Calculator_calculate_presult& obj);
};


class Calculator_zip_args {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  Calculator_zip_args(const Calculator_zip_args&);
  Calculator_zip_args& operator=(const Calculator_zip_args&);
  Calculator_zip_args() {
  }

  virtual ~Calculator_zip_args() throw();

  bool operator == (const Calculator_zip_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Calculator_zip_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Calculator_zip_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Calculator_zip_args& obj);
};


class Calculator_zip_pargs {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~Calculator_zip_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Calculator_zip_pargs& obj);
};

class CalculatorClient : virtual public CalculatorIf, public  ::shared::SharedServiceClient {
 public:
  CalculatorClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
     ::shared::SharedServiceClient(prot, prot) {}
  CalculatorClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :     ::shared::SharedServiceClient(iprot, oprot) {}
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void ping();
  void send_ping();
  void recv_ping();
  int32_t add(const int32_t num1, const int32_t num2);
  void send_add(const int32_t num1, const int32_t num2);
  int32_t recv_add();
  int32_t calculate(const int32_t logid, const Work& w);
  void send_calculate(const int32_t logid, const Work& w);
  int32_t recv_calculate();
  void zip();
  void send_zip();
};

class CalculatorProcessor : public  ::shared::SharedServiceProcessor {
 protected:
  boost::shared_ptr<CalculatorIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (CalculatorProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_add(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_calculate(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_zip(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  CalculatorProcessor(boost::shared_ptr<CalculatorIf> iface) :
     ::shared::SharedServiceProcessor(iface),
    iface_(iface) {
    processMap_["ping"] = &CalculatorProcessor::process_ping;
    processMap_["add"] = &CalculatorProcessor::process_add;
    processMap_["calculate"] = &CalculatorProcessor::process_calculate;
    processMap_["zip"] = &CalculatorProcessor::process_zip;
  }

  virtual ~CalculatorProcessor() {}
};

class CalculatorProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  CalculatorProcessorFactory(const ::boost::shared_ptr< CalculatorIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< CalculatorIfFactory > handlerFactory_;
};

class CalculatorMultiface : virtual public CalculatorIf, public  ::shared::SharedServiceMultiface {
 public:
  CalculatorMultiface(std::vector<boost::shared_ptr<CalculatorIf> >& ifaces) : ifaces_(ifaces) {
    std::vector<boost::shared_ptr<CalculatorIf> >::iterator iter;
    for (iter = ifaces.begin(); iter != ifaces.end(); ++iter) {
       ::shared::SharedServiceMultiface::add(*iter);
    }
  }
  virtual ~CalculatorMultiface() {}
 protected:
  std::vector<boost::shared_ptr<CalculatorIf> > ifaces_;
  CalculatorMultiface() {}
  void add(boost::shared_ptr<CalculatorIf> iface) {
     ::shared::SharedServiceMultiface::add(iface);
    ifaces_.push_back(iface);
  }
 public:
  void ping() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ping();
    }
    ifaces_[i]->ping();
  }

  int32_t add(const int32_t num1, const int32_t num2) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->add(num1, num2);
    }
    return ifaces_[i]->add(num1, num2);
  }

  int32_t calculate(const int32_t logid, const Work& w) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->calculate(logid, w);
    }
    return ifaces_[i]->calculate(logid, w);
  }

  void zip() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->zip();
    }
    ifaces_[i]->zip();
  }

};

} // namespace

#endif
