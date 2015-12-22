/**
 * David Lambropoulos
 * Demetrios Lambropoulos
 */

#include	<sys/types.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/socket.h>
#include	<netdb.h>
#include	<netdb.h>
#include	<pthread.h>
#include    <time.h>
#include    <netinet/in.h>
#include    <semaphore.h>
#include    <signal.h>

// Miniature client to exercise getaddrinfo(2).
static int sd;

/**
 * connect_to_server(const char* server, const char* port) takes in the value
 * of a server that is to be connected to through a given port.
 */
int connect_to_server(const char* server, const char* port)
{
	int			sd;	//socket device
	struct addrinfo		addrinfo;
	struct addrinfo *	result;
	char			message[256];

	addrinfo.ai_flags = 0;
	addrinfo.ai_family = AF_INET;		// IPv4 only
	addrinfo.ai_socktype = SOCK_STREAM;	// Want TCP/IP
	addrinfo.ai_protocol = 0;		// Any protocol
	addrinfo.ai_addrlen = 0;
	addrinfo.ai_addr = NULL;
	addrinfo.ai_canonname = NULL;
	addrinfo.ai_next = NULL;
	if ( getaddrinfo( server, port, &addrinfo, &result ) != 0 )
	{
		fprintf( stderr, "\x1b[1;31mgetaddrinfo( %s ) failed.  File %s line %d.\x1b[0m\n", server, __FILE__, __LINE__ );
		return -1;
	}
	else if ( errno = 0, (sd = socket( result->ai_family, result->ai_socktype, result->ai_protocol )) == -1 )
	{
		freeaddrinfo( result );
		return -1;
	}
	else
	{
		do {
			if ( errno = 0, connect( sd, result->ai_addr, result->ai_addrlen ) == -1 )
			{
				sleep(3);
				write( 1, message, sprintf( message, "\x1b[2;33mConnecting to server %s ...\x1b[0m\n", server ) );
			}
			else
			{
				freeaddrinfo( result );
				return sd;		// connect() succeeded
			}
		} while ( errno == ECONNREFUSED );
		freeaddrinfo( result );
		return -1;
	}
}

/**
 * command_input_thread is a thread that handles input from the user and delivers
 * the input to the server.
 */
void* command_input_thread(void* arg)
{
	char			string[2048];
	char			prompt[] = "\nChoose an option\n-------------------------------------------\n1) Create Account - create accountname\n2) Serve Account - serve accountname\n3) Deposit Money - deposit amount\n4) Withdraw Money - withdraw amount\n5) Query - query\n6) End - end\n7) Quit - quit\nEnter Selection: ";
	int			len;
	pthread_detach( pthread_self() );		// Don't join on this thread
	while ( write( 1, prompt, sizeof(prompt) ),(len = read( 0, string, sizeof(string) )) > 0)
	{
		string[len-1]= '\0';
		write( sd, string, strlen( string ) + 1 );
		if(strcmp(string,"quit") == 0)
		{
			return 0;
		}

		sleep(2);
	}

	return 0;
}

/**
 * response_output_thread is a thread that handles output from the server and
 * presents it to the user.
 */
void* response_output_thread(void* arg)
{
	char			output[2048];
	char			buffer[2048];
	int 			len;
	pthread_detach( pthread_self() );		// Don't join on this thread
	while(len = read(sd, buffer, sizeof(buffer)), len >= 0 )
	{
		if(strcmp(buffer, "quit") == 0)
		{
			strcpy(output,"\nGoodbye! :)\n\n");
			write(1, output, strlen(output));
			return 0;
		}
		if(len == 0)
		{
			printf("\n\nServer has unexpectedly crashed!\n\n");
			exit(1);
		}
		sprintf( output, "\nResult is >%s<\n", buffer );
		write( 1, output, strlen(output) );
	//	sleep(2);
	}

	return 0;
}

int main( int argc, char ** argv )
{
	char			message[256];
	pthread_t		tid;
	pthread_attr_t		kernel_attr;
	char* func = "main";
	//int			count;

	if ( argc < 2 )
	{
		fprintf( stderr, "\x1b[1;31mNo host name specified.  File %s line %d.\x1b[0m\n", __FILE__, __LINE__ );
		exit( 1 );
	}
	if ( pthread_attr_init( &kernel_attr ) != 0 )
	{
		printf( "pthread_attr_init() failed in file %s line %d\n", __FILE__, __LINE__ );
		return 0;
	}
	else if ( pthread_attr_setscope( &kernel_attr, PTHREAD_SCOPE_SYSTEM ) != 0 )
	{
		printf( "pthread_attr_setscope() failed in file %s line %d\n", __FILE__, __LINE__ );
		return 0;
	}
	else if ( (sd = connect_to_server( argv[1], "49660" )) == -1 )
	{
		write( 1, message, sprintf( message,  "\x1b[1;31mCould not connect to server %s errno %s\x1b[0m\n", argv[1], strerror( errno ) ) );
		return 1;
	}
	else
	{
		printf( "Connected to server %s\n", argv[1] );
		if (pthread_create(&tid, &kernel_attr, command_input_thread, 0) != 0 )
		{
			printf( "pthread_create() failed in file %s line %d\n", __FILE__, __LINE__ );
			return 0;
		}
		else if(pthread_create( &tid, &kernel_attr, response_output_thread, 0) != 0 )
		{
			printf( "pthread_create() failed in %s()\n", func );
			return 0;
		}
		pthread_exit(0);
		close( sd );
		return 0;
	}
}
