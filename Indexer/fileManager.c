#include "fileManager.h"
 

/**
 * Checks if a given path is a regular file.
 */
int isRegularFile(const char* path)
{	
	struct stat buf;
    	stat(path, &buf);
		
	return S_ISREG(buf.st_mode);
}

/**
 * Checks if a given path is a directory.
 */
int isDirectory(const char* path)
{
	struct stat buf;
    	stat(path, &buf);
	
	return S_ISDIR(buf.st_mode);
}

/**
 * Checks if a given path is a character device.
 */
int isCharacterDevice(const char* path)
{	
	struct stat buf;
    	stat(path, &buf);
	
	return S_ISCHR(buf.st_mode);
}

/**
 * Checks if a given path is a block device.
 */
int isBlockDevice(const char* path)
{	
	struct stat buf;
    	stat(path, &buf);
	
	return S_ISBLK(buf.st_mode);
}

/** 
 * Checks if a given path is a pipe.
 */
int isPipe(const char* path)
{
	struct stat buf;
    	stat(path, &buf);
	
	return S_ISFIFO(buf.st_mode);
}

/**
 * Checks if a given path is a symbolic link.
 */
int isSymbolicLink(const char* path)
{	
	struct stat buf;
    	stat(path, &buf);
	
	return S_ISLNK(buf.st_mode);
}

/**
int isSocket(const char* path)
{
	int status;	
	struct stat buf;
    	stat(path, &buf);
	
	status = S_ISSOCK(buf.st_mode);
	
	if(status == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}	
}

**/

/**
 * Coverts a file located with a given path into a string
 * so that the contents can be worked with without the user needing 
 * to do extra work.
 */
char* fileToString(const char* path)
{	
	
	char* string = calloc(1, sizeof(char *));
	int ch = 0;
	int index = 0;
	
	//Determine whether or not the path is a file
	if(isRegularFile(path))
	{
		//Attempt to open file
		FILE* fp = fopen(path, "r");
		
		//If the file is NULL proceed no further			
		if(fp != NULL)
		{
			//Define a string to hold contents of a file
			string = calloc(bufferSize(fp), sizeof(char *));
			while((ch = fgetc(fp)) != EOF)
			{
				if((char) ch != '\n' || (char) ch != '\t' || (char) ch != '\v' || (char) ch != '\f')
				{
					//printf("char %d %c: \n", index, (char) ch);
					string[index] = (char) ch;
					index++;
				}
				else
				{
					string[index] = ' ';
					index++;
				}
			}

		}
		
		//Close the file
		fclose(fp);	
	
	}
	// Return the file contents as a string.
	return string;
	
}

/**
 * Counts and returns the number of binary files located in a directory.
 */
int numBinaryFileInDirectory(const char* path)
{
	int binaryFiles = 0;

	//Determine whether or not the path is a directory
	if(isDirectory(path))
	{
		//Attempt to open the directory
		DIR* dp = opendir(path);
		struct dirent* entry;		
		
		//If the directory is NULL proceed no further.
		if(dp != NULL)
		{
			//Walk through the directory.
			while((entry = readdir(dp)) != NULL)
			{
				//Ignore the parent and child files
				if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				{
					continue;
				}
				
				char newPath[strlen(path)+1+strlen(entry->d_name)];
				strcpy(newPath, path);
				strcat(newPath, "/");
				strcat(newPath, entry->d_name);
				
				if(isBinary(newPath))
				{
					binaryFiles++;
				}
			}
		}
		
		//Close directory
		closedir(dp);
	}
	
	return binaryFiles;
}

/**
 * Counts and returns the number of files that are located in directory
 * located at the given path.
 */
int numFilesInDirectory(const char* path)
{
	int count = 0;
	
	//Determine whether or not the path is a directory
	if(isDirectory(path))
	{
		//Attempt to open the directory
		DIR* dp = opendir(path);
		struct dirent* entry;		
		
		//If the directory is NULL proceed no further.
		if(dp != NULL)
		{
			//Walk through the directory.
			while((entry = readdir(dp)) != NULL)
			{
				//Ignore the parent and child files
				if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				{
					continue;
				}

				count++;
			}
		}
		
		//Close directory
		closedir(dp);
	}

	return count;
	
}

