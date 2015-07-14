/*
 * publish_subscribe.c
 *
 *  Created on: Jul 14, 2015
 *      Author: vassik
 */

#include <stdio.h>

#include "../ThingMLMQTT.h"
#include "MQTTAsync.h"


int main(int argc, char* argv[]) {
	int ch;
	do {
		ch = getchar();

		switch (ch) {
			case 'c' : {
				printf("Connecting client...\n");
			}; break;
		}
	} while (ch != 'q');

	printf("Goodbye!\n");
}
