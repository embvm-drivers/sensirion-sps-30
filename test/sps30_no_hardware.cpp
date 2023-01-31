#include <catch2/catch_test_macros.hpp>
#include <driver.hpp>
#include <sps30_transport.hpp>

constexpr std::chrono::duration<uint32_t> FOUR_HOURS_IN_SEC(60 * 60 * 4);

TEST_CASE("Declare a driver instance", "[test/sps30]")
{
	sps30::transport t;
	sps30::sensor s(t);
}

TEST_CASE("Probe, values should be populated", "[test/sps30]")
{
	sps30::transport t;
	sps30::sensor s(t);

	s.probe();

	const auto serial = s.serial();
	CHECK(serial[0] != 0);

	auto duration = s.autoCleanInterval();
	CHECK(duration.count() != 0);
	auto version = s.firmwareVersion();
	CHECK(version.major != 0);
	CHECK(version.minor != 0);
}

TEST_CASE("Probe, then set auto clean interval", "[test/sps30]")
{
	sps30::transport t;
	sps30::sensor s(t);

	s.probe();

	auto duration = s.autoCleanInterval();
	CHECK(duration.count() != 0);

	auto new_duration = s.autoCleanInterval(FOUR_HOURS_IN_SEC);
	CHECK(duration != new_duration);
	CHECK(new_duration == FOUR_HOURS_IN_SEC);
	CHECK(s.autoCleanInterval() == FOUR_HOURS_IN_SEC);
}
