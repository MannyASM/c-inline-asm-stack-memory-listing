// ------------------------------------------------------
// 5/1/2019
//
// List current function stack frame pointers
//		* ask user for how many bytes to display
//		* default to at least 64 bytes
//		* use inline asm
//
// Inline asm to obtain current function frame pointers
// 
//	Visual Studio 2017 with Intel C++ compiler 19.0
//	-----------------------------------------------
//		__asm mov rbpVar, rbp;
//		__asm mov rspVar, rsp;
//
//	gcc compiler
//	-----------------------------------------------
//		__asm__ (".intel_syntax; \n\t"
//				"movq %0, %%rbp; \n\t"
//				"movq %1, %%rsp; \n\t"
//				".att_syntax;"
//				: "=r" (rbpVar), "=r" (rspVar));
// ------------------------------------------------------

#include <stdio.h>
#include <inttypes.h>

typedef unsigned char BYTE;

void hexDump(char *desc, void *addr, int len) {

	int i;
	BYTE buff[9];
	BYTE* pc = (BYTE *)addr;

	if (desc != NULL) {
		printf("\n%s\n", desc);
	}

	for (i = 0; i < len; i++) {

		if ((i % 8) == 0) {
			if (i != 0) {
				printf("  %s\n", buff);
			}
			printf("  [0x%016p] ", &pc[i]);
		}

		printf(" %02x", pc[i]);

		if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
			buff[i % 8] = '.';
		}
		else {
			buff[i % 8] = pc[i];
		}

		buff[(i % 8) + 1] = '\0';
	}

	while ((i % 8) != 0) {
		printf("   ");
		i++;
	}

	printf("  %s\n", buff);

	// ----------------------------------------------
	// list 1 byte
	// ----------------------------------------------

	printf("\n[1 byte hex stack memory listing]\n");

	for (i = 0; i < len; i++) {

		if ((i % 8) == 0) {
			printf("\n(%d)", (i/8) + 1);
		}
		printf("\n  [0x%016p] %02x", &pc[i], pc[i]);
	}
}

int main() {

	printf("start main() - print at least 64 bytes of stack with rbp as anchor\n\n");

	uint64_t rbpVar = 0x1000;
	uint64_t rspVar = 0x2000;

	int i = 0xBEEF;
	int iBytesToPrint;               // no. of stack mem bytes to list

	// get user input 
	printf("Number of bytes to print: ");

	// scanf() reads the formatted input
	scanf_s("%d", &iBytesToPrint);

	// edit user input
	if (iBytesToPrint < 64) {
		iBytesToPrint = 64;
	}

	// ---------------------------------------------------------
	// get current function frame pointers	VS2017
	__asm mov rbpVar, rbp;
    __asm mov rspVar, rsp;

	// ---------------------------------------------------------
	// get current function frame pointers	gcc
	//		__asm__ (".intel_syntax; \n\t"
	//				"movq %0, %%rbp; \n\t"
	//				"movq %1, %%rsp; \n\t"
	//				".att_syntax;"
	//				: "=r" (rbpVar), "=r" (rspVar));

	// ---------------------------------------------------------

	BYTE *a = (BYTE *) rbpVar;

	printf("\t[main() - rbp] 0x%016" PRIx64 "\n", rbpVar);
	printf("\t[main() - rsp] 0x%016" PRIx64 "\n", rspVar);
	printf("\n");
	printf("\nStack memory hex listing");
	printf("\n====================================================");
	printf("\n");
	printf("\n    a pointer to rbpVar                     = %p", a);
	printf("\n    start location is a-(iBytesToPrint / 2) = %p", a - (iBytesToPrint / 2));
	printf("\n    bytes to print                          = %d", iBytesToPrint);
	printf("\n");

	hexDump("[start]\n", a - (iBytesToPrint / 2), iBytesToPrint);

	//printf("\n\n[8 bytes hex memory listing]");
	//hexDump_EightBytesListing(a - 0x20, iBytesToPrint);

	printf("\n\n<done> press any key to continue ...");
	getch();

	return 0;
}

//void hexDump_EightBytesListing(void *addr, int len) {
//
//	int i;
//	int j = 0;
//
//	BYTE* pc = (BYTE *)addr;
//
//	for (i = 0; i < len; i++) {
//
//		if (j == 0) {
//			printf("\n[%d]", i);
//			printf("\n[0x%016p] ", i ,&pc[i]);
//		}
//
//		printf("%02x ", pc[i]);
//
//		if (j == 7) {
//			j = 0;
//		}
//		else {
//			j++;
//		}
//	}
//}
