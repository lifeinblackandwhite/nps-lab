#include<stdio.h>


unsigned short compute_checksum_sender() {
    unsigned fields[10];
    int i;
    unsigned int sum = 0;
    printf("Enter 10 IP header fields in 16 bit hex (sender):\n");
    for(i = 0; i < 10; i++) {
        scanf("%x", &fields[i]);
        sum += (unsigned short)fields[i];
        while (sum >> 16)
            sum = (sum & 0xFFFF) + (sum >> 16);
    }
    sum = ~sum;
    return (unsigned short)sum;
}

unsigned short compute_checksum_receiver() {
    unsigned fields[11];
    int i;
    unsigned int sum = 0;
    printf("Enter 10 IP header fields + checksum in 16 bit hex (receiver):\n");
    for(i = 0; i < 11; i++) {
        scanf("%x", &fields[i]);
        sum += (unsigned short)fields[i];
        while (sum >> 16)
            sum = (sum & 0xFFFF) + (sum >> 16);
    }
    sum = ~sum;
    return (unsigned short)sum;
}


int main() {
    unsigned short sender_checksum, receiver_result;

    // Sender
    sender_checksum = compute_checksum_sender();
    printf("\nComputed Checksum at sender: %04x\n", sender_checksum);
    printf("\nNow enter the same 10 fields plus the checksum as the 11th value for receiver check.\n");

    // Receiver
    receiver_result = compute_checksum_receiver();
    printf("\nComputed Checksum at receiver: %04x\n", receiver_result);

    if (receiver_result == 0x0000)
        printf("\nNo error in Data received\n");
    else
        printf("\nError in Data received\n");

    return 0;
}