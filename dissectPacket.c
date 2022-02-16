#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Author: Aaron Starr
// Dissect network packet code

void ReadFile(FILE *file);
bool isFileEmpty(FILE *file);

void ReadFile(FILE *file){
	unsigned char *buffer;
	unsigned long fileLen;

	//file = fopen("packets.1", "rb");
	//file = fopen("test.txt", "rb");
	
	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);

	//Allocates
	buffer=(unsigned char *)malloc(fileLen+1);
	if(!buffer){
		fprintf(stderr, "buffer not created");
        fclose(file);
		exit(EXIT_FAILURE);
	}

	fread(buffer, fileLen, 1, file);
	fclose(file);

	//unsigned long buffer_size = fileLen;
	//printf("%ld\n", fileLen);
	
	//printf("%ld\n", buffer_size);
	
	//char s[] = (unsigned char *)buffer[0];
	
	unsigned char size1 = ((unsigned char *)buffer)[0];
	unsigned char size2 = (((unsigned char *)buffer)[1]);
	unsigned char size3 = (((unsigned char *)buffer)[2]);
	unsigned char size4 = (((unsigned char *)buffer)[3]);
	unsigned int c = (((size3 << 16) | (size4 << 24)) | (size2 << 8)) | size1 << 0;
	int numPackets = c >> 0;
	
	
	//int f;
	//f = GetNumber(buffer);
	//printf("\n%d\n\n\n", f);
	//char str[] = ((unsigned char *)buffer)[0];
	printf("%d Packets.\n",numPackets);
	int i;
	int j;
	for(i = 8,j = 8 ; i < numPackets+8 ; i++){
		printf("==>Packet %d\n",i-7);
		//for(i = 8;i < buffer_size;++i){
			
			///VERSION
			unsigned char vers = (((unsigned char *)buffer)[j] & 0xF0);
			int num = vers >> 4;
			if(num == 6){printf("Theres a version 6 in here.. uhh\n");}
			printf("Version:\t\t 0x%01X (%d)\n",vers>>4, num);
			///HEADER LENGTH (IHL)
			unsigned char hlen = (((unsigned char *)buffer)[j] & 0x0F);
			int num2 = hlen >> 0;
			printf("IHL (Header Length):\t\t 0x%01X (%d)\n",hlen>>0, num2);
			///TYPE OF SERVICE (TOS)
			unsigned char tos = (((unsigned char *)buffer)[j+1]);
			int num3 = tos >> 0;
			printf("Type of Service (TOS):\t\t 0x%01X (%d)\n",tos>>0, num3);
			
			j += 3;
			///TOTAL LENGTH
			unsigned char tlen = (((unsigned char *)buffer)[j-1]);
			unsigned char tlen2 = (((unsigned char *)buffer)[j]);
			unsigned int num4 = tlen2 | (tlen << 8);
			printf("Total Length:\t\t 0x%01X (%d)\n",num4, num4);
			
			j += 1;
			unsigned int nibble = j; 
			while (nibble <= j+num4){
				///IDENTIFICATION
				unsigned char iden = (((unsigned char *)buffer)[nibble]);
				unsigned char iden2 = (((unsigned char *)buffer)[nibble+1]);
				unsigned int num5 = iden2 | (iden << 8);
				printf("Identification:\t\t 0x%01X (%d)\n",num5, num5);
				
				nibble += 2;
				
				///IP FLAGS
				unsigned char ipf = (((unsigned char *)buffer)[nibble]);
				unsigned char ipf2 = (((unsigned char *)buffer)[nibble+1] & 0xF0);
				//ipf2 = ipf2 >> 4;
				unsigned int num6 = ipf | (ipf2 << 8);
				printf("IP Flags:\t\t 0x%01X (%d)\n",num6, num6);

				nibble += 2;
				
				///FRAGMENT OFFSET
				unsigned char foff = (((unsigned char *)buffer)[nibble-1] & 0x0F);
				unsigned int num7 = foff;
				printf("Fragment Offset:\t\t 0x%01X (%d)\n",num7, num7);
				
				///TIME TO LIVE (TTL)
				unsigned char ttl = (((unsigned char *)buffer)[nibble]);
				unsigned int num8 = ttl;
				printf("Time to Live (TTL):\t\t 0x%01X (%d)\n",num8, num8);
				
				nibble += 1;
				
				///PROTOCOL
				unsigned char pr = (((unsigned char *)buffer)[nibble]);
				unsigned int num9 = pr;
				char protoID[6];
				if (num9 == 1){strcpy(protoID, "ICMP\0");}
				else if (num9 == 2){strcpy(protoID, "IGMP\0");}
				else if (num9 == 6){strcpy(protoID, "TCP\0");}
				else if (num9 == 9){strcpy(protoID, "IGRP\0");}
				else if (num9 == 17){strcpy(protoID, "UDP\0");}
				else if (num9 == 47){strcpy(protoID, "GRE\0");}
				else if (num9 == 50){strcpy(protoID, "ESP\0");}
				else if (num9 == 51){strcpy(protoID, "AH\0");}
				else if (num9 == 57){strcpy(protoID, "SKIP\0");}
				else if (num9 == 88){strcpy(protoID, "EIGRP\0");}
				else if (num9 == 89){strcpy(protoID, "OSPF\0");}
				else if (num9 == 115){strcpy(protoID, "L2TP\0");}
				printf("Protocol:\t\t %s 0x%01X (%d)\n",protoID, num9, num9);
				
				nibble += 1;
				
				///HEADER CHECKSUM
				unsigned char hsum = (((unsigned char *)buffer)[nibble]);
				unsigned char hsum2 = (((unsigned char *)buffer)[nibble+1]);
				unsigned int numa = hsum2 | (hsum << 8);
				printf("Header Checksum:\t\t 0x%01X (%d)\n",numa, numa);
				
				nibble += 2;
				
				///SOURCE ADDRESS
				unsigned char sadd = (((unsigned char *)buffer)[nibble]);
				unsigned char sadd2 = (((unsigned char *)buffer)[nibble+1]);
				unsigned char sadd3 = (((unsigned char *)buffer)[nibble+2]);
				unsigned char sadd4 = (((unsigned char *)buffer)[nibble+3]);
				printf("Source Address:\t\t %d.%d.%d.%d\n",sadd, sadd2, sadd3, sadd4);
				
				nibble += 4;
				
				///DESTINATION ADDRESS
				unsigned char dadd = (((unsigned char *)buffer)[nibble]);
				unsigned char dadd2 = (((unsigned char *)buffer)[nibble+1]);
				unsigned char dadd3 = (((unsigned char *)buffer)[nibble+2]);
				unsigned char dadd4 = (((unsigned char *)buffer)[nibble+3]);
				printf("Destination Address:\t\t %d.%d.%d.%d\n",dadd, dadd2, dadd3, dadd4);
				
				//rest of the nibbles are ignored from the looks of it (data part I think)
				break;
			}
			j = j+num4;
			
			///USED FOR PRINTING ALL BITS, I code on a windows system, so life is hard.
		//printf("%02X : ", ((unsigned char *)buffer)[i]);
		//if((i % 16 == 0) && (i != 0)){printf("\n");}
		//}
	}

	free(buffer);
}

bool isFileEmpty(FILE *file){
	
    long startingthing = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0){
        return true;
    }
    fseek(file, startingthing, SEEK_SET);
    return false;
}

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "-usage [filename]");
		exit(EXIT_FAILURE);
	}
	
	FILE *file = fopen(argv[1], "rb");
	
	if(isFileEmpty(file) == true){
		fprintf(stderr, "emptyfile given");
		exit(EXIT_FAILURE);
	}
	if (!file){
		fprintf(stderr, "-usage [filename]");
		exit(EXIT_FAILURE);
	}
	printf("==== File %s contains ", argv[1]);
	ReadFile(file);
	exit(EXIT_SUCCESS);
}