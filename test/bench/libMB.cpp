#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <Windows.h>

#define ZZmain
#include "zzincl.h"

//#define USE_UTIL_CLEAR

//Entity classes
class Library
{    
    ZZ_EXT_Library
public:    
};

//--------------------------
class Genre
{
    ZZ_EXT_Genre
public:
};

//--------------------------
class Book
{
    ZZ_EXT_Book
public:
    unsigned order; //
};

//--------------------------
class Chapter
{
    ZZ_EXT_Chapter
public:
};

//--------------------------
class GenToBook {
    ZZ_EXT_GenToBook
public:
};

ZZ_HYPER_DOUBLE_AGGREGATE(listBooksByName, Library, Book);
ZZ_HYPER_DOUBLE_AGGREGATE(listBooksByOrder, Library, Book);
//ZZ_HYPER_ARRAY(arrBooksByName, Library, Book*); 
//ZZ_HYPER_ARRAY(arrBooksByOrder, Library, Book*); 
ZZ_HYPER_ARRAY(genres,Library,Genre*);
ZZ_HYPER_SINGLE_COLLECT(chaptersByName, Book, Chapter);
ZZ_HYPER_ARRAY(borrowingTimes,Book,int);
//ZZ_HYPER_SINGLE_COLLECT(booksInGenre, Genre, GenToBook); 
ZZ_HYPER_M_TO_N(booksInGenre, Genre,GenToBook, Book);
//ZZ_HYPER_SINGLE_LINK(genToBook, GenToBook, Book);
ZZ_HYPER_NAME(bookName, Book);
ZZ_HYPER_NAME(genName, Genre);
ZZ_HYPER_NAME(chapName, Chapter);
ZZ_HYPER_UTILITIES(util);

//genres
const char * genre_names[] = {
    "Sci-fi",
    "Fairy tales",
    "Crimi",
    "Belles-lettres",
    "Adventure"
};
const int genres_count = 5;

#define BOOKS_COUNT         1000000
#define CHAPTERS_MAX_COUNT  10
#define MAX_BORROWINGS 50
#define MAX_BORROWING_TIME 30

#define LIBRARY_FILE    "library_dol.dat"
#define LIBRARY_FILE2    "library2_dol.dat"

//some utility functions
unsigned GetTimeMillis();
void Log(const char *msg, ...);
void Help();

class Result
{
public:
    char testName[128];
    int testNo;
    unsigned time;
    Result():testNo(0), time(0)
    {
        memset(testName, 0, sizeof(testName));
    }
};
#define RESULTS_COUNT 12
Result g_Results[RESULTS_COUNT];
unsigned g_ResultsCount = 0;


void CreateLibrary();
void OpenLibrary();
void AddResult(int testNo, const char* name, unsigned time);
void PrintResults();


//comparators
int CompListBooksByName(const void *b1, const void *b2);
int CompArrBooksByName(const void *b1, const void *b2);
int CompListBooksByOrder(const void *b1, const void *b2);
int CompArrBooksByOrder(const void *b1, const void *b2);
int CompChaptersByName(const void *c1, const void *c2);

int main(int argc, char *argv[])
{
    util.mode(0, 0, 0, 0);
    util.blkAlloc(BOOKS_COUNT * 4096, 16);

    bool create = true;

    if(argc>1 && tolower(argv[1][0])=='c')
    {
        create = true;
    }
    else if(argc>1 && tolower(argv[1][0])=='o')
    {
        create = false;        
    }
    else
    {
        Help();
        return -1;
    }
    
    if(create)
    {
        CreateLibrary();
    }
    else
    {
        OpenLibrary();
    }

    PrintResults();
    
    Log("\n\nDOL benchmark done");
    getchar();

    return 0;
}


