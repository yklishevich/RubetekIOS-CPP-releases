#pragma once

#define MSW_ASIO_SAFE_CALLBACK(F, ...) ((decltype(F))(F))(__VA_ARGS__);
