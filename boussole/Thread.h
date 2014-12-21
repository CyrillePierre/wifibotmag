#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <iostream>

class Thread : public QThread
{
    Q_OBJECT

    private:
        double value;

        void run()
        {
            while(1)
            {
                std::cin >> value;
				value = 360 - value;
                emit changed2(value);
            }
        }

    public:
        virtual ~Thread(){}

    signals:
        void changed2(double);

};

#endif
