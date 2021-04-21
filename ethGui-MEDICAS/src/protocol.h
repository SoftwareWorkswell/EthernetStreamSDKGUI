#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>

struct Protocol
{
    // functions to create messages, adds formatting(spaces, delimeter)
    QString prepareMessage(const QString & command);
    QString prepareMessage(const QString & command, const QString & args);
    // default delimiter is '\n', needs to be used when setting delimiter or checking connection
    QString prepareMessageWithDefDelim(const QString & command);
    QString prepareMessageWithDefDelim(const QString & command, const QString & args);

    QString delimiter = "";
    // basic
    const QString set_delimiter = "SDLM";
    const QString get_delimiter = "GDLM";
    const QString check_connection = "HIWS";
    const QString get_serial_number = "GSRN";
    const QString get_article_number = "GATN";
    const QString get_fw_version = "GFWV";
    const QString get_cpu_temp = "GTCU";
    const QString get_camera_core_temp = "GTIC";
    const QString get_device_temp = "GTIN";
    const QString get_fan_power = "GFPW";
    // activation
    const QString activate = "ACTV";
    const QString is_activated = "IACT";
    // ethernet mode
    const QString set_eth_mode = "SETH";
    const QString get_eth_mode = "GETH";
    // range
    const QString set_range = "SRMM";
    const QString get_range = "GRMM";
    // functions
    const QString set_main_camera = "SMCA";
    const QString get_main_camera = "GMCA";
    const QString set_temperature_mode = "STOF";
    const QString get_temperature_mode = "GTOF";
    // capture
    const QString capture_image = "CPTR";
    const QString check_capture = "ICPT";
    const QString start_recording = "RCRS";
    const QString finish_recording = "RCRF";
    const QString check_is_recording = "IRCR";
    const QString set_length_rotation_recording = "SRLN";
    const QString get_length_rotation_recording = "GRLN";
    // palette
    const QString set_palette_by_name = "SPTE";
    const QString set_palette_by_index = "SPTI";
    const QString get_palette_name_index = "GPTE";
    const QString get_palette_values = "GPLV";
    const QString get_all_palettes = "GPTL";
    // alarms
    const QString set_alarm_mode = "SALM";
    const QString get_alarm_mode = "GALM";
    const QString set_thresholds = "SALV";
    const QString get_thresholds = "GALV";
    const QString get_alarm_only_after_trigger = "GSAT";
    const QString set_alarm_only_after_trigger = "SSAT";
    const QString get_alarm_flash = "GLED";
    const QString set_alarm_flash = "SLED";
    const QString get_alarm_buzz= "GBUZ";
    const QString set_alarm_buzz = "SBUZ";
    // alarms graphic settings
    const QString set_thresholds_colors = "SALC";
    const QString get_thresholds_colors = "GALC";
    const QString set_thresholds_opacity = "SAOP";
    const QString get_thresholds_opacity = "GAOP";
    // measure - display settings
    const QString set_show_cross_max = "SSCM";
    const QString get_show_cross_max = "GSCM";
    const QString set_show_cross_user = "SSCU";
    const QString get_show_cross_user = "GSCU";
    const QString set_show_blackbody_roi = "SSBR";
    const QString get_show_blackbody_roi = "GSBR";
    const QString set_show_user_roi = "SSHR";
    const QString get_show_user_roi = "GSHR";
    // measure - details
    const QString get_cross_max = "GCMA";
    const QString set_cross_user = "SUCR";
    const QString get_cross_user = "GUCR";
    const QString get_show_user_roi_name = "GRSN";
    const QString set_show_user_roi_name = "SRSN";
    const QString set_blackbody_roi = "SBBR";
    const QString get_blackbody_roi = "GBBR";
    const QString get_blackbody_status = "GBBS";
    const QString get_blackbody_size = "GBBD";
    const QString get_blackbody_mask_size = "GBBM";
    const QString set_blackbody_area = "SBBT";
    const QString set_blackbody_detection = "SBDE";
    const QString get_blackbody_detection = "GBDE";
    const QString set_blackbody_scale = "SBBF";
    const QString get_blackbody_scale = "GBBF";
    const QString set_user_roi = "SURO";
    const QString get_user_roi = "GURO";
    const QString set_user_roi_name = "SRNM";
    const QString get_user_roi_name = "GRNM";
    const QString get_thermal_extremes = "GTEX";
    // DIO settings
    const QString get_dio_connected = "IDIO";
    const QString set_trigger_mode = "STGM";
    const QString get_trigger_mode = "GTGM";
    const QString set_picture_on_trigger = "STGS";
    const QString get_picture_on_trigger = "GTGS";
    const QString set_min_pulse_width = "STGW";
    const QString get_min_pulse_width = "GTGW";
    const QString set_dio_signal = "STGP";
    const QString get_dio_signal = "GTGP";
    const QString set_dio_logic = "SRLL";
    const QString get_dio_logic = "GRLL";
    const QString set_dio_alarm_level = "SRLE";
    const QString get_dio_alarm_level = "GRLE";
    const QString set_trigger_dalay = "STGD";
    const QString get_trigger_delay = "GTGD";
    const QString set_min_detection_time = "SRLD";
    const QString get_min_detection_time = "GRLD";
    const QString set_min_alarm_time = "SRLA";
    const QString get_min_alarm_time = "GRLA";
    const QString set_temperature_hysteris = "SRLT";
    const QString get_temperature_hysteris = "GRLT";
    const QString set_gate_open_time = "SRLG";
    const QString get_gate_open_time = "GRLG";
    const QString set_show_dio_status = "SSDS";
    const QString get_show_dio_status = "GSDS";
    const QString restart_dio = "RDIO";
    // thermal camera settings
    const QString get_seconds_to_thermal_shutter = "GTSN";
    const QString get_seconds_from_thermal_shutter = "GTSL";
    const QString force_calibration = "FORC";
    // advanced medicas params for visible camera
    const QString set_gamma = "SGAM";
    const QString get_gamma = "GGAM";
    const QString set_white_balance = "SWBA";
    const QString get_white_balance = "GWBA";
    const QString set_backlight_compensation = "SBCO";
    const QString get_backlight_compensation = "GBCO";
    const QString set_wide_range_mode = "SWDR";
    const QString get_wide_range_mode = "GWDR";
    const QString set_noise_reduction2d = "SNR2";
    const QString get_noise_reduction2d = "GNR2";
    const QString set_noise_reduction3d = "SNR3";
    const QString get_noise_reduction3d = "GNR3";
    const QString set_color_gain = "SCLG";
    const QString get_color_gain = "GCLG";
    const QString set_gain = "SGAI";
    const QString get_gain = "GGAI";
    const QString set_iris = "SIRI";
    const QString get_iris = "GIRI";
    const QString set_exposure_mode = "SEXM";
    const QString get_exposure_mode = "GEXM";
    const QString set_exposure_compensation = "SEXC";
    const QString get_exposure_compensation = "GEXC";
    const QString set_vis_shutter = "SSHU";
    const QString get_vis_shutter = "GSHU";
    const QString set_visible_default = "SVID";
    const QString set_visible_resolution = "SVRE";
    const QString get_visible_resolution = "GVRE";
    // image and video setting
    const QString set_image_radiometric_jpeg = "SIRJ";
    const QString get_image_radiometric_jpeg = "GIRJ";
    const QString set_image_visible_jpeg = "SIVI";
    const QString get_image_visible_jpeg = "GIVI";
    const QString set_save_dest = "SILC";
    const QString get_save_dest = "GILC";
    const QString set_video_ir_encoded = "SVTE";
    const QString get_video_ir_encoded = "GVTE";
    const QString set_video_visible = "SVVI";
    const QString get_video_visible = "GVVI";
    // memory
    const QString format_memory = "";
    const QString copy_logs = "";
    const QString get_memory_status = "GMST";
    const QString get_memory_size = "GMSI";
    const QString get_memory_free = "GMFR";
    // system
    const QString set_current_datetime = "SDTI";
    const QString get_current_datetime = "GDTI";
    const QString set_units = "SUNI";
    const QString get_units = "GUNI";
    const QString reboot = "REBT";
    const QString set_default_settings = "SDST";
    const QString get_language = "GLNG";
    const QString set_language = "SLNG";
    // users
    const QString login = "LGIN";
    const QString logout = "LGOT";
    const QString change_user_password = "SUPW";
    const QString get_current_user = "GUSE";
    const QString set_user_settings = "SUST";
    const QString get_all_user_settings = "GUST";
    // head
    const QString get_head_values = "GHVL";
    const QString set_special_pheripheral_output = "SPCL";
};

struct ErrorCodes
{
    const QString general_error = "ERR";
    const QString blackbody_error = "CAN_NOT_SET_WHEN_BB_NOK";
    const QString blackbody_heating_up = "HEATING_UP";

};

#endif // PROTOCOL_H
