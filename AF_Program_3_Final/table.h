//table.h

#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>

/* Andy Fleischer - CS163 - Program #3 - 5/14/2021
 * ----------------------------------------------
 * This class is a Table ADT used to map given
 * terms to a list of websites. It utilizes a hash
 * function that takes in the term and returns an
 * index in the table to store the related sites
 */

struct item //struct to hold term, description, and dynamically allocated array of websites
{
    char* term;
    char* description;
    int num_sites;
    char** sites;
};

struct node //LLL node (for chaining the data)
{
    ~node();
    item data;
    node* next;
};

class table
{
    public:
        table(int size=5); //constructor - make table of given size, defaulted at size 5 (Task 1)
        ~table(); //destructor - remove and deallocate all data (Task 2)

        //functions below return 1 if success, 0 if fail
        int add(const item& to_add); //add a term with description and list of websites (Task 4)
        int display_term(const char* term) const; //display all the related data to a given term (Task 5)
        int load(const char* file_name); //load in terms and related data from external file (Task 6)
        int add_site(const char* new_site, const char* term); //add a given website to the list of websites for the given term (Task 7)
        int remove_term(const char* term); //remove all data for a given term (Task 8)
        int retrieve_term(const char* term, item& found) const; //retrieve data for given term and load into empty item found (Task 9)
        int remove_link(const char* link); //remove all nodes that contain the given link (Task 10)
        int display_all() const; //displays all data and chains (for debugging purposes)

    private:
        int hash_function(const char* key) const; //takes in string key and returns index (Task 3)
        node** hash_table;
        int table_size;
};
