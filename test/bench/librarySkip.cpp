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


//Entity classes
class Library
{    
    PersistClass(Library);
public:    
    ZZ_Library ZZds;
};

//--------------------------
class Author
{
    PersistClass(Author);
public:
    ZZ_Author ZZds;
};

//--------------------------
class Book
{
    PersistClass(Book);
public:
    ZZ_Book ZZds;
    unsigned vote; //
};

//--------------------------
class BookToAuthor {
    PersistClass(BookToAuthor);
public:
    ZZ_BookToAuthor ZZds;
};

#define OUT_DIR_FMT "%d_%d"
#define RESULTS_FILE_FMT "results_ppf_%d_%d.txt"
//#define RESULTS_OPEN_FILE_FMT "results_ppf_open_%d_%d.txt"


#define MAX_AUTHORS_PER_BOOK 3
#define BOOKS_TO_AUTHORS_RATIO 5
//some utility functions
unsigned GetTimeMillis();
void Log(const char *msg, ...);
void Help();

class Result
{
public:
    char testName[128];
    int testNo;
    double value;
    Result():testNo(0), value(0.0)
    {
        memset(testName, 0, sizeof(testName));
    }
};
#define RESULTS_COUNT 12
Result g_Results[RESULTS_COUNT];
unsigned g_ResultsCount = 0;


void CreateLibrary(bool genAbstract, unsigned booksCount, bool testing);
void OpenLibrary(bool testing);
void AddResult(int testNo, const char* name, double value);
void PrintResults();
void WriteResultsToFile(unsigned booksCount, const char* res_filename);
unsigned FileSize();
unsigned MyRand(unsigned maxVal=0);

//comparators
int CompBooksByVote(PTR(Book) const b1, PTR(Book) const b2);

unsigned stringSum(char *p){
    unsigned s=0; 
    for(; *p; p++){
	s=s + (unsigned char)(*p);
    }
    return s;
}

void prtCheckSums(PTR(Library) lib,char *fileName){
    unsigned numBooks=0;
    unsigned numAuthors=0;
    unsigned numLinks=0;
    unsigned bookNameLength=0;
    unsigned bookNameSum=0;
    unsigned authorNameLength=0;
    unsigned authorNameSum=0;
    unsigned abstrLength=0;
    unsigned abstrSum=0;

    PTR(Book) bk; PTR(Author) auth; PTR(BookToAuthor) lnk; FILE *fp; char *s;
    PersistString p;
    books_Iterator bit;
    authors_Iterator ait;
    booksToAuthors_Iterator lit;

    printf("\n==== TESTING DATA VALIDITY =====\n");
    printf("==== MESURED TIMES ARE INVALID ====\n\n");

    // traverse all authors
    for(auth=ait.fromHead(lib); auth; auth=ait.next()){
	numAuthors++;
        p=authorName::fwd(auth);
	if(p!=NULL){
            s=p.getPtr();
	    if(s!=NULL){
	        authorNameLength=authorNameLength+strlen(s);
	        authorNameSum=authorNameSum+stringSum(s);
	    }
	}
    }

    // traverse all books
    for(bk=bit.fromHead(lib); bk; bk=bit.next()){
	numBooks++;

        p=bookTitle::fwd(bk);
	if(p!=NULL){
            s=p.getPtr();
	    if(s!=NULL){
	        bookNameLength=bookNameLength+strlen(s);
	        bookNameSum=bookNameSum+stringSum(s);
	    }
	}

        p=bookAbstract::fwd(bk);
	if(p!=NULL){
            s=p.getPtr();
	    if(s!=NULL){
	        abstrLength=abstrLength+strlen(s);
	        abstrSum=abstrSum+stringSum(s);
	    }
        }

	// traverse all author links for this book
        for(lnk=lit.from1(bk); lnk; lnk=lit.next1()){
	    numLinks++;
        }
    }

    fp=fopen(fileName,"w");
    fprintf(fp,"TEST SUMS:\n");
    fprintf(fp,"number of books=%u\n",numBooks);
    fprintf(fp,"number of authors=%u\n",numAuthors);
    fprintf(fp,"number of links between books and authors=%u\n",numLinks);
    fprintf(fp,"total length of book titles=%u\n",bookNameLength);
    fprintf(fp,"check sum for book titles=%u\n",bookNameSum);
    fprintf(fp,"total length of abstracts=%u\n",abstrLength);
    fprintf(fp,"check sum for abstracts=%u\n",abstrSum);
    fprintf(fp,"total length of author names=%u\n",authorNameLength);
    fprintf(fp,"check sum for author names=%u\n",authorNameSum);
    fclose(fp);
}


