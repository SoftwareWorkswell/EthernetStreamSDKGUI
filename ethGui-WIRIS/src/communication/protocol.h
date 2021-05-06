#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>

/**
 * The Protocol struct
 * Every command used in a communication with camera is defined here.
 * Not all the cameras support all the commands
 */
struct Protocol
{
    // PROTOCOL MANIPULATION METHODS
    // functions to create messages, adds formatting(spaces, delimeter)
    static QString prepareMessage(const QString & command);
    static QString prepareMessage(const QString & command, int arg);
    static QString prepareMessage(const QString & command, bool arg);
    static QString prepareMessage(const QString & command, const QString & args);
    static QString prepareMessage(const QString & command, const QStringList & args);
    // default delimiter is '\n', needs to be used when setting delimiter or checking connection
    static QString prepareMessageWithDefDelim(const QString & command);
    static QString prepareMessageWithDefDelim(const QString & command, const QString & args);
    // message parsing and utils
    static bool isOk(const QString & response);
    static bool isTrue(const QString & response);
    static bool isPositive(const QString & response);
    static bool isOk(const std::string & response);
    static bool isTrue(const std::string & response);
    static bool isPositive(const std::string & response);
    static QString boolToString(bool val);
    static std::istream& getAnyLine(std::istream& is, std::string& t);
    // delimiter used in a communication
    static QString delimiter;

