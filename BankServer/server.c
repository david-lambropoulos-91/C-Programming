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
#include	<pthread.h>
#include    <sys/time.h>
#include    <netinet/in.h>
#include    <semaphore.h>
#include    <signal.h>
#include	<ctype.h>


static sem_t		actionCycleSemaphore;
static pthread_mutex_t	mutex;
static int		connection_count = 0;

int printAccounts();

/**
 * The structure of an individual account is whether or not it is locked, the name related to
 * the owner of that account and the balance held by the account.
 */
struct BankAccount_
{
	char name[100];
	double balance;
	int inUse;
	pthread_mutex_t accountmutex;
};
typedef struct BankAccount_ BankAccount;

/**
 * The structure of a bank is whether it is locked, how many open account are at the bank and
 * an array of 20 possible bank accounts that can be held at this bank.
 */
struct Bank_
{
	pthread_mutex_t bankmutex;
	BankAccount accounts[20];
	int openAccounts;
};

typedef struct Bank_* bank;

bank Bank;

/* Miniature server to exercise getaddrinfo(2).*/

/**
 * The Initializer sets up the structure of the bank account by allocating memory and initializing
 * variables.
 */
void TheInitializer()
{

	Bank = calloc(1,sizeof(bank));
	Bank->openAccounts = 0;

}

//Print the accounts currently held at the bank.
int printAccounts()
{
	int i = Bank->openAccounts;
	int j = 0;

	while(pthread_mutex_trylock(&Bank->bankmutex) != 0)
	{
		sleep(1);
	}

	if(i == 0)
	{
		printf("There are no current accounts held with this bank!\n");
		pthread_mutex_unlock(&Bank->bankmutex);
		return -1;
	}

	printf("There are %d taken accounts and %d free accounts.",i,(20-i));

	while(j != i)
	{
		printf("\n\nBANK ACCOUNT %d\n",(j+1));
		printf("-----------------\n");
		printf("Account Holder: %s\n",Bank->accounts[j].name);
		printf("Balance: $%.2f\n", Bank->accounts[j].balance);
		printf("In Use: %d\n\n", Bank->accounts[j].inUse);
		j++;
	}

	pthread_mutex_unlock(&Bank->bankmutex);

	return 0;
}

/**
 * Destroy all memory allocated for the bank
 */
void TheDestroyer()
{
	free(Bank);
}

/**
 * Given a port number claim_port will claim the port for the server
 */
int claim_port( const char * port )
{
	struct addrinfo		addrinfo;
	struct addrinfo *	result;
	int			sd;
	char			message[256];
	int			on = 1;

	addrinfo.ai_flags = AI_PASSIVE;		/* for bind() */
	addrinfo.ai_family = AF_INET;		/* IPv4 only  */
	addrinfo.ai_socktype = SOCK_STREAM;	/* Want TCP/IP */
	addrinfo.ai_protocol = 0;		/* Any protocol */
	addrinfo.ai_addrlen = 0;
	addrinfo.ai_addr = NULL;
	addrinfo.ai_canonname = NULL;
	addrinfo.ai_next = NULL;
	if ( getaddrinfo( 0, port, &addrinfo, &result ) != 0 )		/* want port 3000 */
	{
		fprintf( stderr, "\x1b[1;31mgetaddrinfo( %s ) failed errno is %s.  File %s line %d.\x1b[0m\n", port, strerror( errno ), __FILE__, __LINE__ );
		return -1;
	}
	else if ( errno = 0, (sd = socket( result->ai_family, result->ai_socktype, result->ai_protocol )) == -1 )
	{
		write( 1, message, sprintf( message, "socket() failed.  File %s line %d.\n", __FILE__, __LINE__ ) );
		freeaddrinfo( result );
		return -1;
	}
	else if ( setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) ) == -1 )
	{
		write( 1, message, sprintf( message, "setsockopt() failed.  File %s line %d.\n", __FILE__, __LINE__ ) );
		freeaddrinfo( result );
		close( sd );
		return -1;
	}
	else if ( bind( sd, result->ai_addr, result->ai_addrlen ) == -1 )
	{
		freeaddrinfo( result );
		close( sd );
		write( 1, message, sprintf( message, "\x1b[2;33mBinding to port %s ...\x1b[0m\n", port ) );
		return -1;
	}
	else
	{
		write( 1, message, sprintf( message,  "\x1b[1;32mSUCCESS : Bind to port %s\x1b[0m\n", port ) );
		freeaddrinfo( result );
		return sd;			/* bind() succeeded; */
	}
}