int main(int argc, char *argv[])
{
    bool testing = false;

    bool create = true;
    bool createOk = false;

    unsigned booksCount = 0;
    bool countOk = false;

    unsigned nthRun = 1;
    bool nthRunOk = false;

    bool genAbstract = false;

    for(int i = 1; i<argc; i++)
    {
        if(strcmp(argv[i], "-c")==0)
        {
            system("del *.ppf");
            create = true;
            createOk = true;
            continue;
        }
        else if(strcmp(argv[i], "-b")==0)
        {
            if(i < (argc-1))
            {
                booksCount = (unsigned)atoi(argv[i+1]);
                countOk = true;
                i++;
                continue;
            }
        }
        else if(strcmp(argv[i], "-a")==0)
        {
            genAbstract = true;
            continue;
        }
        else if(strcmp(argv[i], "-t")==0)
        {
            testing = true;
            continue;
        }
        else if(strcmp(argv[i], "-o")==0)
        {
            create = false;
            createOk = true;
            continue;
        }
        else if(strcmp(argv[i], "-n")==0)
        {
            nthRun = (unsigned)atoi(argv[i+1]);
            nthRunOk = true;
            i++;
            continue;
        }
    }
    if(!createOk || !countOk || !nthRunOk)
    {
        Help();
        return -1;
    }
    
    char dir[64] = {0,};
    sprintf(dir, OUT_DIR_FMT, booksCount, nthRun);
    char cmd[64] = {0,};
    sprintf(cmd, "mkdir %s", dir);
    system(cmd);
    //sprintf(cmd, "cd %s", dir);
    //system(cmd);
    SetCurrentDirectory(dir);

    char res_filename[512] = {0,};

    PersistFactory::useCache=0;
    // choose one of the following lines if preloading/warming up is required
    // PersistFactory::warmDiskCache();
    PersistFactory::preloadPages();

    //unsigned page = 16384;        
    Library::startPager(128,2,0,0);
    /*unsigned pgs = 1;
    unsigned p = 1;
    unsigned page = sizeof(Author)*booksCount/BOOKS_TO_AUTHORS_RATIO * p;
    page = page + 4096 - (page % 4096);    
    Author::startPager(page, pgs, 0, 0);
    page = sizeof(Book)*booksCount * p;
    page = page + 4096 - (page % 4096);    
    Book::startPager(page,pgs,0,0);
    page = sizeof(BookToAuthor)*booksCount * MAX_AUTHORS_PER_BOOK / 2.0 * p;
    page = page + 4096 - (page % 4096);    
    BookToAuthor::startPager(page, pgs, 0, 0);
    pgs = booksCount == 0 ? 1000 : booksCount/5;   
    */
    /*unsigned pgs = booksCount == 0 ? 1000 : booksCount/5;    
    unsigned page = 16384;    
    Author::startPager(page, pgs, 0, 0);    
    Book::startPager(page,pgs,0,0);    
    BookToAuthor::startPager(page, pgs, 0, 0);    
    */

    unsigned objPerPage = booksCount / BOOKS_TO_AUTHORS_RATIO;
    unsigned maxObjects = objPerPage * 2;
    Author::startPagerObj(objPerPage, maxObjects, maxObjects, 2);
    objPerPage = booksCount;
    maxObjects = objPerPage * 2;
    Book::startPagerObj(objPerPage, maxObjects, maxObjects, 2);
    objPerPage = booksCount * MAX_AUTHORS_PER_BOOK / 2.0;
    maxObjects = objPerPage * 2;
    BookToAuthor::startPagerObj(objPerPage, maxObjects, maxObjects, 2);    
    objPerPage = 4096;
    maxObjects = 512*booksCount;
    PersistString::startPagerObj(1024, objPerPage*100, maxObjects, 2);
    
    PersistStart;
    
    sprintf(res_filename, RESULTS_FILE_FMT, booksCount, nthRun);

    if(create)
    {
        CreateLibrary(genAbstract, booksCount, testing);        
    }
    else
    {        
        OpenLibrary(testing);        
        booksCount = 0;
    }
        
    unsigned time1, time2;
    time1 = GetTimeMillis();

    Library::closePager();
    Author::closePager();
    Book::closePager();   
    BookToAuthor::closePager();
    PersistString::closePager();
    PersistClose;
    

    time2 = GetTimeMillis();
    if(create)
    {
        for(int i=0; i<RESULTS_COUNT; i++)
        {
            if(g_Results[i].testNo==3)
            {
                g_Results[i].value += (time2-time1);        
            }
        }
    }
    else
    {
        AddResult(9, "Reduced data saving [ms]", time2-time1);
    }
    

    unsigned testNo = testNo = create ? 4 : 10;
    unsigned size = FileSize();
    AddResult(testNo, "File size [MB]", size / 1000000.);

    //system("cd ..");
    SetCurrentDirectory("..");

    PrintResults();
    WriteResultsToFile(booksCount, res_filename);

    Log("\n\nPPF benchmark done\n");

    return 0;
}

