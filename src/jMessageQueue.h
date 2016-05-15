//
// Created by jasf on 5/15/16.
//

#ifndef VIRTUALMEMORYMANAGER_JMESSAGEQUEUE_H
#define VIRTUALMEMORYMANAGER_JMESSAGEQUEUE_H


class jMessageQueue {

private:

public:
    int msgqid;
    static int requestMsgId;

    typedef struct RequestMessage {
        long msgTypeId;
        int pid;
        int page;
        int returnToQueueId;
        int returnToMsgTypeId;

    } RequestMessage;

    static int anserMsgId;
    typedef struct AnswerMsg {
        long msgTypeId;
        int frame;
    } AnswerMsg;

    jMessageQueue();

    void create(int key);

    void start(int key);

    void destroy();

    void sendRequest(int pageRequested, int answerToQueueId);
    RequestMessage getRequest();

    static void sendAnswer(int frame, int toQueueId);
    static void sendAnswer(int frame, int toQueueId, int toMsgId);
    AnswerMsg getAnswer();
    AnswerMsg getAnswer(int withThisId);



};


#endif //VIRTUALMEMORYMANAGER_JMESSAGEQUEUE_H