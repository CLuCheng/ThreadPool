#ifndef __WOKER_HPP__
#define __WOKER_HPP__
#include <chrono>
#include <functional>
#include <list>
#include <mutex>
#include <thread>

#include "event.hpp"

using async_queue_task_type = std::function<void(void)>;
using sync_queue_task_type = std::function<int(void)>;

class Worker {
  static constexpr int kmaxTimeout = 15000;

 public:
  Worker() = default;

  ~Worker() {
    BreakThread();
  }

  void Wait() {
    fin_event_.Wait([this] { return fin_singal_.load(); });
  }

  void Clear() {
    std::lock_guard<std::mutex> _(mutex_);
    tasks_.clear();
  }

  bool IsAlive() { return is_exit_ == false; }

  bool BreakThread() {
    is_exit_ = true;
    queue_event_.Notify();
    if (thread_ && thread_->joinable()) thread_->join();
    return is_exit_;
  }

  bool Initialze() {
    thread_.reset(new std::thread([this] { this->main_loop(); }));
    thread_id_ = thread_->get_id();
    return true;
  }

  void async_call(async_queue_task_type&& task) noexcept {
    {
      std::lock_guard<std::mutex> _(mutex_);
      tasks_.push_back([task] { task(); });
    }
    queue_event_.Notify();
  }

  int sync_call(sync_queue_task_type&& task) noexcept {
    int ret = -1;
    if (thread_id_ == std::this_thread::get_id()) {
      return task();
    }
    std::shared_ptr<std::atomic<bool>> sync_singal_ =
        std::make_shared<std::atomic<bool>>(false);
    std::shared_ptr<Event> sync_event = std::make_shared<Event>();
    {
      std::lock_guard<std::mutex> _(mutex_);
      tasks_.push_back([task, sync_event, sync_singal_, &ret] {
        ret = task();
        sync_singal_->store(true);
        sync_event->Notify();
      });
    }
    queue_event_.Notify();
    sync_event->Wait_for([sync_singal_] { return sync_singal_->load(); },
                         kmaxTimeout);
    return ret;
  }

 private:
  void main_loop() {
    while (true) {
      async_queue_task_type task;
      queue_event_.Wait([this]() { return is_exit_ || !tasks_.empty(); });
      {
        std::lock_guard<std::mutex> _(mutex_);
        if (is_exit_ && tasks_.empty()) {
          return;
        }
        if (!tasks_.empty()) {
          task = std::move(tasks_.front());
          tasks_.pop_front();
        }
      }
      if (task) task();
      if (tasks_.empty()) {
        fin_singal_ = true;
        this->fin_event_.Notify();
      } else {
        fin_singal_ = false;
      }
    }
  }

 private:
  std::unique_ptr<std::thread> thread_;
  std::list<async_queue_task_type> tasks_;
  std::mutex mutex_;
  std::thread::id thread_id_;
  std::atomic<bool> is_exit_{false};
  Event queue_event_;
  Event fin_event_;
  std::atomic<bool> fin_singal_{false};
};

#endif