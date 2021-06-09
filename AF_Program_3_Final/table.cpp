//table.cpp

#include "table.h"

/* Andy Fleischer - CS163 - Program #3 - 5/14/2021
 * ----------------------------------------------
 * This class is a Table ADT used to map given
 * terms to a list of websites. It utilizes a hash
 * function that takes in the term and returns an
 * index in the table to store the related sites
 */

//node destructor - deallocate memory for a single node (setting next to NULL, not continuing to delete)
node::~node()
{
    delete [] data.term; //deallocate the char*s
    data.term = NULL;
    delete [] data.description;
    data.description = NULL;

    for(int j = 0; j < data.num_sites; ++j) //go through and delete all sites
    {
        delete [] data.sites[j];
        data.sites[j] = NULL;
    }
    delete [] data.sites;
    data.sites = NULL;
    data.num_sites = 0;

    next = NULL;
}

//constructor - allocate memory for the table and each node
table::table(int size)
{
    table_size = size;
    hash_table = new node*[table_size]; //allocate array of pointers
    for(int i = 0; i < table_size; ++i) //start each node as NULL
        hash_table[i] = NULL; 
}

//destructor - delete all memory for each node
table::~table()
{
    for(int i = 0; i < table_size; ++i) //go through entire table
    {
        node* current = hash_table[i];
        while (current) //if there is a head node, go through the chain
        {
            node* temp = current->next; //delete current and go to next in chain
            delete current;
            current = temp;
        }
    }
    delete [] hash_table;
    hash_table = NULL;
}

//add - copy the item passed in into the table
int table::add(const item& to_add)
{
    if (!to_add.term || !to_add.description || !to_add.num_sites)
        return 0; //error: invalid input

    int index = hash_function(to_add.term);

    //make a new node, copy in all the data
    node* new_node = new node;
    new_node->data.term = new char[strlen(to_add.term) + 1];
    new_node->data.description = new char[strlen(to_add.description) + 1];
    strcpy(new_node->data.term, to_add.term);
    strcpy(new_node->data.description, to_add.description);
    new_node->data.num_sites = to_add.num_sites;
    new_node->data.sites = new char*[to_add.num_sites];
    for (int i = 0; i < to_add.num_sites; ++i) //copy in each website
    {
        new_node->data.sites[i] = new char[strlen(to_add.sites[i]) + 1];
        strcpy(new_node->data.sites[i], to_add.sites[i]);
    }
    
    //add new_node at head of chain
    node* temp = hash_table[index];
    new_node->next = temp;
    hash_table[index] = new_node;

    return 1; 
}

//display_term - display the data for a given term
int table::display_term(const char* term) const
{
    int index = hash_function(term);
    node* current = hash_table[index]; //hash to get the index

    while (current && (strcmp(current->data.term, term) != 0)) //go through chain until NULL or found the term
        current = current->next;

    if (!current)
        return 0; //error: could not find that term

    //print out all the data
    std::cout << current->data.term << std::endl;
    std::cout << current->data.description << std::endl;
    for (int i = 0; i < current->data.num_sites; ++i)
        std::cout << "  " << i+1 << ". " << current->data.sites[i] << std::endl;

    return 1;
}

//load - load in data from an external data file
int table::load(const char* file_name)
{
    std::ifstream load_file;
    load_file.open(file_name);
    if (!load_file)
        return 0; //error: file does not exist

    char line[100];

    int terms; //get the number of terms
    load_file.getline(line, 100);
    terms = atoi(line);

    for (int i = 0; i < terms; ++i) //for every term...
    {
        item to_add;

        //allocate memory and read in the data
        to_add.term = new char[100];
        load_file.getline(line, 100);
        strcpy(to_add.term, line);
        if (to_add.term[strlen(to_add.term)-1] == '\r') //fix the \r endline character
            to_add.term[strlen(to_add.term)-1] = '\0';

        to_add.description = new char[100];
        load_file.getline(line, 100);
        strcpy(to_add.description, line);
        if (to_add.description[strlen(to_add.description)-1] == '\r')
            to_add.description[strlen(to_add.description)-1] = '\0';

        load_file.getline(line, 100);
        to_add.num_sites = atoi(line);

        to_add.sites = new char*[to_add.num_sites];
        for (int j = 0; j < to_add.num_sites; j++) //go through every site
        {
            to_add.sites[j] = new char[100];
            load_file.getline(line, 100);
            strcpy(to_add.sites[j], line);
            if (to_add.sites[j][strlen(to_add.sites[j])-1] == '\r')
                to_add.sites[j][strlen(to_add.sites[j])-1] = '\0';
        }

        int add_success = add(to_add);

        delete [] to_add.term; //delete all allocated memory
        delete [] to_add.description;
        for (int i = 0; i < to_add.num_sites; ++i)
            delete [] to_add.sites[i];
        delete [] to_add.sites;

        if (!add_success)
            return 0;
    }

    load_file.close();
    return 1;
}

