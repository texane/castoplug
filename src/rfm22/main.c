#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "./castoplug.c"

int main(void)
{
  int i;

  castoplug_setup();

  while (1)
  {
    /* turn off the a:3 wall outlet */
    /* send 5 consecutive frames for redundancy */
    for (i = 0; i != 5; ++i)
    {
      castoplug_send(a, 3, off);
      _delay_ms(10);
    }

    _delay_ms(500);
    _delay_ms(500);
  }

  return 0;
}
