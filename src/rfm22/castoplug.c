#ifndef CASTOPLUG_C_INCLUDED
#define CASTOPLUG_C_INCLUDED


#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


/* rfm22 */

/* pinout, from rfm22.pdf */

/* spi.csn is nsel */
/* spi.sck is sck */
/* spi.mosi is sdi */
/* spi.miso is sdo */
/* sdn to gnd (always on) */
/* tx_ant to gpio_0 */
/* rx_ant to gpio_1 */

#define SOFTSPI_MODE_0

#define SOFTSPI_CLK_DDR DDRC
#define SOFTSPI_CLK_PORT PORTC
#define SOFTSPI_CLK_MASK (1 << 1)
#define SOFTSPI_MOSI_DDR DDRC
#define SOFTSPI_MOSI_PORT PORTC
#define SOFTSPI_MOSI_MASK (1 << 2)
#define SOFTSPI_MISO_DDR DDRC
#define SOFTSPI_MISO_PIN PINC
#define SOFTSPI_MISO_PORT PORTC
#define SOFTSPI_MISO_MASK (1 << 3)

#include "./softspi.c"

#define RFM22_IO_CSN_DDR DDRC
#define RFM22_IO_CSN_PORT PORTC
#define RFM22_IO_CSN_MASK (1 << 0)

#if 0 /* unused */
#define RFM22_IO_SDN_MASK (1 << 4)
#define RFM22_IO_SDN_DDR DDRD
#define RFM22_IO_SDN_PORT PORTD
#define RFM22_IO_TXANT_MASK (1 << 5)
#define RFM22_IO_TXANT_DDR DDRD
#define RFM22_IO_TXANT_PORT PORTD
#define RFM22_IO_RXANT_MASK (1 << 6)
#define RFM22_IO_RXANT_DDR DDRD
#define RFM22_IO_RXANT_PORT PORTD
#endif /* unused */

static inline void rfm22_csn_setup(void)
{
  RFM22_IO_CSN_DDR |= RFM22_IO_CSN_MASK;
}

static inline void rfm22_csn_low(void)
{
  RFM22_IO_CSN_PORT &= ~RFM22_IO_CSN_MASK;
}

static inline void rfm22_csn_high(void)
{
  RFM22_IO_CSN_PORT |= RFM22_IO_CSN_MASK;
}

static inline void rfm22_csn_wait(void)
{
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
  __asm__ __volatile__ ("nop\n");
}

static uint8_t rfm22_read_reg(uint8_t i)
{
  /* i the register address */

  /* rfm22.pdf, ch 3 */

  uint8_t x;

  rfm22_csn_low();
  rfm22_csn_wait();
  softspi_write_uint8(i);
  x = softspi_read_uint8();
  rfm22_csn_high();

  return x;
}

static void rfm22_write_reg(uint8_t i, uint8_t x)
{
  /* i the register address */
  /* x the register value */

  rfm22_csn_low();
  rfm22_csn_wait();
  softspi_write_uint8((1 << 7) | i);
  softspi_write_uint8(x);
  rfm22_csn_high();
}

static inline uint8_t rfm22_read_vc(void)
{
  /* vendor code */
  return rfm22_read_reg(0x01);
}

static inline uint8_t rfm22_read_ds(void)
{
  /* device status */
  return rfm22_read_reg(0x02);
}

static inline uint8_t rfm22_read_is1(void)
{
  /* interrupt status 1 */
  return rfm22_read_reg(0x03);
}

static inline void rfm22_write_ie2(uint8_t x)
{
  /* interrupt enable 2 */
  rfm22_write_reg(0x06, x);  
}

static inline void rfm22_write_omfc1(uint8_t x)
{
  /* operating mode and function control 1 */
  rfm22_write_reg(0x07, x);
}

static inline void rfm22_write_omfc2(uint8_t x)
{
  /* operating mode and function control 2 */
  rfm22_write_reg(0x08, x);
}

static inline uint8_t rfm22_read_omfc2(void)
{
  /* operating mode and function control 2 */
  return rfm22_read_reg(0x08);
}

static inline void rfm22_write_gc0(uint8_t x)
{
  /* gpio configuration 0 */
  rfm22_write_reg(0x0b, x);
}

static inline void rfm22_write_gc1(uint8_t x)
{
  /* gpio configuration 1 */
  rfm22_write_reg(0x0c, x);
}

static inline void rfm22_write_dac(uint8_t x)
{
  /* data access control */
  rfm22_write_reg(0x30, x);
}

static inline void rfm22_write_hc1(uint8_t x)
{
  /* header control 1 */
  rfm22_write_reg(0x32, x);
}

static inline void rfm22_write_hc2(uint8_t x)
{
  /* header control 2 */
  rfm22_write_reg(0x33, x);
}

static inline void rfm22_write_pl(uint8_t x)
{
  /* preamble length */
  rfm22_write_reg(0x34, x);
}

static inline void rfm22_write_tpow(uint8_t x)
{
  /* tx power */
  rfm22_write_reg(0x6d, x);
}

static inline void rfm22_write_trate(uint16_t x)
{
  /* tx data rate */
  rfm22_write_reg(0x6e, (uint8_t)(x >> 8));
  rfm22_write_reg(0x6f, (uint8_t)(x >> 0));
}

static inline void rfm22_write_mmc1(uint8_t x)
{
  /* modulation mode control 1 */
  rfm22_write_reg(0x70, x);
}

static inline void rfm22_write_mmc2(uint8_t x)
{
  /* modulation mode control 2 */
  rfm22_write_reg(0x71, x);
}

