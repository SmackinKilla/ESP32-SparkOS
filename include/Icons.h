#pragma once
#include <stdint.h>

enum WeatherIcon {
    ICON_CLEAR, ICON_SCATTERED_CLOUDS, ICON_RAIN, ICON_SNOW, ICON_MIST, ICON_UNKNOWN
};

inline WeatherIcon getIconFromCode(int code) {
    // WMO (Open-Meteo)
    if (code <= 1) return ICON_CLEAR;
    if (code <= 3) return ICON_SCATTERED_CLOUDS;
    if (code == 45 || code == 48) return ICON_MIST;
    if ((code >= 51 && code <= 67) || (code >= 80 && code <= 82) || code >= 95) return ICON_RAIN;
    if ((code >= 71 && code <= 77) || code == 85 || code == 86) return ICON_SNOW;
    return ICON_UNKNOWN;
}

inline const char* getDescriptionFromCode(int code) {
    if (code <= 1) return "Clear";
    if (code <= 3) return "Cloudy";
    if (code == 45 || code == 48) return "Mist";
    if ((code >= 51 && code <= 67) || (code >= 80 && code <= 82)) return "Rain";
    if ((code >= 71 && code <= 77) || code == 85 || code == 86) return "Snow";   
    return "Unknown"; 
}

inline const char* iconPath(WeatherIcon icon) {
    switch (icon) {
        case ICON_CLEAR:            return "/img/Clear.bmp";
        case ICON_SCATTERED_CLOUDS: return "/img/Clouds.bmp";
        case ICON_RAIN:             return "/img/Rain.bmp";
        case ICON_SNOW:             return "/img/Snow.bmp";
        case ICON_MIST:             return "/img/Mist.bmp";
        default:                    return "/img/Clear.bmp";
    }
}