/**
 * Checks if the a file located at a given path is a binary file.
 */
int isBinary(const char* path)
{
	int binary = 0;
	int ch;
	if(isRegularFile(path))
	{
		FILE* fp = fopen(path, "r");
		if(fp != NULL)
		{
			while((ch = fgetc(fp)) != EOF)
			{
				if(isprint(ch) == 0 && (char) ch != '\n' && (char) ch != '\t' && (char) ch != '\v' && (char) ch != '\n' && (char) ch != '\f' && (char) ch != '\b' && (char) ch != '\r' && (char) ch != '\'' && (char) ch != '\\' && (char) ch != '\"' && (char) ch != '\a') 
				{
					binary = 1;
					break;
				}
			}
		}
	}

	return binary;
}

/**
 * Converts a given directory to a double char pointer of the contents located within the files in the directory.
 */
char** directoryToString(const char* path)
{
	char** strings = calloc(4*(numFilesInDirectory(path)-numBinaryFileInDirectory(path)),sizeof(char *));
	int index = 0;
	
	//Determine whether or not path is a directory.
	if(isDirectory(path))
	{
		//Attempt to open directory.
		DIR* dp = opendir(path);
		struct dirent* entry;		
		
		
		if(dp != NULL)
		{
			while((entry = readdir(dp)) != NULL)
 			{
				if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				{
					continue;
				}

				char newPath[strlen(path)+1+strlen(entry->d_name)];
				strcpy(newPath, path);
				strcat(newPath, "/");
				strcat(newPath, entry->d_name);
				
				if(isBinary(newPath) == 0)
				{
					strings[index] = fileToString(newPath);
					index++;
				}

			}
		}
		
		closedir(dp);
	}

	return strings;
}

/**
 * Prints the files located in a directory at a given file path.
 */
void printFilesInDirectory(const char* path)
{
	int index = 1;

	if(isDirectory(path))
	{
		DIR* dp = opendir(path);
		struct dirent* entry;		
		
		if(dp != NULL)
		{
			while((entry = readdir(dp)) != NULL)
			{
				if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				{
					continue;
				}

				char newPath[strlen(path)+1+strlen(entry->d_name)];
				strcpy(newPath, path);
				strcat(newPath, "/");
				strcat(newPath, entry->d_name);

				if(isBinary(newPath))
				{
					printf("File %d: %s (BINARY)\n", index, entry->d_name);
				}
				else 
				{
					printf("File %d: %s\n", index, entry->d_name);
				}
				index++;
			}
		}
		
		closedir(dp);
	}
}

char** filesInDirectory(const char* path)
{
	int numFiles = numFilesInDirectory(path) - numBinaryFileInDirectory(path);	
	char** files = calloc(numFiles,sizeof(char*));
	int index = 0;
	
	if(isDirectory(path))
	{
		DIR* dp = opendir(path);
		struct dirent* entry;		
		
		if(dp != NULL)
		{
			while((entry = readdir(dp)) != NULL)
			{
				if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				{
					continue;
				}

				char newPath[strlen(path)+1+strlen(entry->d_name)];
				strcpy(newPath, path);
				strcat(newPath, "/");
				strcat(newPath, entry->d_name);

				if(!isBinary(newPath))
				{
					files[index] = entry->d_name;
					index++;
				}
			}
		}
		
		closedir(dp);
	}

	return files;
}

/**
 * Returns buffer size.
 */
long int bufferSize(FILE* fp)
{
	long int BUFSIZE = 0;
	
	//Obtain the size of a file
	fseek(fp, 0, SEEK_END);
	BUFSIZE = ftell(fp);
	rewind(fp);

	return BUFSIZE;
}

/**
 * Check if a file located at a given path exists.
 */
int Exists(const char* path)
{
	int exists = 0;

	if(isRegularFile(path))
	{	
		FILE* fp = fopen(path, "r");
	
		if(fp != NULL)
		{
			exists = 1;
		}
		
		fclose(fp);	
	}
	else if(isDirectory(path))
	{
		DIR* dp = opendir(path);

		if(dp != NULL)
		{
			exists = 1;
		}

		closedir(dp);
	}	

	return exists;

}