void CreateLibrary(bool genAbstract, unsigned booksCount,bool testing)
{    
    PTR(Library) lib = NULL;
    PTR(Book) book;
    PTR(Author) auth;
    PTR(BookToAuthor) b2a;   
    unsigned start, time1, time2;
    char name[128];
    char abstr[516]; // in case abstract is 512 char plus \0
    
    srand((unsigned)time(NULL));

    start = time1 = GetTimeMillis();
    Log("\n\nCreating the library...\n");    

    lib = new Library(); 
    lib.setRoot();
    //first create authors
    Log("Generating authors...\n");
    unsigned authorsCount = booksCount>=BOOKS_TO_AUTHORS_RATIO ? booksCount / BOOKS_TO_AUTHORS_RATIO : booksCount;
    PTR(Author) *tmpAuth = new PTR(Author)[authorsCount];
    for(unsigned i=0; i<authorsCount; i++)
    {
        auth = new Author();
        sprintf(name, "%c%c author %d", MyRand(26) + 65, MyRand(26) + 65, i+1);
        authorName::addNew(auth, name);        
        tmpAuth[i] = auth;
        authors::addHead(lib, auth);
    }

    Log("Generating books...\n");
    int absSize;
    unsigned count = 0;        
    bool ok;
    booksToAuthors_2XtoXIterator it;
    for(unsigned i=0; i<booksCount; i++)
    {
        book = new Book();
        name[0] = '\0'; // empty string
		//generate random book name
        sprintf(name, "%c%c book %u\0", MyRand(26) + 65, MyRand(26) + 65, i+1);
        bookTitle::addNew(book, name);
        book->vote = MyRand(booksCount);
        books::addHead(lib, book);
        
        if(genAbstract)
        {
            absSize = MyRand(512)+1;
            absSize = absSize < 16 ? 16 : absSize;
            memset(abstr, 'a', absSize);
            abstr[absSize-1] = '\0';
            bookAbstract::addNew(book, abstr);
        }

        //random authors
        unsigned bookAuthCount = authorsCount>=MAX_AUTHORS_PER_BOOK ? MyRand(MAX_AUTHORS_PER_BOOK) : authorsCount;
        bookAuthCount = bookAuthCount==0 ? 1 : bookAuthCount;
        count = 0;
        //avoid assign one author twice to the same book
        while(true)
        {
            int idx = MyRand(authorsCount);
            auth = tmpAuth[idx];           
            ok = true;            
            
            PTR(Author) a;
            for(b2a=it.from1(book); b2a; b2a=it.next1()) 
            {
                a = booksToAuthors::entity2(b2a);
                if(a==auth)
                {
                    ok = false;
                    break;
                }

            }            
            if(ok)
            {   
                b2a = new BookToAuthor();
                booksToAuthors::add(b2a, book, auth);                
                count++;
            }
            else
            {
                continue;
            }
            if(count==bookAuthCount)
            {
                break;
            }
        }
    }

    //remove authors with no book   
    bool hasBook;
    for(unsigned i=0; i<authorsCount; i++)
    {
        b2a = NULL;
        hasBook = false;
        for(b2a=it.from2(tmpAuth[i]); b2a; b2a=it.next2())
        {
            hasBook = true;
            break;
        }        
        if(!hasBook)
        {
            authors::del(lib, tmpAuth[i]);
            tmpAuth[i].delObj();
            tmpAuth[i]=NULL;
        }
    }
    delete [] tmpAuth;

    time2 = GetTimeMillis();
    Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
    AddResult(1, "Library creation [ms]", time2-time1);

    if(testing)prtCheckSums(lib,"sums1.txt");


    Log("\n\nSorting books by vote...\n");
    time1 = GetTimeMillis();        
    books::sort(CompBooksByVote, lib);
    time2 = GetTimeMillis();
    Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
    AddResult(2, "Sort by vote [ms]", time2-time1);

    AddResult(3, "Library saving [ms]", 0);  
}

