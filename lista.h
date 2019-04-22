struct list{
	char studentName[100]; // user ID
	int firstOp; // first option of course
	int secondOp; // second option of course

	int secondOptionWasRemoved; // 0 => NO and 1 => YES
	float score;

	struct list *prox; // next student
};
typedef struct list List;

struct course{
	char courseName[100];
	int positions;
	float passingScore;
	List *listOfStudents;
};
typedef struct course Course;

// Course Functions:
Course* lst_create_course();
Course* lst_insert_course(Course *l, char courseName[100], int positions);
void lst_print_course(Course *l);
void lst_free_course(Course **c, int courseQtd);
Course **read_students(Course **listOfCourses);
Course *read_informations(Course *l);

// List Functions:
List* lst_cria();
void lst_imprime(List *l);
List *lst_retira(List *l, char studentName[100]);
List *lst_insere_ordenado(List *l, char studentName[100], float score, int firstOp, int secondOP, int secondOptionWasRemoved);
char *findStudentNameToRemove(List *listOfStudents, int positions, int index);
List *markAsSecondOptionRemoved(List *listOfStudents, int positions, int index, char studentName[100]);
List *remove_second_options(List *listOfStudents, char studentToRemove[100]);
int findStudentSecondOptionIndex(List *listOfStudents, char studentName[100]);

void initSisu(); // start program