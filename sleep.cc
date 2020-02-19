#include <seastar/core/sleep.hh>
#include <iostream>

seastar::future<> f1()
{
  std::cout << "sleeping ... " << std::flush;

  using namespace std::chrono_literals;
  seastar::sleep(200ms).then( [] {
      std::cout << "200ms " << std::flush; });
  seastar::sleep(100ms).then( [] {
      std::cout << "100ms " << std::flush; });
  return seastar::sleep(1s).then( [] {
      std::cout << "done." << std::endl; });
}

seastar::future<int> slow()
{
  using namespace std::chrono_literals;
  return seastar::sleep(100ms).then([] { return 3; });
}

seastar::future<> f2()
{
  return slow().then([] (int val) {
      std::cout << "got " << val << std::endl;
      });
}

seastar::future<> f3()
{
  using namespace std::chrono_literals;
  return seastar::sleep(100ms).then([] {
      return 3;
  }).then([] (int val) {
    std::cout << "got " << val << std::endl;
  });
}
