#include <catch2/catch.hpp>
#include <driver.hpp>
#include <sps30_transport.hpp>

TEST_CASE("Sanity Check", "[test/catch2_test_case]")
{
	CHECK(1 == 1);
	STATIC_REQUIRE(1 == 1);
}
