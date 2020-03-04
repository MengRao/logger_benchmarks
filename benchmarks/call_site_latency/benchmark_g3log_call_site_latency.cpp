#include "call_site_latency_bench.h"

#include "g3log/g3log.hpp"
#include "g3log/logworker.hpp"

/***/
void g3log_benchmark(std::vector<int32_t> thread_count_array, size_t num_iterations_per_thread)
{
  std::remove("g3log_call_site_latency_percentile_linux_benchmark.log");

  auto worker = g3::LogWorker::createLogWorker();
  auto handle = worker->addDefaultLogger("g2log_logger", "./");
  g3::initializeLogging(worker.get());

  // wait for the backend thread to start
  std::this_thread::sleep_for(std::chrono::seconds(2));

  // Define a logging lambda
  auto log_func = [](int32_t i, double d, char const* str) {
    auto const start = std::chrono::steady_clock::now();
    LOG(INFO) << "Logging str: " << str << ", int: " << i << ", double: " << d;
    auto const end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  };

  auto on_start = []() {};

  auto on_exit = []() {};

  // Run the benchmark for n threads
  for (auto thread_count : thread_count_array)
  {
    run_benchmark("Logger: G3Log - Benchmark: Caller Thread Latency", thread_count,
                  num_iterations_per_thread, on_start, log_func, on_exit);
  }
}

/***/
int main(int argc, char* argv[]) { g3log_benchmark(THREAD_LIST_COUNT, ITERATIONS); }