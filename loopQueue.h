#ifndef __LOOPQUEUE_H__
#define __LOOPQUEUE_H__




#include <stdint.h>
typedef unsigned int lq_size_t;



typedef struct _lq{
    lq_size_t head;
    lq_size_t end;
    lq_size_t max;
}lq_t;

typedef struct _lqv{
    lq_size_t head;
    lq_size_t end;
    lq_size_t max;
    void *data;
}lqv_t;

typedef struct _lqu8{
    lq_size_t head;
    lq_size_t end;
    lq_size_t max;
    uint8_t *data;
}_lqu8_t;




//查询列队是否为空
#define loopQueueIsEmpty(lq)    (                           \
        (lq->head == lq->end)? 1 : 0 )

//查询列队是否为非空
#define loopQueueIsNonEmpty(lq)    (                        \
        (lq->head == lq->end)? 0 : 1 )

//查询列队是否已满
#define loopQueueIsFull(lq)     (                           \
        ((lq->end + 1) % lq->max == lq->head)? 1 : 0 )

//获取列队已经使用的长度
#define loopQueueGetUsedLength(lq)   (                      \
        (lq->end >= lq->head)?                              \
        (lq->end - lq->head):                               \
        ((lq->max - lq->head) + lq->end))

//获取列队没被使用的长度
#define loopQueueGetNotUsedLength(lq)  (                    \
        lq->max - 1 - loopQueueGetUseLength(lq))





//前提是已知道列队没满
#define loopQueueNonFullAddOne(lq)                          \
    lq->end = (lq->end + 1)%lq->max;

//不知道列队是否已满添加 会强制添加
#define loopQueueUnknownFullAddOne(lq)                      \
        loopQueueNonFullAddOne(lq);                         \
        if(lq->end == lq->head) lq->head = (lq->head + 1)%lq->max;

//前提是已知列队中有数据
#define loopQueueNonEmptyTakeOne(lq)                        \
    lq->head=(lq->head+1)%lq->max;

//未知列队中是否有数据 有就减少一个
#define loopQueueUnknownEmptyTakeOne(lq)                    \
        (loopQueueGetUseLength(lq) > 0)?loopQueueNonEmptyGetOne(lq):;

//清空弄列队
#define loopQueueClearAll(lq)                               \
        lq->head = lq->end;








#endif /* #ifndef __LOOPQUEUE_H__ */
