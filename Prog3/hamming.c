#include<stdio.h>
int main() {
	int data[8] = {0}, recv[8] = {0};
	int c, c0, c1, c2, i;
	printf("Enter four data bits (space-separated, e.g. 1 0 1 1): ");
	scanf("%d %d %d %d", &data[3], &data[5], &data[6], &data[7]);

	// Calculate parity bits
	data[1] = (data[3] + data[5] + data[7]) % 2;
	data[2] = (data[3] + data[6] + data[7]) % 2;
	data[4] = (data[5] + data[6] + data[7]) % 2;

	printf("The Data encoded (7 bits) is: ");
	for (i = 1; i < 8; i++) {
		printf("%d", data[i]);
	}
	printf("\n");

	printf("Enter received 7 bits (space-separated, e.g. 0 1 1 0 0 1 1): ");
	for (i = 1; i < 8; i++) {
		scanf("%d", &recv[i]);
	}

	// Calculate syndrome bits
	c0 = (recv[1] + recv[3] + recv[5] + recv[7]) % 2;
	c1 = (recv[2] + recv[3] + recv[6] + recv[7]) % 2;
	c2 = (recv[4] + recv[5] + recv[6] + recv[7]) % 2;
	c = c2 * 4 + c1 * 2 + c0 * 1;

	if (c == 0) {
		printf("No error detected in received data.\n");
	} else if (c > 0 && c < 8) {
		printf("Error detected at position %d. Correcting...\n", c);
		recv[c] ^= 1;
	} else {
		printf("Invalid syndrome or input.\n");
	}

	printf("The Data after correction is: ");
	for (i = 1; i < 8; i++) {
		printf("%d", recv[i]);
	}
	printf("\n");

	printf("Extracted original data bits: %d %d %d %d\n", recv[3], recv[5], recv[6], recv[7]);

	return 0;
}
