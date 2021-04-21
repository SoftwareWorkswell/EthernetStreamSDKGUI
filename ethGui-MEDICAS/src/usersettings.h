#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <QObject>

/**
 * @brief The UserSettings class
 * @details Holds settings for specific user - viz manual (page 46)
 */
class UserSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool range READ range NOTIFY rangeChanged)
    Q_PROPERTY(bool functions READ functions NOTIFY functionsChanged)
    Q_PROPERTY(bool capture READ capture WRITE setCapture NOTIFY captureChanged)
    Q_PROPERTY(bool measure READ measure WRITE setMeasure NOTIFY measureChanged)
    Q_PROPERTY(bool advancedVisible READ advancedVisible WRITE setAdvancedVisible NOTIFY advancedVisibleChanged)
    Q_PROPERTY(bool advancedImageVisible READ advancedImageVisible WRITE setAdvancedImageVisible NOTIFY advancedImageVisibleChanged)
    Q_PROPERTY(bool advancedAlarms READ advancedAlarms WRITE setAdvancedAlarms NOTIFY advancedAlarmsChanged)
    Q_PROPERTY(bool advancedMemory READ advancedMemory WRITE setAdvancedMemory NOTIFY advancedMemoryChanged)
    Q_PROPERTY(bool advancedSystem READ advancedSystem WRITE setAdvancedSystem NOTIFY advancedSystemChanged)
    Q_PROPERTY(bool advancedRoi READ advancedRoi WRITE setAdvancedRoi NOTIFY advancedRoiChanged)
    Q_PROPERTY(bool advancedDio READ advancedDio WRITE setAdvancedDio NOTIFY advancedDioChanged)
    Q_PROPERTY(bool advancedLayout READ advancedLayout WRITE setAdvancedLayout NOTIFY advancedLayoutChanged)

public:
    UserSettings(QObject *parent = nullptr) : QObject(parent) {}
    bool range();
    bool functions();
    bool capture();
    bool palette();
    bool measure();
    bool advancedVisible();
    bool advancedImageVisible();
    bool advancedAlarms();
    bool advancedMemory();
    bool advancedSystem();
    bool advancedRoi();
    bool advancedDio();
    bool advancedLayout();

    void setRange(bool val);
    void setFunctions(bool val);
    void setCapture(bool val);
    void setPalette(bool val);
    void setMeasure(bool val);
    void setAdvancedVisible(bool val);
    void setAdvancedImageVisible(bool val);
    void setAdvancedAlarms(bool val);
    void setAdvancedMemory(bool val);
    void setAdvancedSystem(bool val);
    void setAdvancedRoi(bool val);
    void setAdvancedDio(bool val);
    void setAdvancedLayout(bool val);

signals:
    void rangeChanged();
    void functionsChanged();
    void captureChanged();
    void paletteChanged();
    void measureChanged();
    void advancedVisibleChanged();
    void advancedImageVisibleChanged();
    void advancedAlarmsChanged();
    void advancedMemoryChanged();
    void advancedSystemChanged();
    void advancedRoiChanged();
    void advancedDioChanged();
    void advancedLayoutChanged();

private:
    bool m_range = false;
    bool m_functions = false;
    bool m_capture = false;
    bool m_palette = false;
    bool m_measure = false;
    bool m_advancedVisible = false;
    bool m_advancedImageVisible = false;
    bool m_advancedAlarms= false;
    bool m_advancedMemory = false;
    bool m_advancedSystem = false;
    bool m_advancedRoi = false;
    bool m_advancedDio = false;
    bool m_advancedLayout = false;
};

#endif // USERSETTINGS_H
