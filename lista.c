#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

// ------------------ Course Functions Start ------------------ //

// Function to initialize course structure
Course* course_create_course(){
	return NULL;
}

// Function to create an structure of course 
Course* course_insert_course(Course *l, char courseName[100], int positions){
	int i;
	Course *novo = (Course *) malloc (sizeof(Course));
	novo->positions = positions;
	for (i = 0; i < strlen(courseName) - 1; i++) {
		novo->courseName[i] = courseName[i];
	}
	return novo;
}

// Function to free the occupied memory with the data of the structure course
void course_free_course(Course **c, int courseQtd){
	int i;
	for (i = 0; i < courseQtd; i++) {
		if (c[i]->listOfStudents) {
			List *p = c[i]->listOfStudents;
			List *t = p->prox;
			free(p);
			p=t;
		}
	}
	free(c);
}

// Function to read student informations
Course **read_students(Course **listOfCourses) {
	char studentName[100];
	int firstOp, secondOP;
	float score;

	fgets(studentName, sizeof(studentName), stdin);

	scanf("%f%d%d", &score, &firstOp, &secondOP);
	setbuf(stdin, NULL);
	fflush(stdin);
	
	listOfCourses[firstOp]->listOfStudents = lst_insere_ordenado(listOfCourses[firstOp]->listOfStudents, studentName, score, firstOp, secondOP, 0);
	listOfCourses[secondOP]->listOfStudents = lst_insere_ordenado(listOfCourses[secondOP]->listOfStudents, studentName, score, firstOp, secondOP, 0);

	return listOfCourses;
}

// Function to read course informations
Course *read_informations(Course *l) {
	char courseName[100];
	int positions;

	fgets(courseName, sizeof(courseName), stdin);

	scanf("%d", &positions);
	setbuf(stdin, NULL);
	fflush(stdin);
	return course_insert_course(l, courseName, positions);
}

void course_print_result(Course *l){
	int waitingListPrinted = 0;

	printf("%s %.2f\n", l->courseName, l->passingScore);
	printf("Classificados\n");
	if (l && l->listOfStudents && l->positions && (l->listOfStudents->firstOp || l->listOfStudents->secondOp)) {

		List *p;
		int i = 0;
		for(p=l->listOfStudents; p!=NULL; p=p->prox){
			printf("%s %.2f\n", p->studentName , p->score);
			if (i + 1 == l->positions || (p->prox == NULL && !waitingListPrinted)) {
				waitingListPrinted = 1;
				printf("Lista de Espera\n");
			}
			i++;
		}
		printf("\n");
	} else {
		printf("Lista de Espera\n");
	}
}

Course **check_courses_lists(int courseQtd, Course **listOfCourses) {
	int hasChanges = 1, i, j;
	while (hasChanges) {
		hasChanges = 0;
		for (i = 0; i < courseQtd; i++) {
			char *studentToRemove = findStudentNameToRemove(listOfCourses[i]->listOfStudents, listOfCourses[i]->positions, i);
			if (strcmp(studentToRemove, "NULL") != 0) {
				int index = find_student_second_option_index(listOfCourses[i]->listOfStudents, studentToRemove);
				if (index != -1) {
					listOfCourses[index]->listOfStudents = lst_retira(listOfCourses[index]->listOfStudents, studentToRemove);
					hasChanges = 1;
					listOfCourses[i]->listOfStudents = mark_as_second_option_removed(listOfCourses[i]->listOfStudents, studentToRemove);
				}
			}
		}
	}
	return listOfCourses;
}

Course *course_passing_score(Course *c, int index, int *ListChangedWatcher) {
	List *l = c->listOfStudents;
	int positions = c->positions, i, isFirstOption = 0;
	
	// Student at last position on regular list informations
	char passingScoreStudentName[100];
	int approvedFirstOp;
	int approvedSecondOp;
	int approvedSecondOptionWasRemoved;
	float approvedScore;

	// Student on waiting list informations
	char waitingStudentName[100];
	int waitingFirstOp;
	int waitingSecondOp;
	int waitingSecondOptionWasRemoved;
	float waitingScore;

	c->passingScore = 0.0;

	for(i = 0; l != NULL; l = l->prox, i++) {
		if (i + 1 == positions) {
			c->passingScore = l->score;
			strcpy(passingScoreStudentName, l->studentName);
			approvedFirstOp = l->firstOp;
			approvedSecondOp = l->secondOp;
			approvedSecondOptionWasRemoved = l->secondOptionWasRemoved;
			approvedScore = l->score;
			if (index == l->firstOp) {
				isFirstOption = 1;
			}
		}

		// If the course in question is the second option of the last placed of the regular list 
		if (!isFirstOption && l->prox) {
			// If the last list score of the first list is equal to the first list score of the waiting list
			if (c->passingScore == l->prox->score) {
				// If this course is the first option of the first placed of the waiting list
				if (l->prox->firstOp == index) {

					strcpy(waitingStudentName, l->prox->studentName);
					waitingFirstOp = l->prox->firstOp;
					waitingSecondOp = l->prox->secondOp;
					waitingSecondOptionWasRemoved = l->prox->secondOptionWasRemoved;
					waitingScore = l->prox->score;

					// Switch users
					update_user_informations(c->listOfStudents, waitingStudentName, passingScoreStudentName, approvedFirstOp, approvedSecondOp, approvedSecondOptionWasRemoved, approvedScore);
					update_user_informations(c->listOfStudents, passingScoreStudentName, waitingStudentName, waitingFirstOp, waitingSecondOp, waitingSecondOptionWasRemoved, waitingScore);

					isFirstOption = 1;
					*ListChangedWatcher = 1;
					break;
				}
			}
		}

	}
	return c;
}

