#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>

#define TRUE	1
#define FALSE	0
#define OK 		1
#define ERROR 	0
typedef int Status;
typedef struct LNode {
    int OccurTime;
    int NType;
    struct LNode* nextL;
} LNode, *LPtr;
typedef struct List {
    LPtr head;
    int LLength;
} List;
typedef struct QNode {
    int ArrivalTime;
    int Duration;
    struct QNode *nextQ;
} QNode, *QPtr;
typedef struct Queue {
    QPtr front;
    QPtr rear;
    int QLength;
} Queue;

List	ev;
LNode 	en;
Queue	q[5];
QPtr	customer;
int TotalTime, CustomerNum;
int CloseTime;
int LastArrive;
//List
Status InitList(List &L);
LPtr CreatLNode(int OT, int N);
LPtr DelFirst(List &L);
Status ListEmpty(List L);
//Queue
Status InitQueue(Queue &Q);
Status QueueEmpty(Queue Q);
QPtr CreatQNode(int AT, int DT);
Status EnQueue(Queue &Q, QPtr e);
Status DeQueue(Queue &Q, QPtr &e);
Status GetQHead(Queue Q, QPtr &e);
//Bank
void OpenForDay();
void OrderInsert(List &ev, LNode ten);
void CustomerArrived();
int cmp(LNode a, LNode b);
void CustomerDeparture();
void Bank_Simulation(int CT);
//List
Status InitList(List &L) {
    L.head = (LPtr)malloc(sizeof(LNode));
    if (L.head == NULL)	return ERROR;
    L.head->nextL = NULL;
    L.LLength = 0;
    return OK;
}

LPtr CreatLNode(int OT, int N) {
    LPtr Ltemp = (LPtr)malloc(sizeof(LNode));
    Ltemp->OccurTime = OT;
    Ltemp->NType = N;
    Ltemp->nextL = NULL;
    return Ltemp;
}

LPtr DelFirst(List &L) {
    if (L.head == NULL || L.LLength == 0)	return FALSE;
    LPtr relnode = L.head->nextL;
    L.head->nextL = relnode->nextL;
    relnode->nextL = NULL;
    L.LLength--;
    return relnode;
}

int cmp(LNode a, LNode b) {
    if (a.OccurTime<b.OccurTime)	return -1;
    else if (a.OccurTime == b.OccurTime)	return 0;
    else								return 1;
}

//Queue
Status InitQueue(Queue &Q) {
    Q.front = Q.rear = (QPtr)malloc(sizeof(QNode));
    if (!Q.front)	exit(-2);
    Q.front->nextQ = NULL;
    Q.QLength = 0;
    return OK;
}

Status QueueEmpty(Queue Q) {
    if (Q.QLength == 0)	return TRUE;
    return FALSE;
}

QPtr CreatQNode(int AT, int DT) {
    QPtr Qtemp = (QPtr)malloc(sizeof(QNode));
    Qtemp->ArrivalTime = AT;
    Qtemp->Duration = DT;
    Qtemp->nextQ = NULL;
    return Qtemp;
}

Status EnQueue(Queue &Q, QPtr e) {
    Q.rear->nextQ = e;
    Q.rear = Q.rear->nextQ;
    Q.QLength++;
    return OK;
}

Status DeQueue(Queue &Q, QPtr &e) {
    e = Q.front->nextQ;
    Q.front->nextQ = e->nextQ;
    e->nextQ = NULL;
    if (e == Q.rear)
        Q.rear = Q.front;
    Q.QLength--;
    return OK;
}

Status GetQHead(Queue Q, QPtr &e) {
    e = (QPtr)malloc(sizeof(QNode));
    e->ArrivalTime = Q.front->nextQ->ArrivalTime;
    e->Duration = Q.front->nextQ->Duration;
    e->nextQ = NULL;
    return OK;
}

//Bank
void OpenForDay() {
    printf("开门了\n");
    TotalTime = 0;
    CustomerNum = 1;
    LastArrive = 0;
    InitList(ev);
    OrderInsert(ev, *CreatLNode(0, 0));
    for (int i = 0; i <= 4; ++i)
        InitQueue(q[i]);
}

void OrderInsert(List &ev, LNode lnode) {
    LPtr p = ev.head;
    LPtr temp = (LPtr)malloc(sizeof(LNode));
    temp->NType = lnode.NType;
    temp->OccurTime = lnode.OccurTime;
    temp->nextL = NULL;
    while (p->nextL != NULL) {
        if (cmp(*(p->nextL), *temp)<0)
            p = p->nextL;
        else {
            temp->nextL = p->nextL;
            p->nextL = temp;
            ev.LLength++;
            return;
        }
    }
    p->nextL = temp;
    ev.LLength++;
    return;
}

int Minimum(Queue* q) {
    int a = (q[1].QLength<=q[2].QLength ? 1 : 2);
    int b = (q[3].QLength<=q[4].QLength ? 3 : 4);
    return (q[a].QLength<=q[b].QLength ? a : b);
}

void CustomerArrived() {
    int durtime = rand() % 30, intertime = rand() % 5;
    LastArrive += intertime;
    if (LastArrive<CloseTime) {
        OrderInsert(ev, *CreatLNode(LastArrive, 0));
        ++CustomerNum;
    }
    int i = Minimum(q);
    EnQueue(q[i], CreatQNode(en.OccurTime, durtime));
    printf("客户排入%d号窗口\n",i);
    if (q[i].QLength == 1)
        OrderInsert(ev, *CreatLNode(en.OccurTime + durtime, i));
}

void CustomerDeparture() {
    int j = en.NType;
    DeQueue(q[j], customer);
    TotalTime += en.OccurTime - customer->ArrivalTime;
    printf("%d分,%d号窗口离开,耗时%d分钟\n", en.OccurTime, en.NType, en.OccurTime - customer->ArrivalTime);
    if (!QueueEmpty(q[j])) {
        GetQHead(q[j], customer);
        OrderInsert(ev, *CreatLNode(en.OccurTime + customer->Duration, j));
    }
}

void draw(Queue* q) {
    printf("——+ \n");
    printf("| 1 | ");
    for(int i=0; i<q[1].QLength; i++)
        printf(" %c",1);
    printf("\n——+ \n");
    printf("| 2 | ");
    for(int i=0; i<q[2].QLength; i++)
        printf(" %c",1);
    printf("\n——+ \n");
    printf("| 3 | ");
    for(int i=0; i<q[3].QLength; i++)
        printf(" %c",1);
    printf("\n——+ \n");
    printf("| 4 | ");
    for(int i=0; i<q[4].QLength; i++)
        printf(" %c",1);
    printf("\n——+ \n");
}

void Bank_Simulation(int CT) {
    OpenForDay();
    CloseTime = CT;
    while (ev.LLength != 0) {
        Sleep(100);
        system("cls");
        en = *DelFirst(ev);
        if (en.NType == 0) {
            printf("%d分,客户到达,", en.OccurTime);
            CustomerArrived();
        }
        else
            CustomerDeparture();
        draw(q);
    }
    system("cls");
    printf("The Average Time is %g\n", (float)TotalTime / CustomerNum);
}

int main() {
    Bank_Simulation(480);
    system("pause");
    return 0;
}