static char *ps( unsigned int x, char * s, char * p )
{
	return x == 1 ? s : p;
}

void periodic_action_handler( int signo, siginfo_t * ignore, void * ignore2 )
{
	if ( signo == SIGALRM )
	{
		sem_post( &actionCycleSemaphore );		/* Perfectly safe to do ... */
	}
}

void * periodic_action_cycle_thread( void * ignore )
{
	struct sigaction	action;
	struct itimerval	interval;

	pthread_detach( pthread_self() );			/* Don't wait for me, Argentina ... */
	action.sa_flags = SA_SIGINFO | SA_RESTART;
	action.sa_sigaction = periodic_action_handler;
	sigemptyset( &action.sa_mask );
	sigaction( SIGALRM, &action, 0 );			/* invoke periodic_action_handler() when timer expires */
	interval.it_interval.tv_sec = 20;
	interval.it_interval.tv_usec = 0;
	interval.it_value.tv_sec = 20;
	interval.it_value.tv_usec = 0;
	setitimer( ITIMER_REAL, &interval, 0 );			/* every 3 seconds */
	for ( ;; )
	{
		sem_wait( &actionCycleSemaphore );		/* Block until posted */
		pthread_mutex_lock( &mutex );
		printf( "There %s %d active %s.\n", ps( connection_count, "is", "are" ),
			connection_count, ps( connection_count, "connection", "connections" ) );
		printAccounts();
		pthread_mutex_unlock( &mutex );
		sched_yield();					/* necessary? */
	}
	return 0;
}

/**
 * Take in a name and determines whether it is of correct format
 * such that correct format states that is no more than a 100
 * characters and only contains spaces and alphabetic characters.
 *
 */
int isCorrect(char* test)
{
	int i = 0;

	if(!isalpha(test[i]))
	{
				return -1;
	}

	while(isalpha(test[i]) != 0 || test[i] == ' ')
	{
		if(i == 100)
		{
			return -1;
		}

		i++;
	}
	if((isalpha(test[i]) == 0) && (test[i] != '\0'))
	{
		return -1;
	}
	return 0;
}

/**
 * Returns the account number 0-19 based on the name of
 * the account.
 */
int AccountNumber(char* test)
{
	int i = 0;

	while(strcmp((Bank->accounts[i]).name, test) != 0)
	{
		i++;
	}

	return i;
}

/**
 * Returns a boolean value whether or not an account is
 * existing or not by searching through the bank account
 * with the name.
 */
int isAccount(char* test)
{
	int i = 0;

	if(Bank->openAccounts > 0)
	{
		while(i < Bank->openAccounts)
		{
			if(strcmp((Bank->accounts[i]).name,test) == 0)
			{
				return 0;
			}

			i++;
		}
	}

	return -1;
}

/**
 * Returns a boolean value whether or not a given
 * value is of appropriate format for withdrawls
 * and deposits.
 */
int isBadAmount(char* test)
{
	int i = 0;
	int decimalCount = 0;

	for(; i<strlen(test); i++)
	{
		if(test[i] == '.')
		{
			decimalCount++;
		}
		else if(isdigit(test[i]) == 0)
		{
			return 0;
		}

		if(decimalCount > 1)
		{
			return 0;
		}
	}

	return -1;
}

