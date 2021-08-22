#include <catch2/catch.hpp>
#include <driver.hpp>
#include <sps30_transport.hpp>

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
