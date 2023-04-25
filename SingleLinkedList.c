/*
 * 단일 연결 리스트
 */

#include<stdio.h>
#include<stdlib.h>

/*노드 하나를 표현한 구조체*/	
//typedef를 작성하면 추후에 struct Node 자료형을 생략하고 별칭만 사용해 선언할 수 있다.
//typedef struct 구조체변수명 {} 별칭; 추후에 별칭만 사용해 구조체를 선언할 수 있다.
typedef struct Node {
	int key;
	struct Node* link;
} listNode;

/*헤드를 표현한 구조체*/
//노드의 주소만 가르키면 되므로 노드의 주소를 담을 수 있도록 노드형 포인터로 선언함.
typedef struct Head {
	struct Node* first;
} headNode;


headNode* initialize(headNode* h);      //연결리스트 초기화 함수
int freeList(headNode* h);              //동적할당 해제 함수

int insertFirst(headNode* h, int key);  //처음에 노드 삽입
int insertNode(headNode* h, int key);   //연결리스트의 적절한 위치(오름차순)에 노드 삽입
int insertLast(headNode* h, int key);   //끝에 노드 삽입

int deleteFirst(headNode* h);           //첫 노드 삭제
int deleteNode(headNode* h, int key);   //사이 노드 삭제
int deleteLast(headNode* h);            //끝 노드 삭제

int invertList(headNode* h);            //리스트 전치

void printList(headNode* h);            //연결리스트 출력

