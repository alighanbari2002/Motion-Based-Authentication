#ifndef DEFS_H
#define DEFS_H

namespace Thresh {
    const double START_ACC = 0.2;
    const double START_ROTATION = 60;
    const double STOP_ACC_Y = 0.05;
    const double STOP_ACC_X = 0.05;
    const double STOP_ROTATION = 0.05;
    const int CALIBRATION_LIMIT = 10;
}

namespace System {
    const double DATARATE = 50;
    const double SAMPLE_COUNT = 5;
    const double SAMPLE_DATARATE = DATARATE / SAMPLE_COUNT;
}

struct Calibration {

};


#endif // DEFS_H
