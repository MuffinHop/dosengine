#include "src\misc\definer.h"

unsigned int visible_page=0;
unsigned int non_visible_page=320*200;
word temp;
/*Wait for vertical blank 60hz, flips address*/
void wait_for_retrace(void) {
    /* wait until done with vertical retrace */
    while  ((inp(INPUT_STATUS) & VRETRACE)) {};
    push_videobuffer();
    /* wait until done refreshing */
    while (!(inp(INPUT_STATUS) & VRETRACE)) {};
}
/*Wait for a specified number of clock ticks (18hz)*/
void wait(int ticks) {
  word start;
  start=*my_clock;
  while (*my_clock-start<ticks) {
    *my_clock=*my_clock;              /* this line is for some compilers
                                         that would otherwise ignore this
                                         loop */
  }
}
