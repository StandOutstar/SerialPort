#ifndef UART1_THREAD_H
#define UART1_THREAD_H
#include <QThread>

class uart1_thread : public QThread
{
public:
    uart1_thread();
protected:
    void run();

};

#endif // UART1_THREAD_H