int main()
{

    printf("[----- [Park Mira] [2020069006] -----]\n\n");


	char command;  //옵션 선택 문자
	int key; //원하는 노드의 key값
	headNode* headnode=NULL; //headNode 구조체 포인터형으로 headnode 선언 후 NULL값으로 초기회 

	do{
		printf("----------------------------------------------------------------\n");
		printf("                     Singly Linked List                         \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize    = z           Print         = p \n");
		printf(" Insert Node   = i           Delete Node   = d \n");
		printf(" Insert Last   = n           Delete Last   = e\n");
		printf(" Insert First  = f           Delete First  = t\n");
		printf(" Invert List   = r           Quit          = q\n");
		printf("----------------------------------------------------------------\n");

        //반드시 연결리스트르 초기화하는 과정이 필요함: 현재 headNode는 구조체 크기의 동적할당을 받지 않았기 때문에 가리킬 수 있는 게 없음.
        //근데 삽입을 보면 헤드가 어떤 것을 가리키고 있어야만 진행되는 상황임. 
		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {

        //연결리스트 초기화
		case 'z': case 'Z':
			headnode = initialize(headnode);
			break;

        //연결리스트 출력
		case 'p': case 'P':
			printList(headnode);
			break;

        //연결리스트의 적절한 위치(오름차순)에 노드 삽입
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insertNode(headnode, key);
			break;

        //연결리스트 사이에 있는 노드 삭제
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(headnode, key);
			break;

        //연결리스트 마지막에 노드 삽입
		case 'n': case 'N':
			printf("Your Key = ");
			scanf("%d", &key);
			insertLast(headnode, key);
			break;

        //연결리스트 마지막 노드 삭제
		case 'e': case 'E':
			deleteLast(headnode);
			break;

        //연결리스트 시작에 노드 삽입
		case 'f': case 'F':
			printf("Your Key = ");
			scanf("%d", &key);
			insertFirst(headnode, key);
			break;

        //연결리스트 시작 노드 삭제
		case 't': case 'T':
			deleteFirst(headnode);
			break;

        //연결리스트 전치
		case 'r': case 'R':
			invertList(headnode);
			break;

        //연결리스트 종료: 동적할당해제 
		case 'q': case 'Q':
			freeList(headnode);
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

/*연결리스트 초기화*/
headNode* initialize(headNode* h) {

    /*
    <h값이 NULL이 아닐 경우>
    h는 동적할당을 받은 주소를 가지고 있을것이다.
    */
    if(h != NULL)
		freeList(h);

    /*
    <헤드포인터를 굳이 동적할당으로 선언해야하는 이유> 
	1) 포인터로 선언할 경우에는 그 포인터에 어떤 변수값을 넣어야 포인터로서 의미가 있음.
    2) 그 변수를 그래서 동적할당으로 선언하여 headNode크기를 가진 변수가 있다고 생각할 수 있음/
    */
    headNode* temp = (headNode*)malloc(sizeof(headNode));
    //새로 다시 선언한 이유는 위에서 헤드노드를 동적할당해제하였기 때문에 새로운 헤드노드가 필요한 것이다.
	temp->first = NULL; //초기화하는 것이므로 헤드노드 구조체에 있는 첫 헤드를 가리키는 temp의 주소값을 NULL로 처리함

	return temp; //초기화가 진행된 헤드노드 구조체를 반환하여 기존 헤드노드에 반영될 수 있도록 함.
}

/*모든 연결리스트 동적할당해제*/
int freeList(headNode* h){

	listNode* p = h->first; //h->first가 listNode를 가리키는 포인터 변수이기 때문에 listNode*에 값을 복사한 것이다.
                            /*
                            복사한 이유: 
                            헤드포인터 자체도 동적할당이라 해제해주어야하는데, 연결리스트를 추적하면서 그 각각도 해제해주어야한다.
                            근데 이걸 헤드포인터 값을 옮기면서 하면 헤드포인터가 값을 잃기 때문에 새로운 것을 하나 만들어 준것이다.
                            */
	listNode* prev = NULL;  //p가 추적하는 용도라면, 추적한 p를 해제할 것이 필요하다. 이때 p를 해제해주면 현재 추적위치를 잃기 때문에 p값을 다른 곳에 저장한 뒤, p는 다시 다른 노드를 추적해야한다. 
                            //처음에는 해제할 게 없기 때문에 우선 NULL 값 할당함

    //p가 NULL이라는 것은 현재 위치의 노드의 link값이 NULL이고, 그것은 마지막노드라는 뜻이다.
	while(p != NULL) {
		prev = p;    //현재 p의 위치를 저장한다.
		p = p->link; //p는 다음위치를 가르킨다.
		free(prev);  //p가 다음으로 옮겨갔으니 그 전 노드는 동적할당해제한다.
	}

	free(h); //헤드포인터를 할당해제한다. 
	return 0;
}

/*연결리스트 첫 번째 노드 삽입*/
int insertFirst(headNode* h, int key) {

	listNode* node = (listNode*)malloc(sizeof(listNode)); //새로운 노드 생성
	node->key = key;         //새로운 노드의 key값을 주어진 key값으로 저장함
	node->link = h->first;   //원래 헤드포인터가 가르키고 있던 첫 주소를 새로운 노드의 다음 노드 주소를 담는 곳으로 저장함.
    h->first = node;         //동적할당 받은 주소를 헤드포인터 구조체의 첫 주소를 담는 변수에 저장

	return 0;
}

/*연결리스트의 적절한 위치(오름차순)에 노드 삽입*/
int insertNode(headNode* h, int key) {

    listNode* node = (listNode*)malloc(sizeof(listNode)); //새로운 노드 생성
    node->key = key;

    /*연결리스트 첫 노드 삽입*/
    if(h->first == NULL){
        h->first = node;
        node->link = NULL;

        return 0;
    }

    /*연결리스트에 첫 노드보다 key값이 작을경우*/
    //따로 빼주는 이유: h->first 값 자체를 바꿔줘야한다.(이 값을 저장하고, 저장한 값을 바꾸는 거는 의미없음)
    listNode* p = h->first; //현재 위치를 나타내는 변수
    
    if(key <= p->key){
        node->link = p;
        h->first = node;

        return 0;
    }

    /*주어진 key값이 연결리스트의 값들보다는 작을 경우*/
	listNode* prev = NULL; //p의 전 위치를 기록해주는 변수

    while(p != NULL){
        if(key <= p->key){
            node->link = p;
            prev->link = node;
            return 0;
        }
		prev = p;
        p = p->link;
    }

    /*주어진 key값이 연결리스트의 값들보다 클 경우*/
    prev->link = node;
	node->link = NULL;
	return 0;
}

/*마지막노드로 삽입*/
int insertLast(headNode* h, int key) {

	listNode* node = (listNode*)malloc(sizeof(listNode)); //새로운 노드 생성
    node->key = key;

	listNode* p = h->first; //현재가 끝인지 알려주는 변수
	listNode* prev = NULL;  //현재 끝을 가리키는 변수

	/*마지막 노드까지 탐색하여 p의 값이 NULL이 되면, prev는 마지막 노드의 주소를 담고 있게된다.*/
	while(p != NULL){
		prev = p;
		p = p->link;
	}

	/*마지막 노드 값 변경*/
	//마지막 노드의 주소를 담고있는 prev를 새로운 노드의 주소로 담는다.
	//새로운 노드가 마지막 노드이므로 NULL을 link에 추가한다.
	prev->link = node;
	node->link = NULL;

	return 0;
}

/*첫번째 노드 삭제: 헤드포인터 값 변경*/
int deleteFirst(headNode* h) {

	listNode* p = h->first; //삭제시킬 노드를 저장할 변수: 동적할당해제를 위해서 + 첫 노드의 다음 노드를 가리키기 위해서

	h->first = p->link; //헤더포인터를 현재 노드의 link값, 즉 다음 노드의 주소로 변경한다.
	free(p); //기존 첫노드는 삭제되었으므로 동적할당해제한다.

	return 0;
}


/*주어진 key값을 가진 노드 삭제*/
int deleteNode(headNode* h, int key) {

	listNode* p = h->first; //현재가 주어진 key를 가진 노드 위치임을 알려주는 변수
	listNode* prev = NULL;  //실제 주어진 key를 가진 노드의 전위치임을 알려주는 변수

	while (p->key != key)
	{	
		prev = p;
		p = p->link;
	}

	prev->link = p->link; // 주어진 key를 가진 노드의 전위치의 link값을  주어진 key를 가진 다음 노드 위치로 변경함

	free(p); //삭제할 노드를 동적할당 해제함

	return 0;

}

/*마지막 노드 삭제*/
int deleteLast(headNode* h) {

	listNode* p = h->first; //현재가 끝인지 알려주는 변수
	listNode* prev = NULL;  //마지막 노드의 전 노드

	while(p->link != NULL){
		prev = p;
		p = p->link;
	}

	prev->link = NULL; //마지막 노드의 전 노드의 LINK값을 NULL로 바꿔줌: 마지막 노드가 되었으니깐
	free(p);

	return 0;
}

/*연결리스트 전치*/
int invertList(headNode* h) {

	listNode* now = h->first; //현재 노드를 나타내는 변수
	listNode* a = NULL;       //현재 노드 기준 전 노드의 주소를 담은 변수
	listNode* b = NULL;       //현재 노드를 저장할 변수

	/*
	[흐름설명]
	1) 현재 노드 기준 전 노드의 주소를 a에 저장한다.
	2) a는 현재 노드의 주소를 저장한다.
	3) now는 현재 노드의 다음 노드 주소를 저장한다.
	4) a는 현재 노드의 다음 노드 주소에 전 노드의 주소를 저장한다.
	5) 이것을 다음노드의 link값이 NULL일때까지 반복한다.
	6) 그래야지 now에 마지막 노드의 주소가 남기 때문이다.
	*/
	while (now->link != NULL)
	{
		a = b; 
		b = now; 
		now = now->link; 
		b->link = a; 
	}

	//마지막 노드를 전 노드로 처리하고, 헤드포인터를 마지막노드로 옮긴다.
	a = b; 
	b = now; 
	b->link = a;
	h->first = now;

	return 0;
}


void printList(headNode* h) {
	int i = 0;
	listNode* p; //헤드포인터는 움직이면 안되므로 움직일 변수가 하나 필요함.

	printf("\n---PRINT\n");

	if(h == NULL) {
		printf("Nothing to print....\n");
		return;
	}

	p = h->first;

	while(p != NULL) {
		printf("[ [%d]=%d ] ", i, p->key);
		p = p->link;
		i++;
	}



	printf("  items = %d\n", i);
}
