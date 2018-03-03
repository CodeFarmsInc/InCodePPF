// NOTE: Sections from the old code which were removed are under
//          #ifdef REMOVED
//       These sections relate to the array of load times which is
//       difficult to port to PPF, and which we decided to remove anyway
//       				Jiri Soukup, July 29, 2012

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <Windows.h>
#include "environ.h"
#include "gen.h"
#include "c:\ppf\factory.h"


//#define USE_UTIL_CLEAR

//Entity classes
class Library
{    
    PersistClass(Library);
public:    
    ZZ_Library ZZds;
};

//--------------------------
class Genre
{
    PersistClass(Genre);
public:
    ZZ_Genre ZZds;
};

//--------------------------
class GenrePtr
{
    PersistClass(GenrePtr);
    PTR(Genre) gp;
public:
    ZZ_GenrePtr ZZds;
    void set(PTR(Genre) g){gp=g;}
    PTR(Genre) get(){return gp;}
};

//--------------------------
class Book
{
    PersistClass(Book);
public:
    ZZ_Book ZZds;
    unsigned order; //
};

//--------------------------
class Chapter
{
    PersistClass(Chapter);
public:
    ZZ_Chapter ZZds;
};

//--------------------------
class GenToBook {
    PersistClass(GenToBook);
public:
    ZZ_GenToBook ZZds;
};

