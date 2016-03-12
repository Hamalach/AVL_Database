/*
* @Author: Yitzy Meyer
* @Date:   2016-03-08 17:22:38
* @Last Modified by:   Yitzy Meyer
* @Last Modified time: 2016-03-10 18:53:36
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------  structs  ----------*/


typedef struct Student
{
	int student_ID;
	char student_name[60];
	struct Student *left, *right;
	int height;
} Node;


/*----------  function prototypes  ----------*/

void printMenu(void);
void newStudent(Node **StudentDatabase);
void removeStudent(Node **StudentDatabase);
void printIndividualReport(Node **StudentDatabase);
void printAllReport(Node **StudentDatabase);
void saveFile(Node *StudentDatabase);
void loadFile(Node **StudentDatabase);
int maximum(int a, int b);
int nodeHeight(Node *student);
Node* newBase(int ID, char *name);
Node *rotateRight(Node *a);
Node *rotateLeft(Node *b);
int getBalance(Node *student);
Node* search(Node **student, int ID);
void addStudent(Node **student, int ID, char *name);
void printReport(Node **student, int ID);
void printReportAllStudents();
void save(Node *student, FILE *fp);
void deleteStudent(Node **StudentDatabase, int ID);
int exitDatabase(Node **StudentDatabase);
void deleteDatabase(Node **StudentDatabase);

/*----------  Main  ----------*/


int main() {

	int ext = 1;
	int option;
	Node **StudentDatabase;

	while(ext)
	{
		printMenu();
		scanf("%d", &option);

		switch(option)
		{
			case 1 :
				newStudent(StudentDatabase);
				break;
			case 2 :
				removeStudent(StudentDatabase);
				break;
			case 3 :
				printIndividualReport(StudentDatabase);
				break;
			case 4 :
				printAllReport(StudentDatabase);
				break;
			case 5 :
				saveFile(*StudentDatabase);
				break;
			case 6 :
				loadFile(StudentDatabase);
				break;
			case 7 :
				ext = exitDatabase(StudentDatabase);
			default:
				printf("Please choose a valid option\n");
				break;
		}

		printf("\n");
	}

    return 0;
}

/*----------  Menu Functions  ----------*/

void printMenu(void)
{
	printf("====================\n");
	printf("\tMENU\n");
	printf("====================\n");
	printf("1) Introduce student\n");
	printf("2) Remove student\n");
	printf("3) Print student report\n");
	printf("4) Print report for all students\n");
	printf("5) Save to file\n");
	printf("6) Retrieve data from file\n");
	printf("7) Exit\n");
	printf("--------------------\n>");
}

void newStudent(Node **StudentDatabase)
{
	char name[60];
	int student_number;

	printf("type new students name:\n>");
	fflush(stdin);
	fgets(name, 60, stdin);
	name[strcspn(name, "\n")] = 0;
	printf("type new students number:\n>");
	scanf("%d", &student_number);
	addStudent(StudentDatabase, student_number, name);	
}

void removeStudent(Node **StudentDatabase)
{
	int student_number;
	int deleteChoice;

	printf("Type ID of student to be removed:\n>");
	scanf("%d", &student_number);
	if (search(StudentDatabase, student_number) == NULL)
	{
		printf("Student does not exist\n");
	}
	else
	{
		printf("This is:\n");
		printReport(StudentDatabase, student_number);
		do
		{
			printf("\nAre you sure you want to delete this student?\n0) No\n1) Yes\n>");
			scanf("%d", &deleteChoice);
			switch(deleteChoice)
			{

				case 1 :
					deleteStudent(StudentDatabase, student_number);
					printf("Student Deleted\n");
					break;
				case 0 : 
					printf("Deletion Cancelled\n");
					break;
				default :
					break;
			}
		} while(deleteChoice != 1 && deleteChoice != 0); // There's got to be a better way to do this
	}
}


void printIndividualReport(Node **StudentDatabase)
{
	int student_number;
	printf("Type the student ID of the student you wish to see.\n>");
	scanf("%d", &student_number);
	printReport(StudentDatabase, student_number);
}


void printAllReport(Node **StudentDatabase)
{
	printf("Student ID\tName\n");	
	printReportAllStudents(StudentDatabase);
}


void saveFile(Node *StudentDatabase)
{
	FILE *fp;

	fp = fopen("Database_AVL.csv", "w");

	fprintf(fp, "Student ID, Student Name\n");

	save(StudentDatabase, fp);

	fclose(fp);
}


void loadFile(Node **StudentDatabase)
{
	FILE *fp;
	long ID;
	char *IDchar;
	char str[70];
	char *name;

	fp = fopen("Database_AVL.csv", "r");

	fgets(str, 70, fp);

	while(fgets(str, 70, fp))
	{
		IDchar = strtok(str, ",");
		name = strtok(NULL, ",");
		ID = strtol(IDchar, NULL, 10);
		addStudent(StudentDatabase, ID, name);
	}

}

int exitDatabase(Node **StudentDatabase)
{
	char exitChoice;

	printf("Are you sure you want to exit?\n");
	printf("Any unsaved changes will be deleted\n");
	printf("y/n\n> ");
	scanf("%d", &exitChoice);

	do
	{
		switch(exitChoice)
		{
			case 'y' :
				deleteDatabase(StudentDatabase);
				return 0;
			case 'n' : 
				return 1;
			default:
				printf("Please choose a valid option\n");
				break;
		}
	} while(exitChoice != 1 && exitChoice != 0);
}

