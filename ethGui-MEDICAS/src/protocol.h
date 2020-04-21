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
    const QString force_calibration = "FORC";
    const QString set_level_of_acception = "SLOA";
    const QString get_level_of_acception = "GLOA";
    const QString set_blackbody_roi = "SBBR";
    const QString get_blackbody_roi = "GBBR";
    // capture
    const QString capture_image = "CPTR";
    const QString check_capture = "ICPT";
    const QString start_recording = "RCRS";
    const QString finish_recording = "RCRF";
    const QString check_is_recording = "IRCR";
    // palette
    const QString set_palette_by_name = "SPTE";
    const QString set_palette_by_index = "SPTI";
    const QString get_palette_name_index = "GPTE";
    const QString get_palette_values = "GPLV";
    const QString get_all_palettes = "GPTL";
    // measure
    const QString set_alarm_mode = "SALM";
    const QString get_alarm_mode = "GALM";
    const QString set_thresholds = "SALV";
    const QString get_thresholds = "GALV";
    const QString set_show_cross_max = "SSCM";
    const QString get_show_cross_max = "GSCM";
    const QString get_cross_max = "GCMA";
    const QString set_show_cross_center = "SSCC";
    const QString get_show_cross_center = "GSCC";
    const QString get_cross_center = "GCCE";
    const QString set_show_blackbody_roi = "SSBR";
    const QString get_show_blackbody_roi = "GSBR";
    const QString get_blackbody_status = "GBBS";
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
    // alarms graphic settings
    const QString set_thresholds_colors = "SALC";
    const QString get_thresholds_colors = "GALC";
    const QString set_thresholds_opacity = "SAOP";
    const QString get_thresholds_opacity = "GAOP";
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

};

#endif // PROTOCOL_H