void OpenLibrary(bool testing)
{
    unsigned time1, time2, start;
    PTR(Library) lib;
    PTR(Author) auth;
    PTR(Book) book;
    PTR(BookToAuthor) b2a;

    start = time1 = GetTimeMillis();
    
    Log("\n\nLoading data...\n");
    lib.getRoot();
    time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
	AddResult(5, "Library reopen", time2-time1);

    if(testing)prtCheckSums(lib,"sums2.txt");


    time1 = GetTimeMillis();
    const int topVoted = 5;
    Log("Print top %d voted books...\n", topVoted);
    unsigned cnt = 0;
    books_Iterator it;
    for(book=it.fromHead(lib); book; book=it.next())   
    {
        if(cnt>=topVoted)
        {
            break;
        }
        Log("%d. book: %s, vote: %d\n", cnt+1, bookTitle::fwd(book).getPtr(), book->vote);
        cnt++;
    }
    time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
	AddResult(6, "Top voted books [ms]", time2-time1);  


    time1 = GetTimeMillis();
    const char* find = "Z book";
    Log("Number of books containing '%s' in their title...\n", find);
    it.start(lib);
    cnt = 0;
    for(book=it.fromHead(lib); book; book=it.next())
    {
        if(strstr(bookTitle::fwd(book).getPtr(), find)!=NULL)
        {
            cnt++;
        }
    }
    Log("Number of books: %d\n", cnt);
    time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
	AddResult(7, "Search book by title [ms]", time2-time1);


    time1 = GetTimeMillis();
    Log("\n\nRemove one quarter of the books\n");
    booksToAuthors_Iterator itBas;
    booksToAuthors_Iterator itBat;
    PTR(BookToAuthor) b2as, b2at;
    
    cnt = 1;
    bool hasBook;
    for(book=it.fromHead(lib); book; book=it.next())
    {
        if((cnt%4)==0)
        {
            //PM: CRASH
            //remove books name            
            bookTitle::del(book);
            //remove abstract if any
            bookAbstract::del(book); // XXX problem?
            //remove book
            books::del(lib, book);            
            
            //remove the relation Book-Author
            for(b2as=itBas.from1(book); b2as; b2as=itBas.next1())
            {
                auth = booksToAuthors::entity2(b2as);
                //remove th relation
                booksToAuthors::remove(b2as);
                //if the author does not have any book, remove the author
                hasBook = false;
                for(b2at=itBat.from2(auth); b2at; b2at=itBat.next2())
                {
                    hasBook = true;
                    break;
                }
                if(!hasBook)
                {
                    //PM: CRASH
                    //remove author's name
                    authorName::del(auth);
                    //remove author                    
                    authors::del(lib, auth);
                    auth.delObj();
                }
                b2as.delObj();
            }
            book.delObj();
        }
        cnt++;
    }

    time2 = GetTimeMillis();
	Log("Done. Time: %d ms, Total: %d ms\n", time2-time1, time2-start);
	AddResult(8, "Data reduction [ms]", time2-time1);

    if(testing)prtCheckSums(lib,"sums3.txt");
}

