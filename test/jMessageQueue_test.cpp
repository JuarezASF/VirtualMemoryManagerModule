//
// Created by jasf on 5/15/16.
//

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <unordered_map>
#include "../src/jMessageQueue.h"

using namespace std;

int main() {

    jMessageQueue q;
    q.create(0x01);

    unordered_map<int, int> pidIdxMap;

    //launch ten process


    int pid;
    for (int k = 0; k < 10; k++) {
        if ((pid = fork()) == 0) {
            int idx = k;
            jMessageQueue q2;
            q2.start(0x01);

            jMessageQueue answer_queue;
            answer_queue.create(getpid());

            for (int i = 0; i < 10; i++) {

                q2.sendRequest(i, answer_queue.msgqid);
                jMessageQueue::AnswerMsg a = answer_queue.getAnswer();
                cout << "idx:" << idx << ":" << i << "->" << a.frame << endl;
            }

            cout << "user process done" << endl;
            q2.sendRequest(-1, 0);
            answer_queue.destroy();
            exit(0);


        }
        else {
            pidIdxMap[pid] = k;
        }

    }

    for (auto e : pidIdxMap)
        cout << e.first << ":" << e.second << endl;

    jMessageQueue q2;
    q2.start(0x01);
    int done = 0;
    while (true) {
        jMessageQueue::RequestMessage a = q2.getRequest();

        if (a.page < 0) {
            done += 1;
            if (done == 10)
                break;

            continue;

        }
        int r = pidIdxMap[a.pid] * 10 + a.page;
        jMessageQueue::sendAnswer(r, a.returnToQueueId);
    }

    int s;
    for (int i = 0; i < 10; i++) {
        wait(&s);
    }
    cout << "server done" << endl;


    q.destroy();

}
