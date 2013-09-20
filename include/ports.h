#ifndef __FIRE_PORTS
#define __FIRE_PORTS

BYTE inb_p(WORD port);
WORD inw_p(WORD port);
DWORD ind_p(WORD port);

void outb_p(BYTE v,WORD port);
void outw_p(WORD v,WORD port);
void outd_p(DWORD v,WORD port);
#endif