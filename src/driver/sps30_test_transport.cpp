#include <sps30_transport.hpp>

using namespace sps30;

transport::status_t transport::read(const transport::command_t command, uint8_t* const data,
									const size_t length)
{
}

transport::status_t write(const transport::command_t command, const uint8_t* const data,
						  const size_t length)
{
}

transport::status_t transcieve(const transport::command_t command, const uint8_t* const tx_data,
							   const size_t tx_length, uint8_t* const rx_data,
							   const size_t rx_length)
{
}
