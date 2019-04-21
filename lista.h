struct list{
	char studentName[100]; // user ID
	int firstOp; // first option of course
	int secondOp; // second option of course

	float score;

	struct list *prox; // next student
};
typedef struct list List;


struct course{
	char courseName[100];
	int positions;
	List *listOfStudents;
};
typedef struct course Course;


List* lst_cria();

List* lst_insere(List *l, int i);

void lst_imprime(List *l);

int lst_vazia(List *l);

List* lst_busca(List *l, int v);

List *lst_retira(List *l, int v);

void lst_libera(List *l);

// List *lst_insere_ordenado(List *l, char studentName[100], float score, int firstOp, int secondOP);




void initSisu(); // start program