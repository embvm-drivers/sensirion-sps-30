#include <catch2/catch.hpp>
#include <driver.hpp>
#include <sps30_transport.hpp>

TEST_CASE("Declare a driver instance", "[test/sps30]")
{
	sps30::transport t;
	sps30::sensor s(t);

	// CHECK(1 == 1);
	// STATIC_REQUIRE(1 == 1);
}