void CreateLibrary()
{
    char *v[1],*t[1];
    GenToBook *gtb; char *buf;
    Chapter *ch; Genre *ge, **gg;
    int i,j,count;
    char bname[128], chname[128]; 
	unsigned start, time1, time2;
    Library *lib = NULL;    

    remove(LIBRARY_FILE);
    remove(LIBRARY_FILE2);

    srand((unsigned)time(NULL));

    start = time1 = GetTimeMillis();
    
	//-----------------------------------------------------------------------
    Log("**** Creating the library... ****\n");    lib = new Library();   
    genres.form(lib, genres_count, -2);
   
    //Genres
    for(i = 0; i < genres_count; i++)
    {        
	Genre **gg;
	ge=new Genre();
        buf = util.strAlloc(genre_names[i]);
        genName.add(ge, buf);
        gg=genres.ind(lib,i);
	*gg=ge;
    }
    
	int start_idx;
    for(i = 0; i < BOOKS_COUNT; i++)
    {
        bname[0] = '\0'; // empty string
		//generate random book name
        sprintf(bname, "%c%c book %u\0", (rand() % 26) + 65, (rand() % 26) + 65, i);

        Book *b = new Book;
        buf=util.strAlloc(bname);
        bookName.add(b,buf);
        b->order = rand();
        
        listBooksByName.add(lib, b);
        listBooksByOrder.add(lib, b);            
       
		//add book to the random number of genres 
		count = (rand() % genres_count) +1;
        //index of the first genre
        start_idx = rand() % genres_count; 
        for(j = 0; j < count; j++)
        {
            gtb=new GenToBook;         
            int idx = (start_idx + j) % genres_count;
            gg=genres.ind(lib,idx);
            booksInGenre.add(*gg, gtb, b);
        }
       
        //book's chapters
        count = (rand() % CHAPTERS_MAX_COUNT) + 1;
        int page = 1;
        for(int j = 0; j < count; j++)
        {
            chname[0] = '\0'; // empty string
            ch = new Chapter;
			//generate random chapter name
            sprintf(chname, "%c chapter %u\0", (rand() % 26) + 65, j+1);
            ch = new Chapter;
            buf=util.strAlloc(chname);
            chapName.add(ch,buf);       
            chaptersByName.add(b, ch);
        }
        
		//borrowing times
		count = (rand() % MAX_BORROWINGS) + 1;
		borrowingTimes.form(b, count, 0);
		int *lt = borrowingTimes.ind(b, 0);
		for (int j = 0; j < count; j++)
		{
			lt[j] = (rand() % MAX_BORROWING_TIME) + 1;
		}
		if((i+1)%10000 == 0)
		{
			Log("%d books created.\n", (i+1));
		}
    }
	time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
	AddResult(1, "Library creation", time2-time1);

	//-----------------------------------------------------------------------
	Log("\n\n**** Indexing the library... ****\n");
	
    Log("Sorting books list by name...\n");
    time1 = GetTimeMillis();
    unsigned time3 = time1;
    listBooksByName.sort(CompListBooksByName, lib);
    unsigned time4 = GetTimeMillis();
    Log("Done list by name. Time: %d ms.", time4-time3);

    Log("Sorting books list by order...\n");
    time3 = GetTimeMillis();
    listBooksByOrder.sort(CompListBooksByOrder, lib);
    time4 = GetTimeMillis();
    Log("Done list by order. Time: %d ms.", time4-time3);
        
    time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
	AddResult(2, "Library indexation", time2-time1);

	//-----------------------------------------------------------------------
	Log("\n\n**** Serializing the library... ****\n");
	time1 = GetTimeMillis();	
    v[0]=(char*)lib;
    t[0]=(char*)"Library";    
    //util.mode(0, 1, 0, 0);
	util.save(LIBRARY_FILE, 1, v, t);
	time2 = GetTimeMillis();
    Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
    AddResult(3, "Library saving", time2-time1);

	//free the library    

    //PM: zde to spadne == uz ne JS
	util.clear(1, v, t);
}

