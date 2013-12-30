#ifndef _MEMORY_H_24356_INCLUDED_
#define _MEMORY_H_24356_INCLUDED_
#pragma once

#ifdef TRACE_MEMORY_LEAKS

/* The following code allows to keep track of unallocated memory, by hijacking
 * new and delete operators. In the end of the program, we can list and print 
 * all memory positions that were not deallocated (and are, therefore, memory leaks)
 * Thanks, flipcode!
 */

#ifndef DWORD
#define DWORD unsigned long
#endif

// Replace new with our debug version
#ifdef _DEBUG
	#define DEBUG_NEW new(__FILE__, __LINE__)
#else
	#define DEBUG_NEW new
#endif
#define new DEBUG_NEW


#ifdef _DEBUG

// Add a new memory allocation
void AddTrack(DWORD addr,  DWORD asize,  const char *fname, DWORD lnum);

// Remove a tracked memory allocation
void RemoveTrack(DWORD addr);

// The overloaded new operator
inline void * __cdecl operator new(unsigned int size, const char *file, int line)
{
	void *ptr = (void *)malloc(size);
	AddTrack((DWORD)ptr, size, file, line);
	return(ptr);
};

// The overloaded delete operator
inline void __cdecl operator delete(void *p)
{
	RemoveTrack((DWORD)p);
	free(p);
};

// Dump all the unfreed memory (potential memory leaks)
void DumpUnfreed();

#endif

#endif

#endif