void* client_session_thread(void* arg)
{
	int			sd;
	char			request[2048];
	int servingAccountFlag = 0;
	int accountBeingServiced;
	char			response[2048];
	char* dummy;
	char* temp;
	double amount;
	sd = *(int *)arg;
	free( arg );					// keeping to memory management covenant
	pthread_detach( pthread_self() );		// Don't join on this thread
	pthread_mutex_lock( &mutex );
	++connection_count;				// multiple clients protected access
	pthread_mutex_unlock( &mutex );
	while ( read( sd, request, sizeof(request) ) > 0 )
	{
		printf( "server receives input:  %s\n", request );

		if(dummy = strtok(request," "), dummy == NULL)
		{
			strcpy(response, "NOTHING ENTERED");
		}
		else if(strcmp(dummy, "create") == 0)
		{
			if(servingAccountFlag == 1)
			{
				strcpy(response, "CANNOT CREATE WHILE SERVICING AN ACCOUNT");
			}
			else if(dummy = strtok(NULL," "), dummy == NULL)
			{
				strcpy(response, "NO ARGUMENTS PROVIDED TO CREATE");
			}
			else if(temp = strtok(NULL, "\0"), temp != NULL)
			{
				sprintf(dummy, "%s %s", dummy, temp++);

				if(isCorrect(dummy) == -1)
				{
					sprintf(response,"ARGUMENT, %s, PROVIDED TO CREATE IS NOT OF CORRECT FORMAT", dummy);
				}
				else if(isAccount(dummy) == 0)
				{
					strcpy(response, "DUPLICATE ACCOUNT, AN ACCOUNT IS ALREADY HELD BY THAT NAME!");
				}
				else
				{
					while(pthread_mutex_trylock(&Bank->bankmutex) != 0)
					{
						sleep(1);
					}
					if(Bank->openAccounts == 20)
					{
						pthread_mutex_unlock(&Bank->bankmutex);
						strcpy(response, "The maximum number of accounts are currently held! No more accounts can be created at this time.");
					}
					else
					{
						strcpy(response, "ACCOUNT WAS SUCCESSFUL CREATED!");
						strcpy((Bank->accounts[Bank->openAccounts]).name, dummy);
						(Bank->accounts[Bank->openAccounts]).balance = 0;
						(Bank->accounts[Bank->openAccounts]).inUse = 0;
						Bank->openAccounts++;
						pthread_mutex_unlock(&Bank->bankmutex);
					}
				}

			}
			else if(isCorrect(dummy) == -1)
			{
				sprintf(response,"ARGUMENT, %s, PROVIDED TO CREATE IS NOT OF CORRECT FORMAT", dummy);
			}
			else if(isAccount(dummy) == 0)
			{
				strcpy(response, "DUPLICATE ACCOUNT, AN ACCOUNT IS ALREADY HELD BY THAT NAME!");
			}
			else
			{
				while(pthread_mutex_trylock(&Bank->bankmutex) != 0)
				{
					sleep(1);
				}
				if(Bank->openAccounts == 20)
				{
					pthread_mutex_unlock(&Bank->bankmutex);
					strcpy(response, "The maximum number of accounts are currently held! No more accounts can be created at this time.");
				}
				else
				{
					strcpy(response, "ACCOUNT WAS SUCCESSFULLY CREATED!");
					strcpy((Bank->accounts[Bank->openAccounts]).name, dummy);
					(Bank->accounts[Bank->openAccounts]).balance = 0;
					(Bank->accounts[Bank->openAccounts]).inUse = 0;
					Bank->openAccounts++;
					pthread_mutex_unlock(&Bank->bankmutex);
				}
			}
		}
		else if(strcmp(dummy, "serve") == 0)
		{
			printf("Here");
			if(servingAccountFlag == 1)
			{
				strcpy(response, "CANNOT SERVICE AN ACCOUNT WHILE SERVICING AN ACCOUNT");
			}
			else if(dummy = strtok(NULL," "), dummy == NULL)
			{
				strcpy(response, "NO ARGUMENTS PROVIDED TO SERVICE");
			}
			else if(temp = strtok(NULL, "\0"), temp != NULL)
			{
				sprintf(dummy, "%s %s", dummy, temp++);

				if(isCorrect(dummy) == -1)
				{
					sprintf(response,"ARGUMENT, %s, PROVIDED TO CREATE IS NOT OF SERVICE FORMAT", dummy);
				}
				else if(isAccount(dummy) == -1)
				{
					sprintf(response,"THERE IS NO ACCOUNT BY THE NAME OF %s!", dummy);
				}
				else
				{
					while(pthread_mutex_trylock(&Bank->bankmutex) != 0)
					{
						sleep(1);
					}
					if((Bank->accounts[AccountNumber(dummy)]).inUse == 1)
					{
						pthread_mutex_unlock(&Bank->bankmutex);
						strcpy(response, "THE ACCOUNT SPECIFIED IS ALREADY BEING SERVED!");
					}
					else if((Bank->accounts[AccountNumber(dummy)]).inUse == 0)
					{
						accountBeingServiced = AccountNumber(dummy);
						(Bank->accounts[accountBeingServiced]).inUse = 1;
						pthread_mutex_lock(&(Bank->accounts[accountBeingServiced]).accountmutex);
						servingAccountFlag = 1;
						pthread_mutex_unlock(&Bank->bankmutex);
						sprintf(response,"CURRENTLY SERVING %s", dummy);
					}
					else
					{
						pthread_mutex_unlock(&Bank->bankmutex);
						strcpy(response, "THE ACCOUNT SPECIFIED IS ALREADY BEING SERVED!");
					}
				}

			}
			else if(isCorrect(dummy) == -1)
			{
				sprintf(response,"ARGUMENT, %s, PROVIDED TO CREATE IS NOT OF CORRECT FORMAT", dummy);
			}
			else if(isAccount(dummy) == -1)
			{
				sprintf(response,"THERE IS NO ACCOUNT BY THE NAME OF %s!", dummy);
			}
			else
			{
				while(pthread_mutex_trylock(&Bank->bankmutex) != 0)
				{
					sleep(1);
				}
				if((Bank->accounts[AccountNumber(dummy)]).inUse == 1)
				{
					pthread_mutex_unlock(&Bank->bankmutex);
					strcpy(response, "THE ACCOUNT SPECIFIED IS ALREADY BEING SERVED!");
				}
				else if((Bank->accounts[AccountNumber(dummy)]).inUse == 0)
				{
					accountBeingServiced = AccountNumber(dummy);
					(Bank->accounts[accountBeingServiced]).inUse = 1;
					pthread_mutex_lock(&(Bank->accounts[accountBeingServiced]).accountmutex);
					servingAccountFlag = 1;
					pthread_mutex_unlock(&Bank->bankmutex);
					sprintf(response,"CURRENTLY SERVING %s", dummy);
				}
				else
				{
					pthread_mutex_unlock(&Bank->bankmutex);
					strcpy(response, "THE ACCOUNT SPECIFIED IS ALREADY BEING SERVED!");
				}
			}
		}
		else if(strcmp(dummy, "deposit") == 0)
		{
			if(servingAccountFlag == 0)
			{
				strcpy(response, "NOT CURRENTLY SERVING AN ACCOUNT ");
			}
			else if(dummy = strtok(NULL," "), dummy == NULL)
			{
				strcpy(response, "NO ARGUMENTS PROVIDED TO DEPOSIT");
			}
			else if(strtok(NULL, "\0") != NULL)
			{
				strcpy(response, "TOO MANY ARGUMENTS PROVIDED TO DEPOSIT");
			}
			else
			{
				  if (dummy[0] == '-')
				  {  // Handle negative integers
					  strcpy(response, "CANNOT DEPOSIT A NEGATIVE AMOUNT!");
				  }
				  else
				  {
					  if(isBadAmount(dummy) != 0)
					  {
						  amount = atof(dummy);
						  (Bank->accounts[accountBeingServiced]).balance += amount;
						  sprintf(response, "SUCCESFULLY DEPOSITED $%0.2f IN %s ACCOUNT!", amount, (Bank->accounts[accountBeingServiced]).name);
					  }
					  else
					  {
						  strcpy(response, "INCORRECT FORMAT TO DEPOSIT");
					  }
				  }
			}
		}
		else if(strcmp(dummy, "withdraw") == 0)
		{
			if(servingAccountFlag == 0)
			{
				strcpy(response, "NOT CURRENTLY SERVING AN ACCOUNT ");
			}
			else if(dummy = strtok(NULL," "), dummy == NULL)
			{
				strcpy(response, "NO ARGUMENTS PROVIDED TO WITHDRAW");
			}
			else if(strtok(NULL, "\0") != NULL)
			{
				strcpy(response, "TOO MANY ARGUMENTS PROVIDED TO WITHDRAW");
			}
			else
			{
				  if (dummy[0] == '-')
				  {  // Handle negative integers
					  strcpy(response, "CANNOT WITHDRAW A NEGATIVE AMOUNT!");
				  }
				  else
				  {
					  if(isBadAmount(dummy) != 0)
					  {
						  amount = atof(dummy);
						  if((Bank->accounts[accountBeingServiced]).balance >= amount)
						  {
							  (Bank->accounts[accountBeingServiced]).balance -= amount;
							  sprintf(response, "SUCCESFULLY WITHDREW $%0.2f IN %s ACCOUNT!", amount, (Bank->accounts[accountBeingServiced]).name);
						  }
						  else
						  {
							  strcpy(response, "CANNOT WITHDRAW MORE MONEY THAN CRRENTLY HELD IN THE ACCOUNT!");
						  }
					  }
					  else
					  {
						  strcpy(response, "INCORRECT FORMAT TO DEPOSIT");
					  }
				  }
			}
		}
		else if(strcmp(dummy, "query") == 0)
		{
			if(servingAccountFlag == 0)
			{
				strcpy(response, "NOT CURRENTLY SERVING AN ACCOUNT ");
			}
			else if(dummy = strtok(NULL," "), dummy != NULL)
			{
				strcpy(response, "NO ARGUMENTS ARE TO BE PROVIDED TO QUERY");
			}
			else
			{
				sprintf(response, "%s CURRENTLY HAS $%0.2f IN THE BANK!", (Bank->accounts[accountBeingServiced]).name, (Bank->accounts[accountBeingServiced]).balance);
			}
		}
		else if(strcmp(dummy, "end") == 0)
		{
			if(servingAccountFlag == 0)
			{
				strcpy(response, "NOT CURRENTLY SERVING AN ACCOUNT ");
			}
			else if(dummy = strtok(NULL," "), dummy != NULL)
			{
				strcpy(response, "NO ARGUMENTS ARE TO BE PROVIDED TO END");
			}
			else
			{
				sprintf(response, "NO LONGER SERVING %s", (Bank->accounts[accountBeingServiced]).name);
				(Bank->accounts[accountBeingServiced]).inUse = 0;
				servingAccountFlag = 0;
			}
		}
		else if(strcmp(dummy, "quit") == 0)
		{
			if(dummy = strtok(NULL," "), dummy != NULL)
			{
				strcpy(response, "NO ARGUMENTS ARE TO BE PROVIDED TO END");
			}
			else
			{
				strcpy(response, "quit");
			}
		}
		else
		{
			strcpy(response, "NOT A VALID COMMAND!");
		}
		write( sd, response, strlen(response)+1);
	}
	close( sd );
	pthread_mutex_lock( &mutex );
	--connection_count;				// multiple clients protected access
	pthread_mutex_unlock( &mutex );
	return 0;
}

