#include "Client.h"
#include "DiabloClone.h"
#include "Terrorized.h"

#include <dpp/dpp.h>
#include <fmt/ranges.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <yaml-cpp/yaml.h>

using namespace d2rc;

constexpr auto CONFIG_VERSION = 3;

bool check_version(int32_t actual, int32_t expected) {
  if (actual != expected) {
    spdlog::error("Invalid app.yml version, found {} expected {}", actual, expected);
    return false;
  }
  return true;
}

void init_logger() {
#ifdef _DEBUG
  spdlog::set_level(spdlog::level::debug);
#else
  spdlog::set_level(spdlog::level::info);
#endif
  spdlog::flush_on(spdlog::level::debug);

  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
  sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("log.txt", 23, 59));
  auto main_logger = std::make_shared<spdlog::logger>("main", begin(sinks), end(sinks));
  spdlog::initialize_logger(main_logger);
  spdlog::set_default_logger(main_logger);
}

int main(int argc, char** argv) {
  init_logger();

  auto cfg = YAML::LoadFile("app.yml");
  if (!check_version(cfg["version"].as<int32_t>(), CONFIG_VERSION)) {
    return EXIT_FAILURE;
  }

  spdlog::info("Starting...");
  auto discord_token = cfg["discord"]["token"].as<std::string>();
  std::unique_ptr<dpp::cluster> discord_bot;
  if (!discord_token.empty()) {
    discord_bot = std::make_unique<dpp::cluster>(discord_token);

    discord_bot->on_log(dpp::utility::cout_logger());

    discord_bot->on_slashcommand([](const dpp::slashcommand_t& event) {
      if (event.command.get_command_name() == "terrorized") {
        auto current = Terrorized::GetInstance()->GetCurrent();
        auto next = Terrorized::GetInstance()->GetNext();

        if (current.empty() && next.empty()) {
          event.reply("I'm not really sure which zones are terrorized ¯\\_(ツ)_/¯");
        } else {
          if (next.empty()) {
            event.reply(fmt::format("The currently terrorized zones are {}", current));
          } else {
            event.reply(
                fmt::format("The currently terrorized zones are {}\nNext "
                            "terrorized zones are going to be {}",
                            current, next));
          }
        }
      } else if (event.command.get_command_name() == "dclone") {
        std::string region = std::get<std::string>(event.get_parameter("region"));
        d2rc::Region regionid = d2rc::Region::EU;
        if (region == "region_eu") {
          regionid = d2rc::Region::EU;
        } else if (region == "region_us") {
          regionid = d2rc::Region::US;
        } else if (region == "region_kr") {
          regionid = d2rc::Region::KR;
        }

        event.reply(DiabloClone::GetInstance()->GetStatus(regionid));
      }
    });

    discord_bot->on_ready([&](const dpp::ready_t& event) {
      if (dpp::run_once<struct register_bot_commands>()) {
        discord_bot->global_command_create(
            dpp::slashcommand("terrorized", "Fetch terrorized zones", discord_bot->me.id));
        dpp::slashcommand dclone_cmd("dclone", "Fetch diablo clone status", discord_bot->me.id);
        dclone_cmd.add_option(dpp::command_option(dpp::co_string, "region", "The region to query", true)
                                  .add_choice(dpp::command_option_choice("Europe", std::string("region_eu")))
                                  .add_choice(dpp::command_option_choice("United States", std::string("region_us")))
                                  .add_choice(dpp::command_option_choice("Asia", std::string("region_kr"))));
        discord_bot->global_command_create(dclone_cmd);
      }
    });

    discord_bot->start(dpp::st_return);
  }

  try {
    asio::io_context io(1);
    std::vector<std::shared_ptr<Client>> clients;

    for (const auto& client_opts : cfg["clients"]) {
      const auto& name = client_opts["name"].as<std::string>();
      const auto& region = static_cast<d2rc::Region>(client_opts["region"].as<uint8_t>());
      const auto& token = client_opts["web_credentials"].as<std::string>();

      spdlog::info("Adding client {}", name);
      auto client = std::make_shared<Client>(io, name, region, token);
      asio::co_spawn(io, client->Start(), asio::detached);
      clients.push_back(client);
    }

    asio::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto) { io.stop(); });

    io.run();
  } catch (const std::exception& e) {
    spdlog::error("Error: {}", e.what());
    return EXIT_FAILURE;
  }
  spdlog::info("Exiting...");

  return EXIT_SUCCESS;
}
