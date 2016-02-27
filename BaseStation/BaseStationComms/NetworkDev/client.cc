#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

using namespace std;

void client()
{
	int sockfd;
	struct hostent *host;

	struct sockaddr_in remote;

	char host_str[100];
	cout << "Enter hostname: ";
	cin >> host_str;

	cin.clear();
	cin.ignore();

	if( (host = gethostbyname(host_str)) == 0)
		cerr << "gethostbyname() error: " << errno << endl;

	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		cerr << "Socket creation error: " << errno << endl;

	int port;
	cout << "Enter port: ";
	cin >> port;

	remote.sin_family = AF_INET;
	remote.sin_port = htons(port);
	remote.sin_addr = *( (struct in_addr*) host->h_addr);

	cout << "Connecting to " << host_str << ":" << port << endl;

	memset( &(remote.sin_zero), 0, 8);
	if( connect(sockfd, (struct sockaddr*) &remote, sizeof(struct sockaddr)) == -1)
	{
		cerr << "Connection error: " << errno << endl;
	}
	else
	{
		cerr << "Connected." << endl;
	}

	cout << endl << endl;

	char temp[1024];
	
	while(1)
	{
		memset(temp, 0, sizeof(temp));
		//Write
		cout << "Enter message: ";
		cin >> temp;

		cout << "Writing: " << temp << endl;
		write(sockfd, &temp, strlen(temp));
		cout << "Write finished" << endl;
		cout << "\n\n";
		
		memset(temp, 0, sizeof(temp));
		//Read
		cout << "Starting read" << endl;
		int n = read(sockfd, &temp, sizeof(temp));
		cout << "Read [" << n << "] bytes [" << temp << "]" << endl;

		if( string(temp).substr(0,5) == "Image")
		{
			cout << "Starting image transfer." << endl;
			//Extract path
			//Open/creat file
			while(1)
			{	
				//Read from network
				memset(temp, 0, sizeof(temp));
				int n = read(sockfd, &temp, sizeof(temp));
				cout << "Read image data [" << temp << "]" << endl;
				if( string(temp) == "Done")
					break;
				//Write to file
			}
			cout << "Image transfer complete." << endl;
		}
		
		cout << "\n\n\n";
	}

	cout << "Client closing" << endl;
	close(sockfd);
}

int main()
{
	client();
	return 0;
}