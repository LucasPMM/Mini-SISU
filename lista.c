#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

Course* lst_create_course(){
	return NULL;
}

Course* lst_insert_course(Course *l, char courseName[100], int positions){
	int i;
	Course *novo = (Course *) malloc (sizeof(Course));
	novo->positions = positions;
	for (i = 0; i < strlen(courseName) - 1; i++) {
		novo->courseName[i] = courseName[i];
	}
	return novo;
}

void lst_print_course(Course *l){
	printf("Nome do curso: %s\n", l->courseName);
	printf("Número de vagas: %d\n\n", l->positions);
	printf("Lista de alunos:\n");

	lst_imprime(l->listOfStudents);
}

List* lst_cria(){
	return NULL;
}

// List* lst_insere(List *l, int i){
// 	List *novo = (List *) malloc (sizeof(List));
// 	novo->prox = l;
// 	return novo;
// }

void lst_imprime(List *l){
	List *p;
	for(p=l;p!=NULL;p=p->prox){
		printf("%s\t%.2f\t%d\t%d\n", p->studentName , p->score, p->firstOp, p->secondOp);
	}
	printf("\n");
}

// int lst_vazia(List *l){
// 	if (l == NULL)
// 		return 1;
// 	else
// 		return 0;
// }

List* lst_busca(List *l, char studentName[100]){
	List *p;
	for(p=l;p!=NULL;p=p->prox){
		if(strcmp(p->studentName, studentName) == 0)
			return p;
	}
	return NULL;
}

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
	if(ant==NULL)
		l=p->prox;
	else
		ant->prox=p->prox;

	free(p);

	return l;
}

// void lst_libera(List *l){
// 	List *p = l;
// 	while(p!=NULL){
// 		List *t = p->prox;
// 		free(p);
// 		p=t;
// 	}
// }

void lst_free_course(Course **c, int courseQtd){
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
		printf("olha o nome que chegou aquii %s\n", studentName);
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

Course *read_informations(Course *l) {
	char courseName[100];
	int positions;

	printf("Digite o nome do curso: ");
	fgets(courseName, sizeof(courseName), stdin);

	printf("Digite a quantidade de vagas: ");
	scanf("%d", &positions);
	setbuf(stdin, NULL);
	fflush(stdin);
	return lst_insert_course(l, courseName, positions);
}

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

List *markAsSecondOptionRemoved(List *listOfStudents, int positions, int index, char studentName[100]) {
	int i;
	List *aux = listOfStudents;
	// copiar as informações dessa pessoa
	// remover da lista
	// inserir novamente com o secondOptionWasRemoved = 1 


	for(i = 0 ;aux!=NULL;aux=aux->prox){
		if (strcmp(aux->studentName , studentName) == 0) {
			int firstOp = aux->firstOp;
			int secondOp = aux->secondOp;
			float score = aux->score;
			char studentNameCopy[100];
			for (i = 0; i < strlen(studentName); i++) {
				studentNameCopy[i] = studentName[i];
			}
			printf("olha o nome que estou mandando para função: %s\n", studentNameCopy);
			listOfStudents = lst_retira(listOfStudents, studentName);
		    listOfStudents = lst_insere_ordenado(listOfStudents, studentNameCopy, score, firstOp, secondOp, 1);
		}		
		i++;
	}
	return listOfStudents;
}

List *remove_second_options(List *listOfStudents, char studentToRemove[100]) {
	printf("\n==== Recebi a listaa =====\n");
	lst_imprime(listOfStudents);
	printf("\n==== Fim da lista recebida =====\n");
	
	listOfStudents = lst_retira(listOfStudents, studentToRemove);

	printf("\n==== Retirei da lista =====\n");
	lst_imprime(listOfStudents);
	printf("\n==== Fim da lista retirada =====\n");

	return listOfStudents;
}

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
		lst_print_course(listOfCourses[i]);
	}
	
	// Remove second course option from students that have already passed on first option
	int hasChanges = 1;
	while (hasChanges) {
		hasChanges = 0;
		for (i = 0; i < courseQtd; i++) {
			char *studentToRemove = findStudentNameToRemove(listOfCourses[i]->listOfStudents, listOfCourses[i]->positions, i);
			if (strcmp(studentToRemove, "NULL") != 0) {
				int index = findStudentSecondOptionIndex(listOfCourses[i]->listOfStudents, studentToRemove);
				if (index != -1) {
					printf("Remover a segunda opção do %s que está no index %d\n", studentToRemove, index);

					listOfCourses[index]->listOfStudents = remove_second_options(listOfCourses[index]->listOfStudents, studentToRemove);
					hasChanges = 1;
					listOfCourses[i]->listOfStudents = markAsSecondOptionRemoved(listOfCourses[i]->listOfStudents, listOfCourses[i]->positions, i, studentToRemove);



	printf("\n\n--------------------------------------------------------------------------------------------------------------\n\n");
	printf("\n\n--------------------------------------------------------------------------------------------------------------\n\n");
	// // Print all courses and lists
	for (k = 0; k < courseQtd; k++) {
		lst_print_course(listOfCourses[k]);
	}
	printf("\n\n--------------------------------------------------------------------------------------------------------------\n\n");
	printf("\n\n--------------------------------------------------------------------------------------------------------------\n\n");







				}
			}
		}
	}

	// // Print all courses and lists
	for (i = 0; i < courseQtd; i++) {
		lst_print_course(listOfCourses[i]);
	}

	// Remove list from memory
	lst_free_course(listOfCourses, courseQtd);
}