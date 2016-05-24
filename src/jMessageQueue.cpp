//
// Created by jasf on 5/15/16.
//

#include "jMessageQueue.h"
#include "AbstractProcess.h"
#include <sys/msg.h>

using namespace std;

int jMessageQueue::requestMsgId = 0x6789;
int jMessageQueue::anserMsgId = 0x9876;

jMessageQueue::jMessageQueue() {

}

void jMessageQueue::create(int key) {

    if ((msgqid = msgget(key, IPC_CREAT | IPC_EXCL | 0664)) < 0) {

        std::cerr << strerror(errno) << std::endl;
        std::cerr << "Trying to get already created queue" << endl;

        if ((msgqid = msgget(key, IPC_CREAT | 0664)) < 0) {
            std::cerr << strerror(errno) << std::endl;
            std::cerr << "Could not event get old queue!" << endl;

        }
    }

}

void jMessageQueue::start(int key) {
    if ((msgqid = msgget(key, IPC_CREAT | 0664)) < 0) {
        std::cerr << "Failed when trying to start queue on key:" << key << endl;
        std::cerr << strerror(errno) << std::endl;
    }
}

void jMessageQueue::destroy() {

    if (msgctl(msgqid, IPC_RMID, 0) < 0) {
        std::cerr << "Error while destroying queue!" << endl;
        std::cerr << strerror(errno) << std::endl;
    }

}

void jMessageQueue::sendRequest(int pageRequested, int answerToQueueId) {

    RequestMessage m;
    m.msgTypeId = jMessageQueue::requestMsgId;
    m.pid = getpid();
    m.page = pageRequested;
    m.returnToQueueId = answerToQueueId;

    int length = sizeof(RequestMessage) - sizeof(long);

    if (msgsnd(msgqid, &m, length, 0) < 0) {
        cerr << "Error while sending msg!" << endl;
        cerr << strerror(errno) << endl;
    }


}

jMessageQueue::AnswerMsg jMessageQueue::getAnswer() {

    AnswerMsg m;
    int length = sizeof(AnswerMsg) - sizeof(long);

    if (msgrcv(msgqid, &m, length, jMessageQueue::anserMsgId, 0) < 0) {
        cerr << "Error while receiving msg!" << endl;
        cerr << strerror(errno) << endl;
    }

    return m;
}

jMessageQueue::AnswerMsg jMessageQueue::getAnswer(int msgId) {

    AnswerMsg m;
    int length = sizeof(AnswerMsg) - sizeof(long);

    if (msgrcv(msgqid, &m, length, msgId, 0) < 0) {
        cerr << "Error while receiving msg!" << endl;
        cerr << strerror(errno) << endl;
    }

    return m;
}

jMessageQueue::RequestMessage jMessageQueue::getRequest() {
    RequestMessage m;
    int length = sizeof(RequestMessage) - sizeof(long);

    if (msgrcv(msgqid, &m, length, 0, 0) < 0) {
        cerr << "Error while receiving msg!" << endl;
        cerr << strerror(errno) << endl;
    }

    return m;
}

void jMessageQueue::sendAnswer(int frame, int toQueueId) {
    AnswerMsg m;
    m.msgTypeId = jMessageQueue::anserMsgId;
    m.frame = frame;

    int length = sizeof(AnswerMsg) - sizeof(long);

    if (msgsnd(toQueueId, &m, length, 0) < 0) {
        cerr << "Error while sending msg!" << endl;
        cerr << strerror(errno) << endl;
    }

}

void jMessageQueue::sendAnswer(int frame, int toQueueId, int toMsgId, bool pageFault) {
    AnswerMsg m;
    m.msgTypeId = toMsgId;
    m.frame = frame;
    m.pagefault = pageFault;

    int length = sizeof(AnswerMsg) - sizeof(long);

    if (msgsnd(toQueueId, &m, length, 0) < 0) {
        cerr << "Error while sending msg!" << endl;
        cerr << strerror(errno) << endl;
    }

}
