//codigo spi.tpp

//#include <spi/spi.hpp>
#include <iomanip> 
#include <cstring>
#include <stdexcept>



namespace SPI{

    template <typename BufferType>
    const bool Spi_t::read_write(const uint8_t cmd, const uint32_t address, BufferType& buffer) {
        // Verificar si el buffer es válido
        if constexpr (std::is_same_v<BufferType, std::vector<uint8_t>>) {
            if (buffer.empty()) {
                throw std::invalid_argument("El buffer proporcionado está vacío.");
                return true;
            }
        }

        // Calcular tamaños
        const size_t length = buffer.size(); // Siempre usar buffer.size() para vector
        const size_t cmd_size = 4;

        // Crear buffers para el comando y datos temporales
        std::vector<uint8_t> cmd_buffer_tx(cmd_size, 0);
        std::vector<uint8_t> cmd_buffer_rx(cmd_size, 0);

        // Llenar el buffer de comando
        cmd_buffer_tx[0] = cmd;
        cmd_buffer_tx[1] = (address >> 16) & 0xFF;
        cmd_buffer_tx[2] = (address >> 8) & 0xFF;
        cmd_buffer_tx[3] = address & 0xFF;

        // Configurar las transferencias SPI
        struct spi_ioc_transfer spi_transfer[2] = {};

        // Transferencia para el comando
        spi_transfer[0].tx_buf = reinterpret_cast<unsigned long>(cmd_buffer_tx.data());
        spi_transfer[0].rx_buf = reinterpret_cast<unsigned long>(cmd_buffer_rx.data());
        spi_transfer[0].len = cmd_size;
        spi_transfer[0].speed_hz = get_spi_speed();
        spi_transfer[0].bits_per_word = 8;

        // Transferencia para los datos
        spi_transfer[1].tx_buf = reinterpret_cast<unsigned long>(cmd == AAI_CMD ? buffer.data() : nullptr); 
        spi_transfer[1].rx_buf = reinterpret_cast<unsigned long>(cmd == CMD_READ_DATA ? buffer.data() : nullptr);
        spi_transfer[1].len = length;
        spi_transfer[1].speed_hz = get_spi_speed();
        spi_transfer[1].bits_per_word = 8;
        spi_transfer[1].cs_change = 0; // Cambiar a 0 si no es necesario cambiar CS

        // Ejecutar transferencias
        return handle_spi_transfer(spi_transfer, 2);
    }

}