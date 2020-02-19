#include <seastar/core/app-template.hh>
#include <seastar/util/log.hh>
#include <seastar/util/backtrace.hh>
#include <iostream>
#include <sstream>

static seastar::logger logger("exceptions");

seastar::future<> fail_exception_future()
{
  return seastar::make_exception_future_with_backtrace<>(std::exception());
}

void boom()
{
  throw std::exception();
}

seastar::future<> fail_boom()
{
  boom();
  return seastar::make_ready_future();
}

seastar::future<> fail_wrapped_boom()
{
  try {
    boom();
  } catch (...) {
    logger.info("caught exception");
    return seastar::make_exception_future<>(std::current_exception());
  }
  return seastar::make_ready_future();
}

seastar::future<> f1()
{
  logger.info("engine alive; run f1()");
  return fail_exception_future().finally([] {
      logger.info("f1(): clean up");
    });
}

seastar::future<> f2()
{
  logger.info("engine alive; run f2()");
  return fail_wrapped_boom();/*.finally([] {
      logger.info("f2(): clean up");
    });
  */
}

seastar::future<> f3()
{
  logger.info("engine alive; run f3()");
  logger.info("f3(): an unhandled exception is going to abort the engine");

  return fail_boom().finally([] {
      logger.info("f3(): this will not be run");
    });
}

seastar::future<> handled_future_exception()
{
  logger.info("handling a future exception");
  return fail_exception_future().handle_exception([] (std::exception_ptr e) {
      std::stringstream ss;
      ss << "caught exception: " << e;
      logger.info(ss.str().c_str());
    });
}

seastar::future<> functions()
{
  handled_future_exception().wait();

  f1().wait();
  seastar::future<> _f =
    f2().finally([] { logger.info("f2(): clean up"); });

// we are commenting f3() out because it will inevitably cause the engine to
// fail miserably. Uncomment if you want to see it in action.
//  f3();


  return seastar::make_ready_future();
}

int main(int argc, char *argv[])
{
  seastar::app_template app;
  app.run(argc, argv, functions);

  app.run(argc, argv, f2);
  return 0;
}
