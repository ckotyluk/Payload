
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
#include <vector>

using namespace std;

const int MAX_SIZE = 1024;

inline  // a simple utility for splitting strings at a find-pattern.
vector<string>
split(const string s, const string pat ) {
    string c;
    vector<string> v;
    int i = 0;
    for (;;) {
        int t = s.find(pat,i);
        int j = ( t == string::npos ) ? s.size() : t;
        string c = s.substr(i,j-i);
        v.push_back(c);
        i = j+pat.size();
        if ( t == string::npos ) return v;
    }
}

void server() //Payload
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

		char temp[MAX_SIZE];

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

			vector<string> v = split(string(temp), ",,,");

			//if( string(temp).substr(0,5) == "Image")
			if(v.at(0) == "Image")
			{
				cout << "Starting image transfer." << endl;
				// Wait for client to accept image transfer
				memset(temp, 0, sizeof(temp));
				int n = read(new_fd, &temp, sizeof(temp));
				if(string(temp) == "Ready")
				{

					//Open source file
					int file_fd = open((const char*)v.at(1).c_str(), O_RDONLY);
					
					memset(temp, 0, sizeof(temp));
					while((n = read(file_fd, temp, MAX_SIZE)) > 0)//Continue while able to read from file
					{	
						write(new_fd, &temp, strlen(temp));

						//Write to network
						
						//cout << "Image data: ";
						//cin >> temp;
						//if(string(temp) == "Done")
						//	break;
						//write(new_fd, &temp, strlen(temp));

					}
					memset(temp, 0, sizeof(temp));
					strncpy(temp, "Done", 4);
					write(new_fd, &temp, strlen(temp));
					cout << "Image transfer complete." << endl;
				}
				else
				{
					cout << "Transfer failed" << endl;
					cout << "Received: " << string(temp) << endl;
				}
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