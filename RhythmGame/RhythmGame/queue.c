#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

void initialize(LinkedQueueType* q)
{
	q->front = q->rear = NULL;
}

// 큐가 비었는지 검사하는 함수
int isEmpty(LinkedQueueType* q)
{
	return q->front == NULL;
}

// 삽입 함수
void enqueue(LinkedQueueType* q, element data)
{
	QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));

	temp->data = data; // 데이터 저장
	temp->link = NULL; // 링크 필드를 NULL

	if (isEmpty(q)) // 큐가 공백이면
	{
		q->front = temp;
		q->rear = temp;
	}
	else // 큐가 공백이 아니면
	{
		q->rear->link = temp; // 순서가 중요
		q->rear = temp;
	}
}

element dequeue(LinkedQueueType* q)
{
	if (isEmpty(q)) // 공백 상태
	{
		fprintf(stderr, "스택이 비어 있음\n");
		exit(1);
	}

	QueueNode* temp = q->front;
	element data = temp->data; // 데이터를 꺼낸다.

	q->front = q->front->link; // front로 다음 노드

	if (q->front == NULL) // 공백 상태
	{
		q->rear = NULL;
	}

	free(temp); //동적 메모리 해제
	return data; //데이터 반환
}
