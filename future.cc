#include <seastar/core/app-template.hh>
#include <seastar/util/log.hh>
#include <iostream>
#include <stdexcept>

extern seastar::future<> f1();
extern seastar::future<> f2();
extern seastar::future<> f3();

int main(int argc, char *argv[])
{

  seastar::app_template app;
  app.run(argc, argv, f3);
  return 0;
}
