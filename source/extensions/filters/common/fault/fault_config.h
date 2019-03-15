#pragma once

#include "envoy/config/filter/fault/v2/fault.pb.h"
#include "envoy/http/header_map.h"

#include "common/singleton/const_singleton.h"

namespace Envoy {
namespace Extensions {
namespace Filters {
namespace Common {
namespace Fault {

class HeaderNameValues {
public:
  const Http::LowerCaseString ThrottleRequestLatency{"x-envoy-throttle-request-latency"};
  const Http::LowerCaseString ThrottleResponseThroughput{"x-envoy-throttle-response-throughput"};
};

typedef ConstSingleton<HeaderNameValues> HeaderNames;

/**
 * fixfix
 */
class FaultDelayConfig {
public:
  FaultDelayConfig(const envoy::config::filter::fault::v2::FaultDelay& delay_config);

  const envoy::type::FractionalPercent& percentage() const { return percentage_; }
  absl::optional<std::chrono::milliseconds> duration(const Http::HeaderEntry* header) const {
    return provider_->duration(header);
  }

private:
  // fixfix
  class DelayProvider {
  public:
    virtual ~DelayProvider() = default;

    virtual absl::optional<std::chrono::milliseconds>
    duration(const Http::HeaderEntry* header) const PURE;
  };

  // fixfix
  class FixedDelayProvider : public DelayProvider {
  public:
    FixedDelayProvider(std::chrono::milliseconds delay) : delay_(delay) {}
    absl::optional<std::chrono::milliseconds> duration(const Http::HeaderEntry*) const override {
      return delay_;
    }

  private:
    const std::chrono::milliseconds delay_;
  };

  // fixfix
  class HeaderDelayProvider : public DelayProvider {
  public:
    absl::optional<std::chrono::milliseconds>
    duration(const Http::HeaderEntry* header) const override;
  };

  using DelayProviderPtr = std::unique_ptr<DelayProvider>;

  DelayProviderPtr provider_;
  const envoy::type::FractionalPercent percentage_;
};

using FaultDelayConfigPtr = std::unique_ptr<FaultDelayConfig>;
using FaultDelayConfigSharedPtr = std::shared_ptr<FaultDelayConfig>;

/**
 * fixfix
 */
class FaultRateLimitConfig {
public:
  FaultRateLimitConfig(const envoy::config::filter::fault::v2::FaultRateLimit& rate_limit_config);

  const envoy::type::FractionalPercent& percentage() const { return percentage_; }
  absl::optional<uint64_t> rateKbps(const Http::HeaderEntry* header) const {
    return provider_->rateKbps(header);
  }

private:
  // fixfix
  class RateLimitProvider {
  public:
    virtual ~RateLimitProvider() = default;

    virtual absl::optional<uint64_t> rateKbps(const Http::HeaderEntry* header) const PURE;
  };

  // fixfix
  class FixedRateLimitProvider : public RateLimitProvider {
  public:
    FixedRateLimitProvider(uint64_t fixed_rate_kbps) : fixed_rate_kbps_(fixed_rate_kbps) {}
    absl::optional<uint64_t> rateKbps(const Http::HeaderEntry*) const override {
      return fixed_rate_kbps_;
    }

  private:
    const uint64_t fixed_rate_kbps_;
  };

  // fixfix
  class HeaderRateLimitProvider : public RateLimitProvider {
  public:
    absl::optional<uint64_t> rateKbps(const Http::HeaderEntry* header) const override;
  };

  using RateLimitProviderPtr = std::unique_ptr<RateLimitProvider>;

  RateLimitProviderPtr provider_;
  const envoy::type::FractionalPercent percentage_;
};

using FaultRateLimitConfigPtr = std::unique_ptr<FaultRateLimitConfig>;

} // namespace Fault
} // namespace Common
} // namespace Filters
} // namespace Extensions
} // namespace Envoy