//genres
char * genre_names[] = {
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
int CompListBooksByName(PTR(Book) const b1, PTR(Book) const b2);
int CompListBooksByOrder(PTR(Book) const b1, PTR(Book) const b2);
int CompChaptersByName(PTR(Chapter) const c1, PTR(Chapter) const c2);
#ifdef REMOVED
int CompArrBooksByName(PTR(BookPtr) const bp1, PTR(BookPtr) const bp2);
int CompArrBooksByOrder(PTR(BookPtr) const bp1, PTR(BookPtr) const bp2);
#endif // REMOVED

int main(int argc, char *argv[])
{
    bool create = true;

    if(argc>1 && tolower(argv[1][0])=='c')
    {
        create = true;
        system("del *.ppf");
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

    Library::startPager(128,2,0,0);
    Genre::startPager(16384,20,0,0);
    Book::startPager(16384,1000,0,0);
    Chapter::startPager(16384,1000,0,0);
    GenToBook::startPager(16384,1000,0,0);
    GenrePtr::startPager(16384,20,0,0);
    PersistString::startPager(16384,1000,0,0);
    PersistStart;
    
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
    
    Library::closePager();
    Genre::closePager();
    Book::closePager();
    Chapter::closePager();
    GenToBook::closePager();
    GenrePtr::closePager();
    PersistString::closePager();
    PersistClose;
}

void CreateLibrary()
{
    PTR(GenToBook) gtb; char *tPtr;
    PTR(Chapter) ch; PTR(Genre) ge; GenrePtr gg;
    int i,j,count;
    char bname[128], chname[128]; 
	unsigned start, time1, time2;
    PTR(Library) lib = NULL;    

    srand((unsigned)time(NULL));

    start = time1 = GetTimeMillis();
    
	//-----------------------------------------------------------------------
    Log("**** Creating the library... ****\n");
    lib = new Library();   
    lib.setRoot();
    genres::form(lib, genres_count, -2);
   
    //Genres
    for(i = 0; i < genres_count; i++)
    {        
	ge=new Genre();
	genName::addNew(ge,genre_names[i]);
	gg=genres::get(lib,i);
	gg.set(ge);
	genres::set(lib,i,gg);
    }
    
	int start_idx;
    for(i = 0; i < BOOKS_COUNT; i++)
    {
        bname[0] = '\0'; // empty string
		//generate random book name
        sprintf(bname, "%c%c book %u\0", (rand() % 26) + 65, (rand() % 26) + 65, i);

        PTR(Book) b = new Book;
	bookName::addNew(b,bname);
        b->order = rand();
        
	listBooksByName::addTail(lib, b);
	listBooksByOrder::addTail(lib, b);            
       
		//add book to the random number of genres 
		count = (rand() % genres_count) +1;
        //index of the first genre
        start_idx = rand() % genres_count; 
        for(j = 0; j < count; j++)
        {
            gtb=new GenToBook;         
            int idx = (start_idx + j) % genres_count;
            gg=genres::get(lib,idx);
	    ge=gg.get();
	    genreToBook::add(gtb, ge, b);
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
	    chapName::addNew(ch,chname);
	    chaptersByName::addTail(b,ch);
        }
#ifdef REMOVED
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
#endif // REMOVED
    }
	time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
	AddResult(1, "Library creation", time2-time1);

	//-----------------------------------------------------------------------
	Log("\n\n**** Indexing the library... ****\n");
	
    Log("Sorting books list by name...\n");
    time1 = GetTimeMillis();
    unsigned time3 = time1;
    listBooksByName::sort(CompListBooksByName, lib);
    unsigned time4 = GetTimeMillis();
    Log("Done list by name. Time: %d ms.", time4-time3);

    Log("Sorting books list by order...\n");
    time3 = GetTimeMillis();
    listBooksByOrder::sort(CompListBooksByOrder, lib);
    time4 = GetTimeMillis();
    Log("Done list by order. Time: %d ms.", time4-time3);
        
    time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
	AddResult(2, "Library indexation", time2-time1);

	//-----------------------------------------------------------------------
	Log("\n\n**** Serializing the library... ****\n");
	time1 = GetTimeMillis();
	// perhaps we can move the PersistClose statements to here
	time2 = GetTimeMillis();
    Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
    AddResult(3, "Library saving", time2-time1);
}

void OpenLibrary()
{
    PTR(GenToBook) gtb; char *tPtr;
    PTR(Chapter) ch; PTR(Genre) ge; GenrePtr gg; 
    PTR(Book) b;
    int i,j,count;
	unsigned start, time1, time2;
    PTR(Library) lib;
    genreToBook_Iterator itgb;
    int testIdx = 0;
    char testName[128];

    start = GetTimeMillis();
    time1 = start;

    //-----------------------------------------------------------------------
	Log("\n\n**** Loading the serialized library... ****\n");
	
	// there is no initial read time
	lib.getRoot(); 
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
        gg=genres::get(lib, i);
	ge=gg.get();
        for(gtb=itgb.from1(ge); gtb; gtb=itgb.next1())
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
        listBooksByName_Iterator itbbn;
        chaptersByName_Iterator itch;

        PTR(Book) b;
	b=NULL;
        int cmp;
        int sumb = 0;
        int sumchap;
        count = 0;
        bool begin_found = false;                
	itbbn.start(lib);
        ITERATE(itbbn, b)
        {
            tPtr = bookName::get(b).getPtr();
            cmp = strncmp(find, tPtr, strlen(find));
            if(cmp==0)
            {
                begin_found = true;
                //count the chapters
                sumchap = 0;                
		itch.start(b);
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
        listBooksByOrder_Iterator itbo;
        count = 0;
	itbo.start(lib);
        ITERATE(itbo,b)
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
        listBooksByName_Iterator itb;
        PTR(Chapter) c;
        chaptersByName_Iterator itc;
        bool chapFound;
        char *chName;
	itb.start(lib);
        ITERATE(itb, b)
        {            
            itc.start(b);
            chapCount = 0;
            chapFound = false;
            ITERATE(itc, c)
            {
                if(!chapFound)
                {
                    chName = chapName::get(c).getPtr();
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
#ifdef REMOVED
	Log("\n\n**** The max. borrowing time ****\n");
	time1 = GetTimeMillis();
	{
		listBooksByName_Iterator itbbn;
		int length, size, sum, maxSum=0;		
		PTR(Book) b=NULL;
		int *arr;
        itbbn.start(lib);
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
#endif // REMOVED

    //-----------------------------------------------------------------------
	Log("\n\n**** Remove each second book from the library ****\n");
	time1 = GetTimeMillis();
    {
        int wMark, incr;
        char *name;
        PTR(Book) b;  
        PTR(Chapter) ch;
	PTR(Genre) ge;
        PTR(GenToBook) gtb;
        chaptersByName_Iterator itch;
        listBooksByName_Iterator itbn;
        genreToBook_Iterator itbig;

        i=0;
        j=0;            
            
	itbn.start(lib);
        ITERATE(itbn, b)
        {
            if((i++)%2==1)
            {
                //remove book from genres
                for(j = 0; j < genres_count; j++)
                {
                    gg = genres::get(lib, j);
                    ge = gg.get();
		    for(gtb=itbig.from1(ge); gtb; gtb=itbig.next1())
                    {
			genreToBook::remove(gtb);
                        gtb.delObj(); // equivalent of:  delete gtb;
                    }
                }
             
                //delete chapters
                itch.start(b);
                ITERATE(itch, ch)
                {
                    name = chapName::get(ch).getPtr();
		    chaptersByName::remove(b, ch);
                    ch.delObj();
                }

#ifdef RENMOVED
                //delete borrowing times
                borrowingTimes.free(b);
#endif // REMOVED

                //delete book's name
                bookName::remove(b); // removes also the string

                //remove book from library
		listBooksByName::remove(b);  
		listBooksByOrder::remove(b); 

                //delete the book
                b.delObj();
            }
        }
    }
    time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
    AddResult(11,  "Library reduction", time2-time1);
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

int CompListBooksByName(const PTR(Book) b1, const PTR(Book) b2)
{
    const char *s1 = bookName::get(b1).getPtr();
    const char *s2 = bookName::get(b2).getPtr();
    return strcmp(s1, s2);
}

int CompListBooksByOrder(const PTR(Book) b1, const PTR(Book) b2)
{
    return b1->order - b2->order;
}

#ifdef REMOVED
int CompArrBooksByName(const PTR(BookPtr) bp1, const PTR(BookPtr) bp2)
{
    PTR(Book) b1 = bp1->ptr;
    PTR(Book) b2 = bp2->ptr;
    const char *s1 = bookName::get(b1).getPtr();
    const char *s2 = bookName::get(b2).getPtr();
    return strcmp(s1, s2);
}

int CompArrBooksByOrder(const PTR(Book) b1, const PTR(Book) b2)
{
    PTR(Book) b1 = bp1->ptr;
    PTR(Book) b2 = bp2->ptr;
    return b1->order - b2->order;
}
#endif // REMOVED

int CompChaptersByName(const PTR(Chapter) c1, const PTR(Chapter) c2)
{
    const char *s1 = chapName::get(c1).getPtr();
    const char *s2 = chapName::get(c2).getPtr();
    return strcmp(s1, s2);
}

void Help()
{
    printf("Usage: DOLTest.exe [par]\n");
    printf("par: (required) either 'c' or 'o', where 'c' means 'create library' and 'o' means 'open library'\n");    
}


PersistImplement(Library);
PersistImplement(Genre);
PersistImplement(Book);
PersistImplement(Chapter);
PersistImplement(GenToBook);
PersistImplement(GenrePtr);

#include "gen.cpp"
#include "c:\ppf\pointer.cpp"