int main( int argc, char ** argv )
{
	int			sd;
	char			message[256];
	pthread_t		tid;
	pthread_attr_t		kernel_attr;
	socklen_t		ic;
	int			fd;
	struct sockaddr_in      senderAddr;
	int *			fdptr;
	char* func = "main";

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
	else if ( (sd = claim_port( "49660" )) == -1 )
	{
		write( 1, message, sprintf( message,  "\x1b[1;31mCould not bind to port %s errno %s\x1b[0m\n", "3000", strerror( errno ) ) );
		return 1;
	}
	else if ( listen( sd, 100 ) == -1 )
	{
		printf( "listen() failed in file %s line %d\n", __FILE__, __LINE__ );
		close( sd );
		return 0;
	}
	else
	{
		ic = sizeof(senderAddr);
		TheInitializer();
		while ( (fd = accept( sd, (struct sockaddr *)&senderAddr, &ic )) != -1 )
		{
			fdptr = (int *)malloc( sizeof(int) );
			*fdptr = fd;					// pointers are not the same size as ints any more.
			if ( pthread_create( &tid, &kernel_attr, client_session_thread, fdptr ) != 0 )
			{
				printf( "pthread_create() failed in file %s line %d\n", __FILE__, __LINE__ );
				return 0;
			}
			else if ( pthread_create( &tid, &kernel_attr, periodic_action_cycle_thread, 0 ) != 0 )
			{
				printf( "pthread_create() failed in %s()\n", func );
				return 0;
			}
			else
			{
				continue;
			}
		}
		close( sd );
		TheDestroyer();
		return 0;
	}
}