/*----------  Functions  ----------*/


/**
 * @brief      Utility: Finds maximum of two integers
 *
 * @param[in]  a     integer 1
 * @param[in]  b     integer 2
 *
 * @return     The maximum integer
 */
int maximum(int a, int b)
{
	int max = a > b ? a : b;
	return max;
}


/**
 * @brief      Utility: finds height of node
 *
 * @param      student  node to find the height of
 *
 * @return     The nodes height
 */
int nodeHeight(Node *student)
{
	if (student == NULL)
	{
		return 0;
	}
	return student->height;
}


/**
 * @brief      Creates a base node with NULL pointers to left and right
 *
 * @param[in]  ID    Student ID of node
 * @param      name  Student Name of node
 *
 * @return     returns address of node
 */
Node* newBase(int ID, char *name)
{
	Node *temp = NULL;
	temp = (Node*)malloc(sizeof(Node));

	temp->student_ID   = ID;
	temp->left         = NULL;
	temp->right        = NULL;
	temp->height       = 1;
	strcpy(temp->student_name, name);

	return (temp);
}


/**
 * @brief      Rotates the tree to the right
 *
 * @param      a     Node around which to rotate
 *
 * @return     The new root node around which there is rotation
 */
Node *rotateRight(Node *a)
{
	Node *b     = a->left;
	Node *value = b->right;

	/* Rotate */
	b->right = a;
	a->left  = value;

	/* Update Height */
	a->height = maximum(nodeHeight(a->left), nodeHeight(a->right)) + 1;
	b->height = maximum(nodeHeight(b->left), nodeHeight(b->right)) + 1;
	
	return b;

}


/**
 * @brief      Rotates the tree to the left
 *
 * @param      b     Node around which to rotate
 *
 * @return     The new root node areound which there is rotation
 */
Node *rotateLeft(Node *b)
{
	Node *a     = b->right;
	Node *value = a->left;

	/* Rotate */
	a->left   = b;
	b->right  = value;

	/* Update Height */
	b->height = maximum(nodeHeight(b->left), nodeHeight(b->right)) + 1;
	a->height = maximum(nodeHeight(a->left), nodeHeight(a->right)) + 1;
	
	return a;

}


/**
 * @brief      Gets balance factor of node
 *
 * @param      student  Node to find balance factor of
 *
 * @return     balance factor
 */
int getBalance(Node *student)
{
	int balance;

	if (student == NULL)
	{
		return 0;
	}

	balance = nodeHeight(student->left) - nodeHeight(student->right);

	return balance;
}


Node* search(Node **student, int ID)
{
	if (!(*student))
	{
		return NULL;
	}
	if (ID == (*student)->student_ID)
	{
		return *student;
	}
	else if (ID > (*student)->student_ID)
	{
		search(&((*student)->right), ID);
	}
	else if (ID < ((*student)->student_ID))
	{
		search(&((*student))->left, ID);
	}
}


void addStudent(Node** student, int ID, char *name)
{
	/*----------  Insert Node  ----------*/


	if ((*student) == NULL)
	{
		printf("EQUAL\n");
		*student = (newBase(ID, name));
	}
	
	else if (ID < (*student)->student_ID)
	{
		printf("LOWER\n");
		addStudent(&((*student)->left), ID, name);
	}
	else if (ID > (*student)->student_ID)
	{
		printf("HIGHER\n");
		addStudent(&((*student)->right), ID, name);
	}


	/*----------  Update Height  ----------*/

	(*student)->height = maximum(nodeHeight((*student)->left), nodeHeight((*student)->right)) + 1;


	/*----------  Get balance factor  ----------*/
		
	int balance = getBalance(*student);

	/*----------  4 possible cases if unbalanced  ----------*/
	

	if (balance > 1 && ID < ((*student)->left)->student_ID)
	{
		*student = rotateRight(*student);
	}

	if (balance < -1 && ID > ((*student)->left)->student_ID)
	{
		*student = rotateLeft(*student);
	}

	if (balance > 1 && ID > ((*student)->left)->student_ID)
	{
		(*student)->left = rotateLeft((*student)->left);
		*student = rotateRight(*student);
	}

	if (balance < -1 && ID < ((*student)->left)->student_ID)
	{
		(*student)->right = rotateRight((*student)->right);
		*student = rotateLeft(*student);
	}
}


void deleteStudent(Node **StudentDatabase, int ID)
{
	/*Foo*/
}


void printReport(Node **student, int ID)
{
	Node *RequestedNode;
	RequestedNode = search(student, ID);
	if (RequestedNode == NULL)
	{
		printf("Student could not be found\n");
	}
	else
	{
		printf("Student ID\tName\n");
		printf("\n%d\t%s\n", RequestedNode->student_ID, RequestedNode->student_name);
	}
}


void printReportAllStudents(Node **student)
{
	if (*student)
	{
		printReportAllStudents(&((*student)->left));
		printf("\n%10d\t%s\n", (*student)->student_ID, (*student)->student_name);
		printReportAllStudents(&((*student)->right));
	}
}


void save(Node *student, FILE *fp)
{
	if (student)
	{
		save(student->left, fp);
		fprintf(fp, "%d,%s,\n", student->student_ID, student->student_name);
		save(student->right, fp);
	}	
}

void deleteDatabase(Node **StudentDatabase)
{
	/*Foo*/
}