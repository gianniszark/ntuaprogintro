Project 1 : Database of Authors and Books implemented with linked lists
  Takes an input file containing a list of authors, their books and some extra information in the following form :
  Template Input File:
 
  1.Number of Authors
  2.  Author#1_Name Author#1_ID(number)
  3.  Author#1_Number_of_Books
  4.    Author#1_Book#1_Title
  5.    Author#1_Book#1_Sales
  6.    Author#1_Book#2_Title
  7.    Author#1_Book#2_Sales
        .....
        Author#1_Book#m_Title
        Author#1_Book#m_Sales
      .....
      Author#n_Name Author#n_ID(number)
      Author#n_Number_of_Books
          Author#1_Book#1_Title
          Author#1_Book#1_Sales
          .....
           Author#1_Book#k_Title
           Author#1_Book#k_Sales
  
  The program stores the information about the authors and their books. 
  The authors are stored in a sorted linked list according to their ID number. 
  The books are also stored in a linked list. 
  Each node of the authorlist(eg each author entry) has a pointer to the head of their book list. 
  The program finally prints the contents of the database in format (Author,Book,Sales of Book) in the output.txt file. 
  Also it prints a list of popular authors (sales >10.000).