// ------------------ Course Functions End ------------------ //


// ------------------ List Functions Start ------------------ //

// Function to initialize list structure
List* lst_cria(){
	return NULL;
}

// Function to print list informations
void lst_imprime(List *l){
	List *p;
	for(p=l;p!=NULL;p=p->prox){
		printf("%s\t%.2f\t%d\t%d\t wasRemoved: %d\n", p->studentName , p->score, p->firstOp, p->secondOp, p->secondOptionWasRemoved);
	}
	printf("\n");
}

// Function to remove an element from list
List *lst_retira(List *l, char studentName[100]){
	List *ant = NULL;
	List *p=l;
	while(p!=NULL && strcmp(p->studentName, studentName) != 0) {
		ant=p;
		p=p->prox;
	}

	if (p==NULL){
		return l;
	}
	if(ant==NULL) {
		l=p->prox;
	}
	else {
		ant->prox=p->prox;
	}
	free(p);
	return l;
}

// Function to insert elements on list
List *lst_insere_ordenado(List *l, char studentName[100], float score, int firstOp, int secondOP, int secondOptionWasRemoved){
	List *novo;
	List *ant = NULL;
	List *p = l;
	int i;

	while(p!=NULL && p->score >= score){
		ant = p;
		p = p->prox;
	}

	novo = (List *) malloc (sizeof(List));
	novo->score = score;
	novo->firstOp = firstOp;
	novo->secondOp = secondOP;
	novo->secondOptionWasRemoved = secondOptionWasRemoved;

	if (secondOptionWasRemoved) {
		for (i = 0; i < strlen(studentName); i++) {
			novo->studentName[i] = studentName[i];
		}
	} else {
		for (i = 0; i < strlen(studentName) - 1; i++) {
			novo->studentName[i] = studentName[i];
		}
	}

	if (ant == NULL){
		novo->prox = l;
		l = novo;
	}
	else{
		novo->prox = ant->prox;
		ant->prox = novo;
	}
	return l;
}

// Function to find the name of an student that have passed on its first course option
char *findStudentNameToRemove(List *listOfStudents, int positions, int index) {
	int i;
	List *s = listOfStudents;
	for(i = 0; s!=NULL; s=s->prox){
		if (s->firstOp == index && i + 1 <= positions && !s->secondOptionWasRemoved) {
			// remove second option of that student
			return s->studentName;
		}		
		i++;
	}
	return "NULL";
}

// Function to mark the student that have passed on its first course option
List *mark_as_second_option_removed(List *listOfStudents, char studentName[100]) {
	int i;
	List *s = listOfStudents;

	if (strcmp(s->studentName , studentName) == 0) {
		s->secondOptionWasRemoved = 1;
	} else {
		s->prox = mark_as_second_option_removed(s->prox, studentName);
	}
	return s;
}

// Function to update user informations
List *update_user_informations(List *listOfStudents, char oldStudentName[100], char newStudentName[100], int firstOp, int secondOp, int secondOptionWasRemoved, float score) {
	int i;

	if (strcmp(listOfStudents->studentName , oldStudentName) == 0) {
		strcpy(listOfStudents->studentName, newStudentName);
		listOfStudents->firstOp = firstOp;
		listOfStudents->secondOp = secondOp;
		listOfStudents->secondOptionWasRemoved = secondOptionWasRemoved;
		listOfStudents->score = score;		
	} else {
		listOfStudents = update_user_informations(listOfStudents->prox, oldStudentName, newStudentName, firstOp, secondOp, secondOptionWasRemoved, score);
	}
	return listOfStudents;
}

// Function to find the index of list array that contains the student's second course option
int find_student_second_option_index(List *listOfStudents, char studentName[100]){
	int i;
	List *s = listOfStudents;
	for(i = 0; s!=NULL; s = s->prox){
		if (strcmp(s->studentName, studentName) == 0) {
			// remove second option of that student
			return s->secondOp;
		}		
		i++;
	}
	return -1;
}

// ------------------ List Functions End ------------------ //


void init_sisu() {
	int i, courseQtd, studentsQtd, k, ListChangedWatcher = 1;

	// Read the quantity of courses and students
	scanf("%d%d", &courseQtd, &studentsQtd);
	setbuf(stdin, NULL);
	fflush(stdin);

	// Read all courses and number of positions
	Course **listOfCourses = (Course **) malloc (courseQtd*sizeof(Course *));
	for (i = 0; i < courseQtd; i++) {
		listOfCourses[i] = read_informations(listOfCourses[i]);
	}

	// Inset all students on right course list
	for (i = 0; i < studentsQtd; i++) {
		listOfCourses = read_students(listOfCourses);	
	}

	while (ListChangedWatcher) {
		ListChangedWatcher = 0;
		// Remove second course option from students that have already passed on first option
		listOfCourses = check_courses_lists(courseQtd, listOfCourses);
		
		// Check draws and define passing score
		for (i = 0; i < courseQtd; i++) {
			listOfCourses[i] = course_passing_score(listOfCourses[i], i, &ListChangedWatcher);
		}
	}

	// Print all courses and lists
	// printf("\n");
	for (i = 0; i < courseQtd; i++) {
		course_print_result(listOfCourses[i]);
	}

	// Remove list from memory
	course_free_course(listOfCourses, courseQtd);
}