    // COMMAND DEFINITIONS
    // basic
    const QString SET_DELIMITER         = "SDLM";
    const QString GET_DELIMITER         = "GDLM";
    const QString CHECK_CONNECTION      = "HIWS";
    const QString GET_SERIAL_NUMBER     = "GSRN";
    const QString GET_ARTICLE_NUMBER    = "GATN";
    const QString GET_FW_VERSION        = "GFWV";
    const QString GET_CPU_TEMP          = "GTCU";
    const QString GET_CAMERA_CORE_TEMP  = "GTIC";
    const QString GET_DEVICE_TEMP       = "GTIN";
    const QString GET_FAN_POWER         = "GFPW";
    const QString SET_SBUS_LOGGING      = "SLSB";
    const QString SET_MAVLINK_LOGGING   = "SLSB";
    const QString SET_THERMAL_UNITS     = "STUT";
    const QString GET_THERMAL_UNITS     = "GTUT";
    const QString GET_COOLDOWN_TIME     = "GCDT";
    // gps
    const QString GET_GPS_COORDS = "GGPS";
    // activation
    const QString ACTIVATE      = "ACTV";
    const QString IS_ACTIVATED  = "IACT";
    // ethernet mode
    const QString SET_ETH_MODE = "SETH";
    const QString GET_ETH_MODE = "GETH";
    // range
    const QString SET_RANGE_MODE   = "SRMD";
    const QString GET_RANGE_MODE   = "GRMD";
    const QString SET_MANUAL_RANGE = "SRMM";
    const QString GET_MANUAL_RANGE = "GRMM";
    const QString GET_SPAN_RANGE   = "GRWC";
    const QString SET_SPAN_RANGE   = "SRWC";
    // environment - absolute temp. range
    const QString GET_ENVIRONMENT       = "GREN";
    const QString GET_ENVIRONMENT_LIST  = "GREL";
    const QString SET_ENVIRONMENT       = "SREN";
    // wiris cecurity thermal parameters
    const QString GET_TIME_STABILIZATION = "GTST";
    const QString SET_TIME_STABILIZATION = "STST";
    const QString GET_HOT_REJECTION      = "GHRJ";
    const QString SET_HOT_REJECTION      = "SHRJ";
    const QString GET_COLD_REJECTION     = "GCRJ";
    const QString SET_COLD_REJECTION     = "SCRJ";
    // functions
    const QString SET_MAIN_CAMERA       = "SMCA";
    const QString GET_MAIN_CAMERA       = "GMCA";
    const QString SET_TEMPERATURE_MODE  = "STOF";
    const QString GET_TEMPERATURE_MODE  = "GTOF";
    // appearance
    const QString SET_HDMI_LAYOUT                   = "SLAY";
    const QString SET_THERMAL_CAMERA_TRANSPARENCY   = "STTY";
    // zoom
    const QString ZOOM_IN                   = "SZIN";
    const QString ZOOM_OUT                  = "SZOT";
    const QString GET_THERMAL_ZOOM          = "GZTV";
    const QString GET_THERMAL_ZOOM_LIST     = "GZTL";
    const QString GET_THERMAL_INDEX         = "SZTN";
    const QString GET_VISIBLE_ZOOM          = "GZVV";
    const QString GET_VISIBLE_ZOOM_LIST     = "GZVL";
    const QString GET_VISIBLE_INDEX         = "GZVN";
    const QString SET_THERMAL_ZOOM          = "STZV";
    const QString SET_THERMAL_ZOOM_INDEX    = "SZTN";
    const QString SET_VISIBLE_ZOOM          = "SZVV";
    const QString SET_VISIBLE_ZOOM_INDEX    = "SZVN";
    // gis focus
    const QString GIS_AUTO_FOCUS        = "SGFA";
    const QString GIS_INFINITE_FOCUS    = "SGFI";
    // gis  modes
    const QString SET_GIS_DIFFERENTIAL_MODE     = "SDGM";
    const QString SET_GIS_HIGH_SENSITIVITY_MODE = "SHSM";
    const QString GET_GIS_DIFFERENTIAL_MODE     = "GDGM";
    const QString GET_GIS_HIGH_SENSITIVITY_MODE = "GHSM";
    // capture
    const QString CAPTURE_IMAGE                 = "CPTR";
    const QString CHECK_CAPTURE                 = "ICPT";
    const QString START_RECORDING               = "RCRS";
    const QString FINISH_RECORDING              = "RCRF";
    const QString CHECK_IS_RECORDING            = "IRCR";
    const QString SET_LENGTH_ROTATION_RECORDING = "SRLN";
    const QString GET_LENGTH_ROTATION_RECORDING = "GRLN";
    // palette
    const QString SET_PALETTE_BY_NAME       = "SPTE";
    const QString SET_PALETTE_BY_INDEX      = "SPTI";
    const QString GET_PALETTE_NAME_INDEX    = "GPTE";
    const QString GET_PALETTE_VALUES        = "GPLV";
    const QString GET_PALETTE_LIST          = "GPTL";
    // alarms
    const QString SET_ALARM_MODE = "SALM";
    const QString GET_ALARM_MODE = "GALM";
    const QString SET_ALARMS     = "SALV";
    const QString GET_ALARMS     = "GALV";
    const QString GET_ALARM_ONLY_AFTER_TRIGGER = "GSAT";
    const QString SET_ALARM_ONLY_AFTER_TRIGGER = "SSAT";
    const QString GET_ALARM_FLASH = "GLED";
    const QString SET_ALARM_FLASH = "SLED";
    const QString GET_ALARM_BUZZ= "GBUZ";
    const QString SET_ALARM_BUZZ = "SBUZ";
    // alarms graphic settings
    const QString SET_ALARMS_COLORS = "SALC";
    const QString GET_ALARMS_COLORS = "GALC";
    const QString SET_THRESHOLDS_OPACITY = "SAOP";
    const QString GET_THRESHOLDS_OPACITY = "GAOP";
    // measure - display settings
    const QString SET_SHOW_CROSS_MAX = "SSCM";
    const QString GET_SHOW_CROSS_MAX = "GSCM";
    const QString SET_SHOW_CROSS_USER = "SSCU";
    const QString GET_SHOW_CROSS_USER = "GSCU";
    const QString SET_SHOW_USER_ROI = "SSHR";
    const QString GET_SHOW_USER_ROI = "GSHR";
    const QString GET_THERMAL_EXTREMES = "GTEX";
    // thermal camera settings
    const QString GET_THERMAL_RESOLUTION = "GTRE";
    const QString GET_EMISSIVITY = "GTEM";
    const QString SET_EMISSIVITY = "STEM";
    const QString GET_REFLECTED_TEMP = "GTRT";
    const QString SET_REFLECTED_TEMP = "STRT";
    const QString GET_ATMOSPHERIC_TEMP = "GTAT";
    const QString SET_ATMOSPHERIC_TEMP = "STAT";
    const QString SET_INTERPOLATION = "STII";
    const QString GET_INTERPOLATION = "GTII";
    const QString GET_SHUTTER_PERIOD = "GTCP";
    const QString SET_SHUTTER_PERIOD = "STCP";
    const QString GET_SYNC_SHUTTER = "GTSC";
    const QString SET_SYNC_SHUTTER = "STSC";
    const QString PERFORM_SHUTTER = "DTSR";
    const QString GET_SECONDS_TO_SHUTTER = "GTSN";
    const QString GET_SECONDS_FROM_SHUTTER = "GTSL";
    // image and video saving setting
    const QString SET_IMAGE_SAVE_DEST           = "SILC";
    const QString GET_IMAGE_SAVE_DEST           = "GILC";
    const QString SET_VIDEO_SAVE_DEST           = "SVLC";
    const QString GET_VIDEO_SAVE_DEST           = "GVLC";
    const QString SET_IMAGE_RADIOMETRIC_JPEG    = "SIRJ";
    const QString GET_IMAGE_RADIOMETRIC_JPEG    = "GIRJ";
    const QString SET_IMAGE_RADIOMETRIC_TIFF    = "SIRT";
    const QString GET_IMAGE_RADIOMETRIC_TIFF    = "GIRT";
    const QString SET_IMAGE_SCREENSHOT_JPEG     = "SISS";
    const QString GET_IMAGE_SCREENSHOT_JPEG     = "GISS";
    const QString SET_IMAGE_SUPERRES            = "SISR";
    const QString GET_IMAGE_SUPERRES            = "GISR";
    const QString SET_IMAGE_VISIBLE_JPEG        = "SIVI";
    const QString GET_IMAGE_VISIBLE_JPEG        = "GIVI";
    const QString SET_VIDEO_IR                  = "SVTH";
    const QString GET_VIDEO_IR                  = "GVTH";
    const QString SET_VIDEO_IR_ENCODED          = "SVTE";
    const QString GET_VIDEO_IR_ENCODED          = "GVTE";
    const QString SET_VIDEO_VISIBLE             = "SVVI";
    const QString GET_VIDEO_VISIBLE             = "GVVI";
    // memory
    const QString GET_MEMORY_STATUS             = "GMST";
    const QString GET_MEMORY_SIZE               = "GMSI";
    const QString GET_MEMORY_FREE               = "GMFR";
    const QString GET_CAPTURED_IMAGES_NUMBER    = "GMCP";
    const QString GET_RECORDED_VIDEO_NUMBER     = "GTRC";
    const QString COPY_FROM_SSD                 = "CPST";
    // system
    const QString SET_CURRENT_DATETIME  = "SDTI";
    const QString GET_CURRENT_DATETIME  = "GDTI";
    const QString SET_UNITS             = "SUNI";
    const QString GET_UNITS             = "GUNI";
    const QString REBOOT                = "REBT";
    const QString SET_DEFAULT_SETTINGS  = "SDST";
    const QString GET_LANGUAGE          = "GLNG";
    const QString SET_LANGUAGE          = "SLNG";

    const QString NULL_ANS = "0\r\n\u0000";
    const QString NAN_COOLDOWN = "-1\r\n\u0000";
};

struct ErrorCodes
{
    const QString general_error = "ERR";

};

#endif // PROTOCOL_H
