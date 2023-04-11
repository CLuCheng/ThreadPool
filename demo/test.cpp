// thread_pool.cpp : Defines the entry point for the application.
//

#include <iostream>

#include "../thread_pool/include/thread_pool.h"
#include "../thread_pool/include/worker.hpp"
using namespace std;

void test1() {
  Worker worker;
  worker.Initialze();
  worker.async_call([] {
    for (int i = 0; i < 20; i++) {
      cout << "hello world" << std::endl;
      std::this_thread::sleep_for(1s);
    }
  });

  int ans = worker.sync_call([] {
    int ans = 0;
    for (int i = 0; i < 10; i++) ans += i;
    return ans;
  });
  std::cout << ans << std::endl;

  ans = worker.sync_call([&worker] {
    return worker.sync_call([] {
      int ans = 0;
      for (int i = 0; i < 10; i++) ans += i;
      return ans;
    });
  });
  std::cout << ans << std::endl;

  worker.Wait();
}

void test2() {
  major_worker()->async_call([] {
    for (int i = 0; i < 20; i++) {
      cout << "hello world" << std::endl;
      std::this_thread::sleep_for(1ms);
    }
  });

  int ans = major_worker()->sync_call([] {
    int ans = 0;
    for (int i = 0; i < 10; i++) ans += i;
    return ans;
  });
  std::cout << ans << std::endl;

  ans = major_worker()->sync_call([] {
    return major_worker()->sync_call([] {
      int ans = 0;
      for (int i = 0; i < 10; i++) ans += i;
      return ans;
    });
  });
  std::cout << ans << std::endl;

}

int main() {
  test2();
  major_worker()->Clear();
  callback_worker()->Clear();
  callback_worker()->BreakThread();
  callback_worker()->async_call([] {
    for (int i = 0; i < 100; i++) {
      cout << "async_call" << std::endl;
      std::this_thread::sleep_for(1s);
    }
  });
  return 0;
}
