//Hasan Kuspinar
#include <iostream>
#include <string>
#include "feedback.h"
using namespace std;

struct node
{
    int digit;
    node * next;
    //default constructor
    node ()
    {}

    //constructor
    node (int i,node *n)
            :digit(i), next(n)
    {}
};

bool search_list(node * head, int dig) // for checking whether dig is in the list or not
{
    while(head!=NULL)
    {
        if(head->digit==dig)
        {
            return true;
        }
        head = head->next;
    }
    return false;
}

bool input_check(string str) // for checking the validity of the input
{
    if(str.length() != 3)
    {
        return false;
    }
    else
    {
        char first = str[0], second = str[1], third = str[2];
        for(int i = 0; i < str.length(); i++)
        {
            if(!(str[i]-'0' <= 9 && str[i] >= 0))
            {
                return false;
            }
        }
        if(first == second || first == third || second == third)
        {
            return false;
        }
    }
    return true;
}

node * AddInOrder(node * head, int newkey) //taken from lecture slides 2.2
// pre: list is sorted
// post: add newkey to list, keep list sorted, return head of new list
{
    node *  ptr = head;   // loop variable

    // if new node should be first, handle this case and return
    // in this case, we return address of new node since it is new head
    if (head == NULL || newkey < head->digit)
    {
        return new node(newkey, head); //new head
    }

    // check node one ahead so we don't pass!
    while (ptr->next != NULL && ptr->next->digit < newkey)
    {
        ptr = ptr->next;
    } // postcondition: new node to be inserted just after the node ptr points

    //now insert new node with newkey after where ptr points to

    ptr->next = new node(newkey, ptr->next);

    return head;
}

void DeleteOneNode (int digit, node * & head) //deleting the node including dig, taken from lecture slides 2.2 and updated
{
    node * ptr;
    node * deleted;
    if(head->digit == digit)
    {
        deleted = head;
        head = head->next;
        delete deleted;
    }
    else
    {
        ptr = head;
        while(ptr->next->digit != digit)
        {
            ptr = ptr->next;
        }
        deleted = ptr->next;
        ptr->next = deleted->next;
        delete deleted;
    }
}

void print_list(node * head) //prints the linked lists
{
    node * ptr = head;
    while(ptr != NULL)
    {
        cout << ptr->digit << " ";
        ptr = ptr->next;
    }
}

bool is_Full(node * head) //checks if the impossibles has 9 elements or not
{
    int count = 0;
    node * ptr = head;
    while(ptr != NULL)
    {
        count++;
        ptr = ptr->next;
    }
    if(count != 9)
    {
        return false;
    }
    return true;
}

void delete_list(node * head) //for deleting the linked lists when the game is over (taken from week3 lab codes)
{
    node * ptr;
    while(head!=NULL)
    {
        ptr=head;
        head=head->next;
        delete ptr;
    }
}

int main()
{
    int gameID;
    bool endgame = false;
    node * imp_ptr[3]; //creating array of node pointers and initializing the pointers to NULL
    for(int i = 0; i < 3; i++)
    {
        imp_ptr[i] = NULL;
    }

    node * pos_ptr[3]; //creating array of node pointers and initializing the pointers to NULL
    for(int i = 0; i < 3; i++)
    {
        pos_ptr[i] = NULL;
    }

    cout << "Please enter a game ID." << endl;
    cin >> gameID;

    string guess;
    while(endgame==false)
    {
        cout << "Enter your guess." << endl;
        cin >> guess;
        while (input_check(guess) == false)
        {
            cout << "Invalid guess. Enter your guess." << endl;
            cin >> guess;
        }
        string feed_back = get_the_feedback(guess, gameID);
        for(int i = 0; i < 3; i++)
        {
            if(feed_back[i] == 'R') //the condition of the feedback being 'R'
            {
                int dig1 = guess[i]-'0';


                for (int a = 0; a < 3; a++)
                {
                    if(search_list(imp_ptr[a], dig1) == false)
                    {
                        imp_ptr[a] = AddInOrder(imp_ptr[a], dig1);
                    }
                }

            }
            else if(feed_back[i] == 'Y') //the condition of the feedback being 'Y'
            {
                int dig1 = guess[i]-'0';
                if(search_list(pos_ptr[i], dig1))
                {
                    DeleteOneNode(dig1, pos_ptr[i]);
                }
                if(search_list(imp_ptr[i], dig1)==false)
                {
                    imp_ptr[i] = AddInOrder(imp_ptr[i], dig1);
                }
                for(int b = 0; b < 3; b++) //adding the digit to the other slots of possible
                {
                    if(b!=i)
                    {
                        if(search_list(pos_ptr[b], dig1)==false && search_list(imp_ptr[b], dig1)==false)
                        {
                            pos_ptr[b] = AddInOrder(pos_ptr[b], dig1);
                        }
                    }
                }
            }
            else //the condition of the feedback being 'G'
            {
                int dig1 = guess[i]-'0';
                if(search_list(pos_ptr[i], dig1)==false)
                {
                    pos_ptr[i] = AddInOrder(pos_ptr[i], dig1);
                }
                for(int b = 0; b < 3; b++) //adding the digit to the other slots of impossible
                {
                    if(b!=i)
                    {
                        if(search_list(imp_ptr[b], dig1)==false)
                        {
                            imp_ptr[b] = AddInOrder(imp_ptr[b], dig1);
                        }
                    }
                }
                for(int b = 0; b < 3; b++) //removing the digit from the other slots of possible
                {
                    if(b!=i)
                    {
                        if(search_list(pos_ptr[b], dig1))
                        {
                            DeleteOneNode(dig1, pos_ptr[b]);
                        }
                    }
                }
                for(int b = 0; b < 10; b++) //adding all the other numbers except digit to the impossible
                {
                    if(b!=dig1)
                    {
                        if(search_list(imp_ptr[i], b)==false)
                        {
                            imp_ptr[i] = AddInOrder(imp_ptr[i], b);
                        }
                    }
                }
                for(int b = 0; b < 10; b++) //removing all the other numbers except digit from the possible
                {
                    if(b!=dig1)
                    {
                        if(search_list(pos_ptr[i], b))
                        {
                            pos_ptr[i] = AddInOrder(pos_ptr[i], b);
                        }
                    }
                }
            }
        }
        cout << "Linked lists: " << endl;
        for(int i = 0; i < 3; i++)
        {
            cout << "Slot: " << i+1 << endl;
            cout << "Impossibles: ";
            print_list(imp_ptr[i]);
            cout << endl;
            cout << "Possibles: ";
            print_list(pos_ptr[i]);
            cout << endl << endl;
        }
        bool bool1 = false;
        for(int i = 0; i < 3; i++)
        {
            if(is_Full(imp_ptr[i])==false) //checking if any element of the imp array has 9 elements or not
            {
                bool1 = true;
            }
        }
        if(bool1 == false) //ending the game depending on the above condition
        {
            endgame = true;
        }

    }
    cout << "The secret number is: ";
    for(int i = 0; i < 3; i++)
    {
        for(int a = 0; a < 10; a++)
        {
            if(search_list(imp_ptr[i], a) == false)
            {
                cout << a;
            }
        }
    }
    cout << endl;

    cout << "Congrats! Now, deleting the lists..." << endl;
    for(int i = 0; i < 3; i++)
    {
        delete_list(imp_ptr[i]);
        delete_list(pos_ptr[i]);
    }

    return 0;
}
