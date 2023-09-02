// простенький таймер на миллис
#include <Arduino.h>

// Timer tik(1000, 1);    1 - повторы  0 - сработает 1 раз
//    reinit(1000, 1);    1 - повторы  0 - сработает 1 раз
class Timer
{
public:
    Timer(uint16_t time, bool mode = 1) // время и тип таймера
    {
        _time = time;
        _mode = mode;
        _tmr = millis();
    }

    void reinit(uint16_t time, bool mode = 1)
    {
        _time = time;
        _tmr = millis();
        _mode = mode;
    }

    bool tik()
    {
        if (millis() - _tmr >= _time)
        {
            if (_mode)
                return true;
            else
            {
                _tmr += _time;
                return true;
            }
        }
        return false;
    }

private:
    uint32_t _tmr;
    uint16_t _time;
    bool _mode;
};