void OpenLibrary()
{
    char *v[1],*t[1];
    GenToBook *gtb; char *buf;
    Chapter *ch; Genre **gg;
    int i,j,count;
	unsigned start, time1, time2;
    Library *lib = NULL;
    int testIdx = 0;
    char testName[128];

    start = GetTimeMillis();
    time1 = start;

    //-----------------------------------------------------------------------
	Log("\n\n**** Loading the serialized library... ****\n");
	
    //util.mode(0, 1, 0, 0);
 	util.open(LIBRARY_FILE, 1, v, t);
    if(util.error()!=0)
    {
        Log("Error while opening file %s.\n", LIBRARY_FILE);
        return;
    }
	lib = (Library*)v[0];
	time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
	AddResult(5, "Library reopen", time2-time1);
    
    //-----------------------------------------------------------------------
    Log("\n\n**** Number of books in genres ****\n");            
    time1 = GetTimeMillis();
	int gen_counts[genres_count] = {0,};
    gtb = NULL;
    for(i = 0; i < genres_count; i++)
    {
        gg=genres.ind(lib, i);
        booksInGenre_sIterator itbg(*gg);
        while(gtb=++itbg)
        {
            gen_counts[i]++;
        }
        Log("Number of books in genre '%s': %d\n", genre_names[i], gen_counts[i]);
    }    
    time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
    AddResult(6, "Number of books in genres", time2-time1);
    
    //-----------------------------------------------------------------------
	const char *find = "MZ book";
    Log("\n\n**** The average number of chapters for books with name starting with '%s'. Iterating list. ****\n", find);
	time1 = GetTimeMillis();
    {   
        listBooksByName_iterator itbbn(lib);
        Book *b = NULL;
        int cmp;
        int sumb = 0;
        int sumchap;
        count = 0;
        bool begin_found = false;                
        ITERATE(itbbn, b)
        {
            buf = bookName.fwd(b);
            cmp = strncmp(find, buf, strlen(find));
            if(cmp==0)
            {
                begin_found = true;
                //count the chapters
                chaptersByName_iterator itch(b);
                sumchap = 0;                
                ITERATE(itch, ch)
                {                    
                    sumchap++;
                }
                sumb += sumchap;
                count++;                
            }
            else if(begin_found)//we have reached the end of the "MZ book" block
            {                
                break;
            }
        }

        Log("The average number of chapters: %f\n", sumb/(float)count);        
    }
    time2 = GetTimeMillis();
	Log("Done list. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
    sprintf(testName, "The avg. num. of chapt. for books named '%s'*", find);
    AddResult(7,  testName, time2-time1);

    //-----------------------------------------------------------------------
	unsigned ordStart = 123;
	unsigned ordEnd = 12345;
    Log("\n\n**** Number of books with order in <%d, %d>. Iterate list.****\n", ordStart, ordEnd);
	time1 = GetTimeMillis();
    {
        listBooksByOrder_iterator itbo(lib);
        Book *b;
        count = 0;
        ITERATE(itbo, b)
        {
            if(b->order >= ordStart)
            {
                count++;
            }
            if(b->order >= ordEnd)
            {
                break;
            }
        }
    }
    time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
    sprintf(testName, "The num. of books with order in <%d, %d>", ordStart, ordEnd);
    AddResult(8,  testName, time2-time1);

    //-----------------------------------------------------------------------
	int chapCnt = CHAPTERS_MAX_COUNT / 2;
    find = "Q chapter";
    Log("\n\n**** The number of books having chapters count >= %d and containing chapter name prefixed by '%s'. Iterating list. ****\n", chapCnt, find);
	time1 = GetTimeMillis();
    {
        count = 0;
        int chapCount;
        listBooksByName_iterator itb(lib);
        Book *b;
        Chapter *c;
        chaptersByName_iterator itc;
        bool chapFound;
        char *chName;
        ITERATE(itb, b)
        {            
            itc.start(b);
            chapCount = 0;
            chapFound = false;
            ITERATE(itc, c)
            {
                if(!chapFound)
                {
                    chName = chapName.fwd(c);
                    if(strncmp(chName, find, strlen(find))==0)
                    {
                        chapFound = true;
                    }
                }
                chapCount++;
            }
            if(chapFound && chapCount>=chapCnt)
            {
                count++;
            }
        }
        Log("The number of books (byName): %d\n", count);
    }
    time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
    sprintf(testName, "The num. of books with ch. '%s' and ch. cnt>=%d", find, chapCnt);
    AddResult(9,  testName, time2-time1);

	//-----------------------------------------------------------------------
	Log("\n\n**** The max. borrowing time ****\n");
	time1 = GetTimeMillis();
	{
		listBooksByName_iterator itbbn(lib);
		int length, size, sum, maxSum=0;		
		Book *b=NULL;
		int *arr;
        ITERATE(itbbn, b)
		{
			length = borrowingTimes.size(b, &length, &size);
			arr = borrowingTimes.ind(b, 0);
			sum = 0;
			for( int i=0; i<length; i++)
			{
				sum += arr[i];
			}			
			maxSum = maxSum < sum ? sum : maxSum;
		}
		Log("Max. borrowing time %d\n", maxSum);
	}
	time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
	AddResult(10,  "Longest borrowing time", time2-time1);

    //-----------------------------------------------------------------------
	Log("\n\n**** Remove each second book from the library ****\n");
	time1 = GetTimeMillis();
    {
        int wMark, incr;
        char *name;
        Book *b;  
        Chapter *ch;
	Genre **gg;
        chaptersByName_iterator itch;
        listBooksByName_iterator itbn(lib);
        i=0;
        j=0;            
            
        ITERATE(itbn, b)
        {
            if((i++)%2==1)
            {
                //remove book from genres
                GenToBook *gtb;
                for(j = 0; j < genres_count; j++)
                {
                    gg = genres.ind(lib, j);
                    booksInGenre_sIterator itbig(*gg);

                    ITERATE(itbig, gtb)
                    {
                        booksInGenre.del(gtb);
                        delete gtb;
                    }
                }

#ifndef USE_UTIL_CLEAR
             
                //delete chapters
                itch.start(b);
                ITERATE(itch, ch)
                {
                    name = chapName.del(ch);
                    util.strFree(name);
                    chaptersByName.del(b, ch);
                    delete ch;
                }

                //delete borrowing times
                borrowingTimes.free(b);

                //delete book's name
                name = bookName.del(b);
                util.strFree(name);

                //remove book from library
                listBooksByName.del(b);
                listBooksByOrder.del(b);

                //delete books
                delete b;
#else
		// JS
		// This entire section is wrong. util.clear() is not for
		// removing specified individual objects, it is a global
		// function which removes typically all the data.
                listBooksByName.del(b);
                listBooksByOrder.del(b);

                //TOO SLOW == uz ne JS
                v[0] = (char*)b;
                t[0] = "Book";
                util.clear(1, v, t);
#endif
            }
        }
    }
    time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
    AddResult(11,  "Library reduction", time2-time1);

    //save the reduced library

    v[0] = (char*)lib;
    t[0] = "Library";
    util.save(LIBRARY_FILE2, 1, v, t);
}

void AddResult(int testNo, const char* name, unsigned time)
{
    g_Results[g_ResultsCount].testNo = testNo;
    strcpy(g_Results[g_ResultsCount].testName, name);
    g_Results[g_ResultsCount].time = time;
    g_ResultsCount++;
}

void PrintResults()
{
    printf("\nBENCHMARK RESULTS:\n\n");

    printf("----------------------------------------------------------------------------\n");
    printf("TestNo. | Test Name                                             | Time[ms] |\n");
    printf("============================================================================\n");
    for(unsigned i = 0; i < g_ResultsCount; i++)
    {
        printf("   %02d   | %-53s | %-8d |\n", g_Results[i].testNo, g_Results[i].testName, g_Results[i].time);       
    }
    printf("----------------------------------------------------------------------------\n");
}



unsigned GetTimeMillis()
{
	LARGE_INTEGER now;	
	static bool first = true;
	static LARGE_INTEGER freq;
	if(first)
	{
		QueryPerformanceFrequency(&freq);		
		first = false;
	}

	QueryPerformanceCounter(&now);
	now.QuadPart = now.QuadPart/(freq.QuadPart/ 1000);
	return (unsigned)now.QuadPart;
}

void Log(const char *msg, ...)
{
        va_list argptr;
        va_start(argptr, msg);
        vprintf(msg, argptr);
        va_end(argptr);
}

int CompListBooksByName(const void *b1, const void *b2)
{
    Book *book1 = (Book*)b1;
    Book *book2 = (Book*)b2;
    const char *s1 = bookName.fwd(book1);
    const char *s2 = bookName.fwd(book2);
    return strcmp(s1, s2);
}

int CompArrBooksByName(const void *b1, const void *b2)
{
    Book *book1 = *((Book**)b1);
    Book *book2 = *((Book**)b2);
    const char *s1 = bookName.fwd(book1);
    const char *s2 = bookName.fwd(book2);
    return strcmp(s1, s2);
}

int CompListBooksByOrder(const void *b1, const void *b2)
{
    Book *book1 = (Book*)b1;
    Book *book2 = (Book*)b2;

    return book1->order - book2->order;
}

int CompArrBooksByOrder(const void *b1, const void *b2)
{
    Book *book1 = *((Book**)b1);
    Book *book2 = *((Book**)b2);
    return book1->order - book2->order;
}

int CompChaptersByName(const void *c1, const void *c2)
{
    Chapter *chap1 = (Chapter*)c1;
    Chapter *chap2 = (Chapter*)c2;
    const char *s1 = chapName.fwd(chap1);
    const char *s2 = chapName.fwd(chap2);
    return strcmp(s1, s2);
}

void Help()
{
    printf("Usage: DOLTest.exe [par]\n");
    printf("par: (required) either 'c' or 'o', where 'c' means 'create library' and 'o' means 'open library'\n");    
}

#include "zzfunc.c"
