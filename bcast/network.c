#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data
#define SERVER "10.0.0.2"

void die(char *s)
{
    perror(s);
    exit(1);
}

int send_message(char message[BUFLEN])
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("Socket creation failed: Send Message");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        die("inet_aton() failed\n");
    }

    //send the message
    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    {
        die("sendto() failed: Send Message");
    }

    close(s);
    return 0;

}

int main(void)
{
    struct sockaddr_in si_me, si_other;

    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("Socket creation failed: Server");
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = INADDR_ANY;
    //si_me.sin_addr.s_addr = INADDR_BROADCAST;

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }

    //keep listening for data
    while(1)
    {
        //for (i=0; i < BUFLEN ; i++) {
        //    buf[i] = "";
        //}
        printf("Waiting for data...");
        fflush(stdout);

        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
        buf[recv_len] = 0;

        si_other.sin_addr.s_addr = INADDR_BROADCAST; 
        si_other.sin_family = AF_INET;
        //si_other.sin_port = htons(PORT);

        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);


        // Parse Message and do stuff
        if (buf[0] == '!') {
            printf("Crash!\n");
            fflush(stdout);
            send_message("OH NO!");
            exit(1);
        }

        // Send message to others
        //send_message(buf);
    }

    close(s);
    return 0;
}
