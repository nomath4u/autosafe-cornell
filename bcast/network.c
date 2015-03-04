#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<openssl/hmac.h>

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data
char key[] = "cornellcup2015";

void die(char *s)
{
    perror(s);
    exit(1);
}

char* concat (char* first, char* second){
    int len1 = strlen(first);
    int len2 = strlen(second);
    printf("%s, %s, %d, %d\n", first, second, len1, len2);
    char* s = (char*)malloc((sizeof(char) * (len1 + len2 + 2)));
    //memcpy(s, &first, len1);
    sprintf(s, "%s:%s", first, second);
    //s[len1] = ':';
    //memcpy(s + len1 + 1, &second, len2 + 1); // includes terminating null
    return s;
}

char* crypt (unsigned char* message){
    unsigned char* digest;
    unsigned int md_len;
    char* s;
    digest = HMAC(EVP_sha1(), key, strlen(key), message, strlen(message), NULL, &md_len);
    printf("Len: %d ", md_len);
    char mdString[md_len*2+5];
    for(int i = 0; i < md_len; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    printf("Digest: %s, Message:%s\n", mdString, message);
    sprintf(s, "%s", mdString);
    return s;
}

int send_message(char* who, char message[BUFLEN])
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char* send;
    send = crypt((unsigned char*)message);
    printf("Send = %s\n", concat(send, (char *)message));
    
    /*
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("Socket creation failed: Send Message");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);


    if (inet_aton(who , &si_other.sin_addr) == 0) 
    {
        die("inet_aton() failed\n");
    }

    //send the message
    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    {
        die("sendto() failed: Send Message");
    }

    close(s);
    */
    return 0;

}

int main(void)
{
    send_message("Nobody", "This is my message");
    /*
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
            send_message("10.0.0.2", "OH NO!");
            send_message("10.0.0.3", "OH NO!");
            //exit(1);
        }

        // Send message to others
        //send_message(buf);
    }

    close(s);
    */
    return 0;
}
