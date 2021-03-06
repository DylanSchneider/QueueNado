#pragma once
#include "Harpoon.h"

class MockHarpoon : public Harpoon {
public:

   MockHarpoon() : Harpoon() {
   }

   virtual ~MockHarpoon() {
   }

   void CallRequestChunks() {
      void RequestChunks();
   }

   Harpoon::Battling CallPollTimeout(int timeoutMs) {
      return Harpoon::PollTimeout(timeoutMs);
   }

};

