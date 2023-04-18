#ifndef __EVENT_HPP__
#define __EVENT_HPP__
#include <mutex>
#include <condition_variable>

class Event {
 public:
  template <class _Predicate>
  void Wait(_Predicate predicate) {
    std::unique_lock<std::mutex> _(mutex_);
    cv_.wait(_, predicate);
    _.unlock();
  }

  template <class _Predicate>
  bool Wait_for(_Predicate predicate, long long ms) {
    std::unique_lock<std::mutex> _(mutex_);
    bool ret = cv_.wait_for(_, std::chrono::milliseconds(ms), predicate);
    _.unlock();
    return ret;
  }

  void Notify() {
    std::unique_lock<std::mutex> _(mutex_);
    cv_.notify_one();
    _.unlock();
  }

  void Notify_all() {
    std::unique_lock<std::mutex> _(mutex_);
    cv_.notify_all();
    _.unlock();
  }

 private:
  std::mutex mutex_;
  std::condition_variable cv_;
};

#endif
