#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#define PORT 0x3f8
void connectToCOM1() {
	outp(PORT + 1, 0x00);
	outp(PORT + 3, 0x80);
	outp(PORT + 0, 0x03);
	outp(PORT + 1, 0x00);
	outp(PORT + 3, 0x03);
	outp(PORT + 2, 0xc7);
	outp(PORT + 4, 0x0b);
}
void listenForByte() {
	int data_in, data_out;
	int LSR;
	LSR = inp(PORT+5);
	if(LSR & 1) {
		data_in = inp(PORT);
		printf("%c",data_in);
	}
}
void writeByte(unsigned char d) {
	int data_in, data_out;
	data_out = d;
	outp(PORT, data_out);
}