#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__
#include <memory>
#include <mutex>
#include <vector>

#include "thread_utils.h"
#include "worker.hpp"

class ThreadPool {
 private:
  struct WorkerProperty_ {
    std::string name;
    std::shared_ptr<Worker> worker;
  };

 protected:
  ThreadPool() {
    unsigned kMaxThreadCount = std::thread::hardware_concurrency();
    if (0 == kMaxThreadCount) {
      kMaxThreadCount = 1;
    }
    int concurrency_level = kMaxThreadCount * 2;
    if (concurrency_level < 16) {
      concurrency_level = 16;
    }

    capacity_ = concurrency_level;
  }

 public:
  static ThreadPool& GetInstance() {
    static ThreadPool pool;
    return pool;
  }
  ~ThreadPool() = default;

  std::shared_ptr<Worker> GetOne(const char* name) {
    if (!name || *name == '\0') {
      return std::shared_ptr<Worker>();
    }

    std::lock_guard<std::mutex> _(mutex_);
    std::string thread_name(name);

    for (auto& worker : workers_) {
      if (worker.name == thread_name) {
        return worker.worker;
      }
    }

    if (static_cast<int>(workers_.size()) < capacity_) {
      std::shared_ptr<Worker> worker = std::make_shared<Worker>();
      worker->Initialze();
      worker->sync_call([=] {
        setThreadName(thread_name);
        return 0;
      });
      workers_.push_back({thread_name, worker});
      return worker;
    }
    int idx = (curr_idx_++) % capacity_;
    if (workers_[idx].name.find(thread_name) == std::string::npos) {
      workers_[idx].name += "," + thread_name;
    }

    workers_[idx].worker->sync_call([=] {
      setThreadName(thread_name);
      return 0;
    });

    return workers_[idx].worker;
  }

  void Clear() {
    std::lock_guard<std::mutex> _(mutex_);
    std::vector<WorkerProperty_> worker_needs_clean;
    for (size_t i = 0; i < workers_.size(); ++i) {
      worker_needs_clean.emplace_back(workers_[i]);
    }
    for (auto& w : worker_needs_clean) {
      w.worker->Clear();
    }
    worker_needs_clean.clear();
    workers_.clear();
  }

  std::vector<std::shared_ptr<Worker>> Workers() {
    std::vector<std::shared_ptr<Worker>> ret;
    std::lock_guard<std::mutex> _(mutex_);
    for (auto& p : workers_) {
      ret.emplace_back(p.worker);
    }
    return ret;
  }

 private:
  std::vector<WorkerProperty_> workers_;
  std::mutex mutex_;
  int capacity_ = 0;
  int curr_idx_ = 0;
};


inline std::shared_ptr<Worker> major_worker() {
  return ThreadPool::GetInstance().GetOne("major_worker");
}

inline std::shared_ptr<Worker> callback_worker() {
  return ThreadPool::GetInstance().GetOne("callback_worker");
}

inline std::shared_ptr<Worker> minor_worker(std::string & worker_name) {
  return ThreadPool::GetInstance().GetOne(worker_name.c_str());
}

inline std::shared_ptr<Worker> event_listener_worker() {
  return ThreadPool::GetInstance().GetOne("event_listener_worker");
}




#endif