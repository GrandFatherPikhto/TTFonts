#include "utils.h"

#include <QDebug>


bool GetSystemFontFilePath(const QString &fontName, QString &fontPath) {
    HKEY hKey;
    uint32_t pathSize = 0;
    TCHAR regPath[] = _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
    
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, regPath, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return false;
    }

    TCHAR valueName[256];
    TCHAR valueData[MAX_PATH];
    DWORD valueNameSize, valueDataSize, type;
    DWORD index = 0;
    bool found = false;

    while (true) {
        valueNameSize = sizeof(valueName) / sizeof(TCHAR);
        valueDataSize = sizeof(valueData);
        
        if (RegEnumValue(hKey, index, valueName, &valueNameSize, NULL,
                        &type, (LPBYTE)valueData, &valueDataSize) != ERROR_SUCCESS) {
            break;
        }

        TCHAR tchFontName[MAX_PATH] = {0};
        TCHAR tchFontPath[MAX_PATH] = {0};
        fontName.toWCharArray(tchFontName);
        // Проверяем, содержит ли имя шрифта искомое название
        if (_tcsstr(valueName, tchFontName) != NULL) {
            _stprintf_s(tchFontPath, _T("C:\\Windows\\Fonts\\%s"), valueData);
            fontPath = QString::fromStdWString(tchFontPath);
            found = true;
            break;
        }
        index++;
    }


    RegCloseKey(hKey);



    return found;
}

