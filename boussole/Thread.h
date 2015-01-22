#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <iostream>



class Thread : public QThread
{
    Q_OBJECT

    private:
        double value;
        static int const SIZE = 7;
        char buffer[SIZE];

        void run();

    public:
        void formatValue();
        virtual ~Thread(){}

    signals:
        void changed2(double);

};

#endif
