#pragma once
#include "BoomStick.h"
#ifdef LR_DEBUG

class MockBoomStick : public BoomStick {
public:

   explicit MockBoomStick(const std::string& binding) : BoomStick(binding), mFailsInit(false),
   mFailsGetNewContext(false),
   mFailseGetNewSocket(false),
   mFailsConnect(false) {
   }

   virtual ~MockBoomStick() {
   }

   bool Initialize() override {
      if (mFailsInit) {
         return false;
      }
      return BoomStick::Initialize();
   }

   std::string Send(const std::string& command) override {
      if (mReturnString.empty()) {
         return BoomStick::Send(command);
      } else {
         return mReturnString;
      }
   }

   bool SendAsync(const std::string& uuid, const std::string& command) override {
      return false;
   }

   std::string GetAsyncReply(const std::string& uuid) override {
      return
      {
      };
   }

   zctx_t* GetNewContext() override {
      if (mFailsGetNewContext) {
         return NULL;
      }
      return BoomStick::GetNewContext();
   }

   void* GetNewSocket(zctx_t* ctx) override {
      if (mFailseGetNewSocket) {
         return NULL;
      }
      return BoomStick::GetNewSocket(ctx);
   }

   bool ConnectToBinding(void* socket, const std::string& binding) override {
      if (mFailsConnect) {
         return false;
      }
      return BoomStick::ConnectToBinding(socket, binding);
   }


   bool mFailsInit;
   bool mFailsGetNewContext;
   bool mFailseGetNewSocket;
   bool mFailsConnect;
   std::string mReturnString;
};
#endif

