#ifndef QUEUE_H
#define QUEUE_H

typedef char element; // 요소의 타입

typedef struct QueueNode // 큐의 노드의 타입
{
	element data;
	struct QueueNode* link;
} QueueNode;

typedef struct // 큐 ADT 구현
{
	QueueNode *front, *rear;
} LinkedQueueType;

void initialize(LinkedQueueType* q);
int isEmpty(LinkedQueueType* q);
void enqueue(LinkedQueueType* q, element data);
element dequeue(LinkedQueueType* q);

#endif