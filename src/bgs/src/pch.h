#pragma once

#include "Utils/MoveOnlyFunction.h"

// Boost (ASIO, networking)
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
// #include <boost/asio/deferred.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/write.hpp>

// Useful namespace aliases, according the the Google Style Guide this is bad
// practice because it bloats the global namespace but the verbose
// namespacing of Boost just inhibits not shortening things. Use full
// qualification for all namespace aliases in case something changes in the
// future
namespace asio = boost::asio;
namespace tls = boost::asio::ssl;
using boost::asio::ip::tcp;

#define AWAIT_RETURN co_return co_await

// Beast (WebSocket)
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>

// Useful namespace aliases, read boost aliases for more information about why
// this is bad
namespace beast = boost::beast;

#include <spdlog/spdlog.h>