void AddResult(int testNo, const char* name, double value)
{
    g_Results[g_ResultsCount].testNo = testNo;
    strcpy(g_Results[g_ResultsCount].testName, name);
    g_Results[g_ResultsCount].value = value;
    g_ResultsCount++;
}

void PrintResults()
{
    printf("\nBENCHMARK RESULTS:\n\n");

    printf("----------------------------------------------------------------------------\n");
    printf("TestNo. | Test Name                                             | Value |\n");
    printf("============================================================================\n");
    for(unsigned i = 0; i < g_ResultsCount; i++)
    {
        printf("   %02d   | %-53s | %-8.2lf |\n", g_Results[i].testNo, g_Results[i].testName, g_Results[i].value);
    }
    printf("----------------------------------------------------------------------------\n");
}

void WriteResultsToFile(unsigned booksCount, const char *res_filename)
{
    setlocale(LC_ALL, "");
    FILE *f = fopen(res_filename, "a");
    //books_count
    // testno; test_name; value
    if(booksCount != 0)
    {
        fprintf(f, "\n%d;;;\n", booksCount);
    }
    for(unsigned i = 0; i < g_ResultsCount; i++)
    {
        char * end = ((i == (g_ResultsCount-1)) && booksCount!=0) ? "" : "\n";
        fprintf(f, ";%d;%s;%.2lf%s", g_Results[i].testNo, g_Results[i].testName, g_Results[i].value, end);
    }
    if(booksCount!=0)
    {
        fprintf(f, "\n");
    }
    else
    {
        fprintf(f, "\n");
        fprintf(f, "\n");
        fprintf(f, "\n");
        fprintf(f, "\n");
    }
    fclose(f);
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

int CompBooksByVote(const PTR(Book) b1, const PTR(Book) b2)
{
    return b2->vote - b1->vote;
}


void Help()
{
    printf("Usage: ppfTest.exe [params]\n");
    printf("param: (required) either '-c' or '-o', where 'c' means 'create library' and 'o' means 'open library'\n"); 
    printf("param: (required) '-b <books_count>': number of books to generate\n");
    printf("param: (required) '-n <run_order>': order of the run of this program\n");
    printf("param: (optional with -c) '-a': generate abstract for each book (random size, max. 512 bytes)");
}

unsigned MyRand(unsigned maxVal)
{
    unsigned rnd;
    rnd = rand()*rand();
    if(maxVal>0)
    {
        rnd %= (maxVal);
    }
    return rnd;
}

unsigned FileSize(const char* filename)
{
    DWORD size = 0;
    DWORD high;
    HANDLE f = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(f != INVALID_HANDLE_VALUE)
    {
        size = GetFileSize(f, &high);
        DWORD err = GetLastError();
        CloseHandle(f);
    }

    return size;
}

unsigned FileSize()
{
    WIN32_FIND_DATA myfile;
    unsigned size = 0;    
    HANDLE hFile = FindFirstFile("*.ppf", &myfile);
    if(hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            size+= myfile.nFileSizeLow;
        }while(FindNextFile(hFile, &myfile)!=0);
    }

    return size;
}

PersistImplement(Library);
PersistImplement(Author);
PersistImplement(Book);
PersistImplement(BookToAuthor);

#include "gen.cpp"
#include "c:\ppf\pointer.cpp"
