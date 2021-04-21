#include "usersettings.h"

bool UserSettings::range()
{
    return m_range;
}

bool UserSettings::functions()
{
    return m_functions;
}

bool UserSettings::capture()
{
    return m_capture;
}

bool UserSettings::palette()
{
    return m_palette;
}

bool UserSettings::measure()
{
    return m_measure;
}

bool UserSettings::advancedVisible()
{
    return m_advancedVisible;
}

bool UserSettings::advancedImageVisible()
{
    return m_advancedImageVisible;
}

bool UserSettings::advancedAlarms()
{
    return m_advancedAlarms;
}

bool UserSettings::advancedMemory()
{
    return m_advancedMemory;
}

bool UserSettings::advancedSystem()
{
    return m_advancedSystem;
}

bool UserSettings::advancedRoi()
{
    return m_advancedRoi;
}

bool UserSettings::advancedDio()
{
    return m_advancedDio;
}

bool UserSettings::advancedLayout()
{
    return m_advancedLayout;
}

void UserSettings::setRange(bool val)
{
    m_range = val;
    emit rangeChanged();

}

void UserSettings::setFunctions(bool val)
{
    m_functions = val;
    emit functionsChanged();
}
void UserSettings::setCapture(bool val)
{
    m_capture = val;
    emit captureChanged();
}
void UserSettings::setPalette(bool val)
{
    m_palette = val;
    emit paletteChanged();
}
void UserSettings::setMeasure(bool val)
{
    m_measure = val;
    emit measureChanged();
}
void UserSettings::setAdvancedVisible(bool val)
{
    m_advancedVisible = val;
    emit advancedVisibleChanged();
}
void UserSettings::setAdvancedImageVisible(bool val)
{
    m_advancedImageVisible = val;
    emit advancedImageVisibleChanged();
}
void UserSettings::setAdvancedAlarms(bool val)
{
    m_advancedAlarms = val;
    emit advancedAlarmsChanged();
}
void UserSettings::setAdvancedMemory(bool val)
{
    m_advancedMemory = val;
    emit advancedMemoryChanged();
}
void UserSettings::setAdvancedSystem(bool val)
{
    m_advancedSystem = val;
    emit advancedSystemChanged();
}
void UserSettings::setAdvancedRoi(bool val)
{
    m_advancedRoi = val;
    emit advancedRoiChanged();
}
void UserSettings::setAdvancedDio(bool val)
{
    m_advancedDio = val;
    emit advancedDioChanged();
}
void UserSettings::setAdvancedLayout(bool val)
{
    m_advancedLayout = val;
    emit advancedLayoutChanged();
}