static inline void rfm22_write_fo(uint16_t x)
{
  /* frequency offset */
  rfm22_write_reg(0x73, (uint8_t)(x >> 0));
  rfm22_write_reg(0x74, (uint8_t)(x >> 8));
}

static inline uint8_t rfm22_read_fbs(void)
{
  /* frequency band select */
  return rfm22_read_reg(0x75);
}

static inline void rfm22_write_fbs(uint8_t x)
{
  /* frequency band select */
  rfm22_write_reg(0x75, x);
}

static inline void rfm22_write_ncf(uint16_t x)
{
  /* nominal carrier frequency */
  rfm22_write_reg(0x76, (uint8_t)(x >> 8));
  rfm22_write_reg(0x77, (uint8_t)(x >> 0));
}

static inline uint8_t rfm22_read_ncf1(void)
{
  /* nominal carrier frequency 1 */
  return rfm22_read_reg(0x76);
}

static inline void rfm22_write_tfc1(uint8_t x)
{
  /* tx fifo control 1 */
  rfm22_write_reg(0x7c, x);
}

static inline void rfm22_write_rfc(uint8_t x)
{
  /* rx fifo control */
  rfm22_write_reg(0x7e, x);
}

static inline uint8_t rfm22_read_fa(void)
{
  /* fifo access */
  return rfm22_read_reg(0x7f);
}

static inline void rfm22_write_fa(uint8_t x)
{
  /* fifo access */
  return rfm22_write_reg(0x7f, x);
}

static void rfm22_reset_fifo(void)
{
  const uint8_t x =  rfm22_read_omfc2();
  rfm22_write_omfc2(x | (1 << 1) | (1 << 0));
  rfm22_write_omfc2(x & ~((1 << 1) | (1 << 0)));
}

static void rfm22_setup(void)
{
  softspi_setup_master();
  rfm22_csn_setup();

  /* gpio configuration */
  rfm22_write_gc0(0x12);
  rfm22_write_gc1(0x15);

  /* crystal frequency capacitance */
  rfm22_write_reg(0x09, 0x7f);

  /* pll, tx, xtal on */
  rfm22_write_omfc1((1 << 3) | (1 << 1) | (1 << 0));

  /* automatic transmission */
  rfm22_write_omfc2(1 << 3);

  /* disable interrupts */
  rfm22_write_ie2(0x00);

  /* tx send size to 4 */
  rfm22_write_tfc1(0x04);

  /* tx power */
  rfm22_write_tpow(0x08);

  /* 433.92 MHz carrier */
  /* refer to rfm22.pdf, 3.6 */
  rfm22_write_fo(0);
  rfm22_write_fbs(19);
#if 0
  rfm22_write_ncf(25088);
#else
  /* -40KHz */
  rfm22_write_ncf(25340);
#endif

  /* ook modulation */
  /* refer to rfm22.pdf, 7.2 */
#if 0 /* fifo_mode */
  rfm22_write_mmc1((1 << 5) | (1 << 1));
  rfm22_write_mmc2((2 << 4) | (1 << 0));
#else
  /* direct asynchronous mode */
  /* miso used for tx data clock */
  /* mosi used for tx data */
  rfm22_write_mmc1(0 << 5);
  rfm22_write_mmc2((0 << 6) | (1 << 4) | (1 << 0));
#endif

  /* 40Kbps tx data rate */
  rfm22_write_trate(0xffff);

  /* minimal rx packet handler */
  /* receive anything after preamble and sync */
  rfm22_write_dac(0x00);
  /* disable checks */
  rfm22_write_hc1(0x00);
  rfm22_write_hc2(0x00);
  rfm22_write_pl(0x00);

  rfm22_reset_fifo();
}

static void rfm22_send(const uint8_t* x, uint8_t n)
{
  uint8_t i;
  for (i = 0; i != n; ++i, ++x) rfm22_write_fa(*x);
}


/* castoplug setup must be called first */

static void castoplug_setup(void)
{
  rfm22_setup();
}

/* castoplug_send_xxx routines */

#define castoplug_send_pulse(__thigh, __tlow)	\
do {						\
  /* miso used for tx data clock */		\
  /* mosi used for tx data */			\
						\
  /* thigh, tlow in us */			\
						\
  SOFTSPI_MOSI_PORT |= SOFTSPI_MOSI_MASK;	\
  _delay_us(__thigh);				\
						\
  SOFTSPI_MOSI_PORT &= ~SOFTSPI_MOSI_MASK;	\
  _delay_us(__tlow);				\
						\
} while (0)

static inline void castoplug_send_pulse_a(void)
{
  castoplug_send_pulse(400, 940);
}

static inline void castoplug_send_pulse_b(void)
{
  castoplug_send_pulse(1005, 340);
}

static inline void castoplug_send_group_a(void)
{
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
}

static inline void castoplug_send_group_b(void)
{
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
}

static inline void castoplug_send_group_c(void)
{
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
}

static inline void castoplug_send_group_d(void)
{
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
}

static inline void castoplug_send_dev_1(void)
{
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
}

static inline void castoplug_send_dev_2(void)
{
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
}

static inline void castoplug_send_dev_3(void)
{
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
}

static inline void castoplug_send_cmd_on(void)
{
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_b();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
}

static inline void castoplug_send_cmd_off(void)
{
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_b();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
  castoplug_send_pulse_a();
}

#define castoplug_send(__g, __d, __c)	\
do {					\
  castoplug_send_group_ ## __g();	\
  castoplug_send_dev_ ## __d();		\
  castoplug_send_cmd_ ## __c();		\
} while (0)


#endif /* CASTOPLUG_C_INCLUDED */
