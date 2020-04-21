#ifndef CAMERACONSTANTS_H
#define CAMERACONSTANTS_H

struct CameraConstants
{
    // limits for range and also possible thresholds
    const int _maxRangeTemp = 42;
    const int _minRangeTemp = 30;
    // visible camera parameters limits
    const int _maxNoiseReduction = 5;
    const int _maxColorGain = 200;
    const int _minColorGain = 60;
    const int _maxGain = 34;
    const int _minGain = 0;
    const int _maxExposureCompensation = 6;
    const int _minExposureCompensation = -6;
    const double _maxShutter = 1.0;
    const double _minShutter = 1.0 / 1000;
    const double _maxLevelOfAcception = 3.0;
    const double _minLevelOfAcception = 0.0;
    // alarm graphic setting
    const int _maxAlarmOpacity = 100;
    const int _minAlarmOpacity = 50;
};

#endif // CAMERACONSTANTS_H
