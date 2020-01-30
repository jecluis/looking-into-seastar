#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>
#include <iostream>

int main(int argc, char *argv[]) {
  seastar::app_template app;
  app.run(argc, argv, [] {
      std::cout << seastar::smp::count << std::endl;
      return seastar::make_ready_future<>();
    });
  return 0;
}
