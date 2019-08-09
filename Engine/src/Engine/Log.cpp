#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Engine
{
  std::shared_ptr<spdlog::logger> impl::Logger::s_logger;

  void impl::Logger::Init(char const * a_name)
  {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_logger = spdlog::stdout_color_mt(a_name);
    s_logger->set_level(spdlog::level::trace);
  }
}
