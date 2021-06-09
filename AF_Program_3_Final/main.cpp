//main.cpp

#include "table.h"
using namespace std;

/* Andy Fleischer - CS163 - Program #3 - 5/14/2021
 * ----------------------------------------------
 * This sample client code handles a table of
 * terms that map to a description and a set of
 * links. */

void add_term(table* my_table);
void display_term(table* my_table);
void display_all(table* my_table);
void load(table* my_table);
void add_link(table* my_table);
void remove_term(table* my_table);
void retrieve(table* my_table, item& current);
void remove_link(table* my_table);
void add_current(table* my_table, item current);
void help();
void clean_input(char* destination, int size);
void delete_item(item& to_delete);

int main()
{
    table* my_table = new table(7); //main table
    item current; //holds returned item from retrieve
    current.term = NULL;

    cout << "------------------" << endl;
    cout << " Welcome to Table " << endl;
    cout << "------------------" << endl;
    help();

    char input[10] = "";
    while (strcmp(input, "quit") != 0) //main game loop
    {
        cout << ">> ";
        clean_input(input, 10);

        //check command
        if (strcmp(input, "addt") == 0)
            add_term(my_table);
        else if (strcmp(input, "dterm") == 0)
            display_term(my_table);
        else if (strcmp(input, "dall") == 0)
            display_all(my_table);
        else if (strcmp(input, "load") == 0)
            load(my_table);
        else if (strcmp(input, "addl") == 0)
            add_link(my_table);
        else if (strcmp(input, "removet") == 0)
            remove_term(my_table);
        else if (strcmp(input, "retrieve") == 0)
            retrieve(my_table, current);
        else if (strcmp(input, "removel") == 0)
            remove_link(my_table);
        else if (strcmp(input, "addc") == 0)
            add_current(my_table, current);
        else if (strcmp(input, "help") == 0)
            help();
        else if (strcmp(input, "quit") != 0)
            cout << "Invalid command. Type \"help\" for a list of valid commands." << endl;
    }
    
    delete my_table; //delete table at end
    my_table = NULL;

    if (current.term)
        delete_item(current);

    return 0;
}

//add_term - add a term with description and website links
void add_term(table* my_table)
{
    item to_add;

    cout << "Term: "; //get term
    to_add.term = new char[20];
    clean_input(to_add.term, 20);
    
    cout << "Description: "; //get description
    to_add.description = new char[100];
    clean_input(to_add.description, 100);

    cout << "Number of sites: "; //get num_sites
    cin >> to_add.num_sites; 
    cin.clear();
    cin.ignore(1000, '\n');

    to_add.sites = new char*[to_add.num_sites];
    for (int i = 0; i < to_add.num_sites; ++i) //read in all the sites
    {
        cout << "Site " << i+1 << ": ";
        to_add.sites[i] = new char[30];
        clean_input(to_add.sites[i], 30);
    }

    if (my_table->add(to_add)) //catch return values
        cout << "Added." << endl;
    else
        cout << "Could not add." << endl;

    delete_item(to_add);
}

//display_term - display all the information for a particular term
void display_term(table* my_table)
{
    char term[20];
    cout << "Term: ";
    clean_input(term, 20);

    if (!my_table->display_term(term))
        cout << "That term does not exist." << endl;
}

//display_all - display all information for all terms
void display_all(table* my_table)
{
    if (!my_table->display_all())
        cout << "No data!" << endl;
}

//load - load in data from an external file
void load(table* my_table)
{
    char file[40];
    cout << "File name: ";
    clean_input(file, 40);

    if (my_table->load(file))
        cout << "Loaded." << endl;
    else
        cout << "Error when loading data." << endl;
}

//add_link - add a link to an existing term
void add_link(table* my_table)
{
    char link[20];
    cout << "Link: ";
    clean_input(link, 20);

    char term[20];
    cout << "Term: ";
    clean_input(term, 20);

    if (my_table->add_site(link, term))
        cout << "Added." << endl;
    else 
        cout << "That term does not exist." << endl;
}

//remove_term - remove a term from the table
void remove_term(table* my_table)
{
    char term[20];
    cout << "Term: ";
    clean_input(term, 20);

    if (my_table->remove_term(term))
        cout << "Removed." << endl;
    else
        cout << "That term does not exist." << endl;
}

//retrieve - retrieve data from a given term and fill into current
void retrieve(table* my_table, item& current)
{
    char term[20];
    cout << "Term: ";
    clean_input(term, 20);

    if (my_table->retrieve_term(term, current))
        cout << "Retrieved." << endl;
    else
        cout << "That term does not exist." << endl;
}

//remove_link - remove all terms with a given link from the table
void remove_link(table* my_table)
{
    char link[30];
    cout << "Link: ";
    clean_input(link, 50); 

    if (my_table->remove_link(link))
        cout << "Removed." << endl;
    else 
        cout << "Could not find that link." << endl;
}

//add_current - add the item stored in current
void add_current(table* my_table, item current)
{
    if (my_table->add(current)) //catch return values
        cout << "Added." << endl;
    else
        cout << "Could not add." << endl;
}

//help - print out a list of valid commands
void help()
{
    cout << "Valid commands are:" << endl;
    cout << "  addt     - add a new term" << endl;
    cout << "  dterm    - display data from a particular key" << endl;
    cout << "  dall     - display all data" << endl;
    cout << "  load     - load in data from a file" << endl;
    cout << "  addl     - add a link to an existing term" << endl;
    cout << "  removet  - remove a term" << endl;
    cout << "  retrieve - retrieve data from a given term" << endl;
    cout << "  removel  - remove all terms with a given link" << endl;
    cout << "  addc     - add last retrieved item" << endl;
    cout << "  help     - display all valid commands" << endl;
    cout << "  quit     - quit the program" << endl;
}

//clean_input - input into a char* and clear the buffer
void clean_input(char* destination, int size)
{
    cin.get(destination, size);
    cin.clear();
    cin.ignore(size, '\n');
}

//delete_item - deletes all memory of an item struct
void delete_item(item& to_delete)
{
    delete [] to_delete.term; //delete all allocated memory
    delete [] to_delete.description;
    for (int i = 0; i < to_delete.num_sites; ++i)
        delete [] to_delete.sites[i];
    delete [] to_delete.sites;
}
