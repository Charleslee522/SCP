# define the C compiler to use
CC = gcc
CXX = g++
CFLAGS = -Wall -g -std=c++11  -Wno-deprecated-declarations
PARAMS =

BIN_DIR=../bin

# Source and header dirs.
RPC_DIR=./rpc-layer
SCP_DIR=./scp
MESSAGE_DIR=$(SCP_DIR)/message
ASTEROID_DIR=./apps/kv-stores/asteroid
COMET_DIR=./apps/kv-stores/comet
UTIL_DIR=./util
LIB_DIR=./lib
SSL_LIB_DIR=/usr/local/Cellar/openssl/1.0.2l/lib/
CER_DIR=./cer

# define any directories containing header files other than /usr/include, e.g. -I
INCLUDES = -I $(RPC_DIR) -I $(LIB_DIR) -I $(SCP_DIR) -I $(MESSAGE_DIR) -I $(UTIL_DIR)
# define library paths in addition to /usr/lib, e.g. -L
LFLAGS = -L $(SSL_LIB_DIR)

# define any libraries to link into executable:, e.g. -l
LIBS = -lssl -lcrypto

ifeq ($(VERBOSE),1)
PARAMS += -DVERBOSE
endif

ifeq ($(DEBUG),1)
PARAMS += -DDEBUG
endif

# Source files.
ifeq ($(APP),comet)
SRCS += $(COMET_DIR)/main.cpp
SRCS += $(COMET_DIR)/client.cpp $(COMET_DIR)/server.cpp
INCLUDES += -I $(COMET_DIR)
MAIN = $(BIN_DIR)/comet
endif

ifeq ($(APP),asteroid)
SRCS += $(ASTEROID_DIR)/main.cpp
SRCS += $(ASTEROID_DIR)/stellarkv.cpp
INCLUDES += -I $(ASTEROID_DIR)
MAIN = $(BIN_DIR)/asteroid
endif

SRCS += $(RPC_DIR)/fakeRPC.cpp
SRCS += $(SCP_DIR)/node.cpp $(SCP_DIR)/slot.cpp
SRCS += $(MESSAGE_DIR)/FinishMessage.cpp $(MESSAGE_DIR)/PrepareMessage.cpp

# define the C source files
OBJS = $(join $(dir $(SRCS)), $(notdir $(SRCS:.cpp=.o)))

.PHONY: clean

all: | directory openssl $(MAIN)
ifdef APP
	@echo  Built to $(MAIN)
else
	@echo "Specify APP to comet or asteroid"
	@echo "ex) export APP=comet"
endif

openssl:
	@echo "[INFO] Install openssl required"
	@ln -fs /usr/local/opt/openssl/include/openssl /usr/local/include/

$(MAIN): $(OBJS) 
	$(CXX) $(PARAMS) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

directory:
	@echo "make bin dir..."
	mkdir -p ../bin

## Generic compilation rule
%.o : %.cpp
	@mkdir -p $(dir $@)
	@echo "============="
	@echo "Compiling $<"
	$(CXX) $(PARAMS) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN) $(OBJS)
	rm -rf $(CER_DIR)

cereal: clone_cereal
	cp -r $(CER_DIR)/include ./lib;

clone_cereal:
	git clone https://github.com/USCiLab/cereal.git $(CER_DIR)