//add_site - add another website link to a given term
int table::add_site(const char* new_site, const char* term)
{
    int index = hash_function(term);
    node* current = hash_table[index]; //hash to get the index

    while (current && (strcmp(current->data.term, term) != 0)) //go through chain until NULL or found the term
        current = current->next;
    
    if (!current)
        return 0; //error: could not find given term

    current->data.num_sites++; //add one to sites and allocate memory
    char** new_sites = new char*[current->data.num_sites];
    for (int i = 0; i < current->data.num_sites - 1; ++i) //loop through and copy and delete all old sites,
    {
        new_sites[i] = new char[strlen(current->data.sites[i]) + 1];
        strcpy(new_sites[i], current->data.sites[i]);
        delete [] current->data.sites[i];
        current->data.sites[i] = NULL;
    }
    new_sites[current->data.num_sites - 1] = new char[strlen(new_site) + 1]; //add the new site
    strcpy(new_sites[current->data.num_sites - 1], new_site);
    delete [] current->data.sites; //delete the old sites, replace with new
    current->data.sites = new_sites;
    
    return 1;
}

//remove_term - remove all data for a given term
int table::remove_term(const char* term)
{
    int index = hash_function(term);
    node* current = hash_table[index]; //hash to get the index

    node* previous = NULL;
    while (current && (strcmp(current->data.term, term) != 0)) //go through chain until NULL or found the term
    {
        previous = current;
        current = current->next;
    }
    
    if (!current)
        return 0; //error: could not find given term

    node* temp = current->next; //store current's next (could be NULL if tail) and delete current
    delete current;

    if (previous) //update either the previous next or the hash_table at the index (if was head)
        previous->next = temp;
    else
        hash_table[index] = temp;
    
    return 1;
}

//retrieve_term - search for given term and fill "found" if found
int table::retrieve_term(const char* term, item& found) const
{
    int index = hash_function(term);
    node* current = hash_table[index]; //hash to get the index

    while (current && (strcmp(current->data.term, term) != 0)) //go through chain until NULL or found the term
        current = current->next;

    if (!current)
        return 0; //error: could not find that term

    //copy in all the data to found
    found.term = new char[strlen(current->data.term) + 1];
    strcpy(found.term, current->data.term);
    found.description = new char[strlen(current->data.description) + 1];
    strcpy(found.description, current->data.description);
    found.num_sites = current->data.num_sites;

    found.sites = new char*[found.num_sites];
    for (int i = 0; i < found.num_sites; ++i) //copy in each site
    {
        found.sites[i] = new char[strlen(current->data.sites[i]) + 1];
        strcpy(found.sites[i], current->data.sites[i]);
    }

    return 1;
}

//remove_link - remove all terms with a given website
int table::remove_link(const char* link)
{
    int to_return = 0;
    for (int i = 0; i < table_size; ++i) //loop through table
    {
        node* previous = NULL;
        node* current = hash_table[i];
        while (current) //loop through LLL
        {
            int flag = 0;
            node* temp = current->next;
            for (int j = 0; j < current->data.num_sites; ++j) //loop through sites
            {
                if (strcmp(current->data.sites[j], link) == 0) //if site match, delete the node and set flag
                {
                    delete current;
                    current = NULL;
                    if (previous) //update the previous pointer
                        previous->next = temp;
                    else
                        hash_table[i] = temp;
                    flag = 1;
                    to_return = 1;
                    break; //break bc node is deleted
                }
            }
            if (!flag)
                previous = current; 
            current = temp;
        }
    }
    return to_return; //returns 0 if no matches were found
}

//display_all - display all chains of data (for debugging purposes)
int table::display_all() const
{
    int success = 0;
    for(int i = 0; i < table_size; ++i) //go through entire table
    {
        node* current = hash_table[i];
        if (current)
            std::cout << "Index " << i << std::endl;
        while (current) //if there is a head node, go through the chain
        {
            success = 1;
            std::cout << "  " << current->data.term << std::endl; //print all the data
            std::cout << "  " << current->data.description << std::endl;
            for (int j = 0; j < current->data.num_sites; ++j)
                std::cout << "    " << j+1 << ". " << current->data.sites[j] << std::endl;
            std::cout << std::endl;

            current = current->next;
       }
    }
    return success; //returns 0 if there was never data to print
} 

//hash_function - private function to hash (two ways, depending on comments) a given key into an index, which it returns
int table::hash_function(const char* key) const
{
    /* hash function 1, just add add the numbers together
    int sum = 0;
    for (int i = 0; key[i] != '\0'; ++i)
        sum += key[i];

    return sum % table_size;
    */

    /* hash function 2, also multiply by 16 to the power of index */
    int result = key[0];
    for (int i = 1; key[i] != '\0'; ++i)
    {
        //conver ascii to letter value (A/a = 1, B/b = 2, etc.)
        int letter;
        if (key[i] > 96 && key[i] < 123)
           letter = key[i] - 96;
        else if (key[i] > 64 && key[i] < 91)
            letter = key[i] - 64;
        else //non-letters just get their ascii values
            letter = key[i];
        result = (result * 16 + letter) % table_size; //Horner's rule
    } 

    return result;
}
