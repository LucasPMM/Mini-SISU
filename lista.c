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

// Function to print one course and their properties
void course_print_course(Course *l){
	printf("Nome do curso: %s\n", l->courseName);
	printf("Número de vagas: %d\n\n", l->positions);
	printf("Lista de alunos:\n");

	lst_imprime(l->listOfStudents);
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

	printf("Digite o nome do aluno: ");
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

	printf("Digite o nome do curso: ");
	fgets(courseName, sizeof(courseName), stdin);

	printf("Digite a quantidade de vagas: ");
	scanf("%d", &positions);
	setbuf(stdin, NULL);
	fflush(stdin);
	return course_insert_course(l, courseName, positions);
}

void course_print_result(Course *l){
	printf("%s %.2f\n", l->courseName, l->passingScore);
	printf("Classificados\n");

	List *p;
	int i = 0;
	for(p=l->listOfStudents; p!=NULL; p=p->prox){
		printf("%s %.2f\n", p->studentName , p->score);
		if (i == l->positions) {
			printf("Lista de espera");
		}
		i++;
	}
}

Course **check_courses_lists(int courseQtd, Course **listOfCourses) {
	int hasChanges = 1, i;
	while (hasChanges) {
		hasChanges = 0;
		for (i = 0; i < courseQtd; i++) {
			char *studentToRemove = findStudentNameToRemove(listOfCourses[i]->listOfStudents, listOfCourses[i]->positions, i);
			if (strcmp(studentToRemove, "NULL") != 0) {
				int index = findStudentSecondOptionIndex(listOfCourses[i]->listOfStudents, studentToRemove);
				if (index != -1) {
					listOfCourses[index]->listOfStudents = remove_second_options(listOfCourses[index]->listOfStudents, studentToRemove);
					hasChanges = 1;
					listOfCourses[i]->listOfStudents = markAsSecondOptionRemoved(listOfCourses[i]->listOfStudents, studentToRemove);
				}
			}
		}
	}
	return listOfCourses;
}

Course *course_passing_score(Course *c, int index) {
	List *l = c->listOfStudents;
	int positions = c->positions, i, isFirstOption = 0;
	char passingScoreStudentName[100];
	
	c->passingScore = 0.0;

	for(i = 0; l != NULL; l = l->prox, i++) {
		if (i == positions) {
			c->passingScore = l->score;
			strcpy(passingScoreStudentName, l->studentName);
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
					// TODO:
					// Switch students positions
					// student1: passingScoreStudentName
					// student2: l->prox->studentName
					isFirstOption = 1;
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
		printf("%s\t%.2f\t%d\t%d\n", p->studentName , p->score, p->firstOp, p->secondOp);
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

	if (p==NULL){ return l; }
	if(ant==NULL) { l=p->prox; }
	else { ant->prox=p->prox; }
	free(p);
	return l;
}

// Function to insert elements on list
List *lst_insere_ordenado(List *l, char studentName[100], float score, int firstOp, int secondOP, int secondOptionWasRemoved){
	List *novo;
	List *ant = NULL;
	List *p = l;
	int i;

	while(p!=NULL && p->score > score){
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
	for(i = 0 ;listOfStudents!=NULL;listOfStudents=listOfStudents->prox){
		if (listOfStudents->firstOp == index && i < positions && !listOfStudents->secondOptionWasRemoved) {
			// remove second option of that student
			return listOfStudents->studentName;
		}		
		i++;
	}
	return "NULL";
}

// List *markAsSecondOptionRemoved(List *listOfStudents, int positions, int index, char studentName[100]) {

// 	// TODO:
// 	// Essa função vai dar problema pois quando dois alunos tiverem a mesma nota, o que conta é a ordem de chegada,
// 	// a qual pode ser prejudicada ao retirar e colocar novamente um aluno da lista
// 	// Recursão!?

// 	int i;
// 	List *aux = listOfStudents;
// 	// copy student informations
// 	// romove from list
// 	// insert again with secondOptionWasRemoved = 1 

// 	for(i = 0; aux!=NULL;aux=aux->prox){
// 		if (strcmp(aux->studentName , studentName) == 0) {
// 			int firstOp = aux->firstOp;
// 			int secondOp = aux->secondOp;
// 			float score = aux->score;
// 			char studentNameCopy[100];
// 			for (i = 0; i < strlen(studentName); i++) {
// 				studentNameCopy[i] = studentName[i];
// 			}
// 			listOfStudents = lst_retira(listOfStudents, studentName);
// 		    listOfStudents = lst_insere_ordenado(listOfStudents, studentNameCopy, score, firstOp, secondOp, 1);
// 		}		
// 		i++;
// 	}
// 	return listOfStudents;
// }

// Function to mark the student that have passed on its first course option
List *markAsSecondOptionRemoved(List *listOfStudents, char studentName[100]) {
	int i;

	if (strcmp(listOfStudents->studentName , studentName) == 0) {
		listOfStudents->secondOptionWasRemoved = 1;
	} else {
		listOfStudents = markAsSecondOptionRemoved(listOfStudents->prox, studentName);
	}
	return listOfStudents;
}

// Function to remove the student's second option
List *remove_second_options(List *listOfStudents, char studentToRemove[100]) {
	listOfStudents = lst_retira(listOfStudents, studentToRemove);
	return listOfStudents;
}

// Function to find the index of list array that contains the student's second course option
int findStudentSecondOptionIndex(List *listOfStudents, char studentName[100]){
	int i;
	for(i = 0 ;listOfStudents!=NULL;listOfStudents=listOfStudents->prox){
		if (strcmp(listOfStudents->studentName, studentName) == 0) {
			// remove second option of that student
			return listOfStudents->secondOp;
		}		
		i++;
	}
	return -1;
}

// ------------------ List Functions End ------------------ //





void initSisu() {
	int i, courseQtd, studentsQtd, k;

	// Read the quantity of courses and students
	scanf("%d%d", &courseQtd, &studentsQtd);
	setbuf(stdin, NULL);
	fflush(stdin);

	// Read all courses and number of positions
	Course **listOfCourses = (Course **) malloc (courseQtd*sizeof(Course *));
	for (i = 0; i < courseQtd; i++) {
		listOfCourses[i] = read_informations(listOfCourses[i]);
	}

	// Inset all students on right course Lista
	for (i = 0; i < studentsQtd; i++) {
		listOfCourses = read_students(listOfCourses);	
	}

	// Print all courses and lists
	for (i = 0; i < courseQtd; i++) {
		course_print_course(listOfCourses[i]);
	}
	
	// Remove second course option from students that have already passed on first option
	listOfCourses = check_courses_lists(courseQtd, listOfCourses);

	// Check draws and define passing score
	for (i = 0; i < courseQtd; i++) {
		// course_passing_score(listOfCourses[i], i);
	}



	// FOR TESTS;
	for (i = 0; i < courseQtd; i++) {
		course_print_course(listOfCourses[i]);
	}
	// FOR TESTS;


	// Print all courses and lists
	// for (i = 0; i < courseQtd; i++) {
	// 	course_print_result(listOfCourses[i]);
	// }

	// Remove list from memory
	course_free_course(listOfCourses, courseQtd);
}