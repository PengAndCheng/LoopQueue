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

//查询列队是否非满
#define loopQueueIsNonFull(lq)     (                        \
        ((lq->end + 1) % lq->max == lq->head)? 0 : 1 )

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
















//8位常用列队
static inline void lqu8Init(_lqu8_t* lq8, uint8_t* u8buf, lq_size_t u8buf_lenght){
    lq8->head = lq8->end = 0;
    lq8->data = u8buf;
    lq8->max = u8buf_lenght;
}

//列队没满才增加数据
static inline lq_size_t lqu8NotForceAddOne(_lqu8_t* lq8,uint8_t data){
    if (loopQueueIsNonFull(lq8)) {
        lq8->data[lq8->end] = data;
        loopQueueNonFullAddOne(lq8);
        return 1;
    }
    return 0;
}

//列队没满才增加数据
static inline lq_size_t lqu8NotForceAdd(_lqu8_t* lq8,uint8_t* data, lq_size_t data_lenght){
    int len = loopQueueGetNotUsedLength(lq8);
    if (len == 0) {
        return 0;
    }
    if (len > data_lenght) {
        len = data_lenght;
    }
    for (int i = 0; i < len; ++i) {
        lq8->data[lq8->end] = data[i];
        loopQueueNonFullAddOne(lq8);
    }
    return len;
}

//不需要知道列队是否已满 强制添加一个数据 如果满录入会覆盖旧数据
static inline void lqu8ForceAddOne(_lqu8_t* lq8,uint8_t data){
    lq8->data[lq8->end] = data;
    loopQueueUnknownFullAddOne(lq8);
}

//不需要知道列队是否已满 强制添加多个个数据 如果满会覆盖旧数据
static inline void lqu8ForceAdd(_lqu8_t* lq8, uint8_t* data, lq_size_t data_lenght){
    for (lq_size_t i = 0; i < data_lenght; ++i) {
        lq8->data[lq8->end] = data[i];
        loopQueueUnknownFullAddOne(lq8);
    }
}

//可获1个数据
static inline lq_size_t lqu8TakeOne(_lqu8_t* lq8, uint8_t* buf){
    lq_size_t len = loopQueueGetUsedLength(lq8);
    if (len == 0) {
        return 0;
    }
    buf[0] = lq8->data[lq8->head];
    loopQueueNonEmptyTakeOne(lq8);
    return 1;
}

//可获取多个数据
static inline lq_size_t lqu8Take(_lqu8_t* lq8, uint8_t* buf, lq_size_t buf_size){
    lq_size_t len = loopQueueGetUsedLength(lq8);
    if (len == 0) {
        return 0;
    }
    if (buf_size < len) {
        len = buf_size;
    }
    for (lq_size_t i = 0; i < len; ++i) {
        buf[i] = lq8->data[lq8->head];
        loopQueueNonEmptyTakeOne(lq8);
    }
    return len;
}



#endif /* #ifndef __LOOPQUEUE_H__ */
