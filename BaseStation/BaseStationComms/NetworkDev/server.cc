
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

using namespace std;


void server()
{

	int sin_len;
	int sockfd, new_fd;
	struct sockaddr_in local_addr;
	struct sockaddr_in remote_addr;
	int sin_size;

	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		cerr << "Socket creation error: " << errno << endl;

	int port;
	cout << "Enter port: ";
	cin >> port;

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(port);
	local_addr.sin_addr.s_addr = INADDR_ANY;

	memset( &(local_addr.sin_zero), 0, 8);
	if( bind(sockfd, (struct sockaddr*) &local_addr, sizeof(struct sockaddr) ) == -1)
		cerr << "Port bind error: " << errno << endl;

	cout << "Waiting for a connection." << endl;
	if(listen(sockfd,5) == -1)
		cerr << "Listen error: " << errno << endl;

	sin_len = sizeof(struct sockaddr_in);

	while(1)
	{
		if( (new_fd = accept(sockfd, (struct sockaddr*) &remote_addr, (socklen_t*) &sin_len)) == -1)
			cerr << "Accept error: " << errno << endl;
		//TODO: Insepct ip and make sure it is expected ip
		cout << "Connection accepted." << endl;

		char temp[1024];

		while(1)
		{
			memset(temp, 0, sizeof(temp));
			//Read
			cout << "Starting read" << endl;
			int n = read(new_fd, &temp, sizeof(temp));
			cout << "Read [" << n << "] bytes [" << temp << "]" << endl;
			
			//Write
			memset(temp, 0, sizeof(temp));
			cout << "Enter message: ";
			cin >> temp;

			cout << "Writing: " << temp << endl;
			write(new_fd, &temp, strlen(temp));
			cout << "Write finished" << endl;

			if( string(temp).substr(0,5) == "Image")
			{
				cout << "Starting image transfer." << endl;
				//Open source file
				while(1)//Continue will able to read from file
				{	
					//Read from file
					//Write to network
					memset(temp, 0, sizeof(temp));
					cout << "Image data: ";
					cin >> temp;
					if(string(temp) == "Done")
						break;
					write(new_fd, &temp, strlen(temp));

				}
				memset(temp, 0, sizeof(temp));
				strncpy(temp, "Done", 4);
				write(new_fd, &temp, strlen(temp));
				cout << "Image transfer complete." << endl;
			}

			cout << "\n\n\n" << endl;
		}

		close(new_fd);
	}
	close(sockfd);
	cout << "Server closing." << endl;
}

int main()
{
	server();
	return 0;
}