#include "unifiedicon.h"

QString UnifiedIcon::iconColor = "#000";

void UnifiedIcon::setStyle(QString style)
{
    if(style == "light")
        iconColor = "#000";
    else if(style == "dark")
        iconColor = "#FFF";
}

QString UnifiedIcon::getStyle()
{
    if(iconColor == "#000")
        return QString("light");
    else
        return QString("dark");
}

QIcon UnifiedIcon::getIcon(QString name)
{
//    QString platform;
//    #if defined(Q_OS_MACOS)
//        platform = "macos";
//    #elif defined(Q_OS_WIN)
//        platform = "windows";
//    #elif defined(Q_OS_LINUX)
//        platform = "linux";
//    #else
//        platform = "unknown";
//    #endif

    // Icon from https://material.io/resources/icons/
    if(name == "cqq")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M 25 2 C 12.297 2 2 12.297 2 25 C 2 37.703 12.297 48 25 48 C 37.703 48 48 37.703 48 25 C 48 12.297 37.703 2 25 2 z M 25.019531 15 C 30.218531 15 33.470703 18.725688 33.470703 24.679688 C 33.470703 28.354688 32.291406 31.235094 30.191406 32.746094 L 32.535156 36 L 29.949219 36 L 28.322266 33.785156 C 27.336266 34.156156 26.210531 34.347656 25.019531 34.347656 C 19.756531 34.347656 16.529297 30.672688 16.529297 24.679688 C 16.529297 18.712688 19.794531 15 25.019531 15 z M 25.019531 17.126953 C 21.254531 17.126953 18.898437 20.033641 18.898438 24.681641 C 18.898438 29.342641 21.24125 32.222656 25.03125 32.222656 C 25.71025 32.222656 26.363906 32.133406 27.003906 31.941406 L 24.556641 28.625 L 27.169922 28.625 L 28.845703 30.853516 C 30.292703 29.701516 31.099609 27.498641 31.099609 24.681641 C 31.101609 20.045641 28.758531 17.126953 25.019531 17.126953 z' fill='"+iconColor+"'/></svg>");
    else if(name == "chat")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M20 2H4c-1.1 0-1.99.9-1.99 2L2 22l4-4h14c1.1 0 2-.9 2-2V4c0-1.1-.9-2-2-2zM6 9h12v2H6V9zm8 5H6v-2h8v2zm4-6H6V6h12v2z' fill='"+iconColor+"'/><path d='M0 0h24v24H0z' fill='none'/></svg>");
    else if(name == "contact")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M19 3h-1V1h-2v2H8V1H6v2H5c-1.11 0-2 .9-2 2v14c0 1.1.89 2 2 2h14c1.1 0 2-.9 2-2V5c0-1.1-.9-2-2-2zm-7 3c1.66 0 3 1.34 3 3s-1.34 3-3 3-3-1.34-3-3 1.34-3 3-3zm6 12H6v-1c0-2 4-3.1 6-3.1s6 1.1 6 3.1v1z' fill='"+iconColor+"'/><path d='M0 0h24v24H0z' fill='none'/></svg>");
    else if(name == "folder")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M10 4H4c-1.1 0-1.99.9-1.99 2L2 18c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V8c0-1.1-.9-2-2-2h-8l-2-2z' fill='"+iconColor+"'/><path d='M0 0h24v24H0z' fill='none'/></svg>");
    else if(name == "view")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M12 4.5C7 4.5 2.73 7.61 1 12c1.73 4.39 6 7.5 11 7.5s9.27-3.11 11-7.5c-1.73-4.39-6-7.5-11-7.5zM12 17c-2.76 0-5-2.24-5-5s2.24-5 5-5 5 2.24 5 5-2.24 5-5 5zm0-8c-1.66 0-3 1.34-3 3s1.34 3 3 3 3-1.34 3-3-1.34-3-3-3z' fill='"+iconColor+"'/></svg>");
    else if(name == "setup")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M942.08 588.8V492.16L828.16 448l-28.16-69.76 54.4-110.08-64-69.76-112 51.2-72.96-30.08-39.68-116.48H469.12l-42.88 115.2-71.04 28.16L245.12 192 175.36 256l51.2 112-30.08 71.68-116.48 39.68L78.08 576 192 618.88l28.16 71.04-53.12 110.08 67.2 69.76 112-51.2 70.4 30.08 39.04 116.48h97.28l42.88-115.2 70.4-28.16 110.72 55.04 69.12-67.84-51.2-111.36 30.72-70.4zM512 720a186.24 186.24 0 1 1 186.24-185.6A185.6 185.6 0 0 1 512 720z' fill='"+iconColor+"'></path></svg>");
    else if(name == "help")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm1 17h-2v-2h2v2zm2.07-7.75l-.9.92C13.45 12.9 13 13.5 13 15h-2v-.5c0-1.1.45-2.1 1.17-2.83l1.24-1.26c.37-.36.59-.86.59-1.41 0-1.1-.9-2-2-2s-2 .9-2 2H8c0-2.21 1.79-4 4-4s4 1.79 4 4c0 .88-.36 1.68-.93 2.25z' fill='"+iconColor+"'/></svg>");
    else if(name == "open")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M19 19H5V5h7V3H5c-1.11 0-2 .9-2 2v14c0 1.1.89 2 2 2h14c1.1 0 2-.9 2-2v-7h-2v7zM14 3v2h3.59l-9.83 9.83 1.41 1.41L19 6.41V10h2V3h-7z' fill='"+iconColor+"'/></svg>");
    else if(name == "clear")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M15 16h4v2h-4zm0-8h7v2h-7zm0 4h6v2h-6zM3 18c0 1.1.9 2 2 2h6c1.1 0 2-.9 2-2V8H3v10zM14 5h-3l-1-1H6L5 5H2v2h12z' fill='"+iconColor+"'/><path fill='none' d='M0 0h24v24H0z'/></svg>");
    else if(name == "delete")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M22 3H7c-.69 0-1.23.35-1.59.88L0 12l5.41 8.11c.36.53.9.89 1.59.89h15c1.1 0 2-.9 2-2V5c0-1.1-.9-2-2-2zm-3 12.59L17.59 17 14 13.41 10.41 17 9 15.59 12.59 12 9 8.41 10.41 7 14 10.59 17.59 7 19 8.41 15.41 12 19 15.59z' fill='"+iconColor+"'/></svg>");
    else if(name == "layout")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M5 15H3v4c0 1.1.9 2 2 2h4v-2H5v-4zM5 5h4V3H5c-1.1 0-2 .9-2 2v4h2V5zm14-2h-4v2h4v4h2V5c0-1.1-.9-2-2-2zm0 16h-4v2h4c1.1 0 2-.9 2-2v-4h-2v4zM12 9c-1.66 0-3 1.34-3 3s1.34 3 3 3 3-1.34 3-3-1.34-3-3-3z' fill='"+iconColor+"'/></svg>");
    else if(name == "refresh")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M17.65 6.35C16.2 4.9 14.21 4 12 4c-4.42 0-7.99 3.58-7.99 8s3.57 8 7.99 8c3.73 0 6.84-2.55 7.73-6h-2.08c-.82 2.33-3.04 4-5.65 4-3.31 0-6-2.69-6-6s2.69-6 6-6c1.66 0 3.14.69 4.22 1.78L13 11h7V4l-2.35 2.35z' fill='"+iconColor+"'/><path d='M0 0h24v24H0z' fill='none'/></svg>");
    else if(name == "notify")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M12 22c1.1 0 2-.9 2-2h-4c0 1.1.89 2 2 2zm6-6v-5c0-3.07-1.64-5.64-4.5-6.32V4c0-.83-.67-1.5-1.5-1.5s-1.5.67-1.5 1.5v.68C7.63 5.36 6 7.92 6 11v5l-2 2v1h16v-1l-2-2z' fill='"+iconColor+"'/></svg>");
    else if(name == "download")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M19.35 10.04C18.67 6.59 15.64 4 12 4 9.11 4 6.6 5.64 5.35 8.04 2.34 8.36 0 10.91 0 14c0 3.31 2.69 6 6 6h13c2.76 0 5-2.24 5-5 0-2.64-2.05-4.78-4.65-4.96zM17 13l-5 5-5-5h3V9h4v4h3z' fill='"+iconColor+"'/></svg>");
    else if(name == "build")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path clip-rule='evenodd' fill='none' d='M0 0h24v24H0z'/><path d='M22.7 19l-9.1-9.1c.9-2.3.4-5-1.5-6.9-2-2-5-2.4-7.4-1.3L9 6 6 9 1.6 4.7C.4 7.1.9 10.1 2.9 12.1c1.9 1.9 4.6 2.4 6.9 1.5l9.1 9.1c.4.4 1 .4 1.4 0l2.3-2.3c.5-.4.5-1.1.1-1.4z' fill='"+iconColor+"'/></svg>");
    else if(name == "about")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm1 15h-2v-6h2v6zm0-8h-2V7h2v2z' fill='"+iconColor+"'/></svg>");
    else if(name == "exit")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M10.09 15.59L11.5 17l5-5-5-5-1.41 1.41L12.67 11H3v2h9.67l-2.58 2.59zM19 3H5c-1.11 0-2 .9-2 2v4h2V5h14v14H5v-4H3v4c0 1.1.89 2 2 2h14c1.1 0 2-.9 2-2V5c0-1.1-.9-2-2-2z' fill='"+iconColor+"'/></svg>");
    else if(name == "person")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M12 12c2.21 0 4-1.79 4-4s-1.79-4-4-4-4 1.79-4 4 1.79 4 4 4zm0 2c-2.67 0-8 1.34-8 4v2h16v-2c0-2.66-5.33-4-8-4z' fill='"+iconColor+"'/><path d='M0 0h24v24H0z' fill='none'/></svg>");
    else if(name == "group")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M16 11c1.66 0 2.99-1.34 2.99-3S17.66 5 16 5c-1.66 0-3 1.34-3 3s1.34 3 3 3zm-8 0c1.66 0 2.99-1.34 2.99-3S9.66 5 8 5C6.34 5 5 6.34 5 8s1.34 3 3 3zm0 2c-2.33 0-7 1.17-7 3.5V19h14v-2.5c0-2.33-4.67-3.5-7-3.5zm8 0c-.29 0-.62.02-.97.05 1.16.84 1.97 1.97 1.97 3.45V19h6v-2.5c0-2.33-4.67-3.5-7-3.5z' fill='"+iconColor+"'/></svg>");
    else if(name == "down")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M7.41 8.59L12 13.17l4.59-4.58L18 10l-6 6-6-6 1.41-1.41z' fill='"+iconColor+"'/><path fill='none' d='M0 0h24v24H0V0z'/></svg>");
    else if(name == "face")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M11.99 2C6.47 2 2 6.48 2 12s4.47 10 9.99 10C17.52 22 22 17.52 22 12S17.52 2 11.99 2zM12 20c-4.42 0-8-3.58-8-8s3.58-8 8-8 8 3.58 8 8-3.58 8-8 8zm3.5-9c.83 0 1.5-.67 1.5-1.5S16.33 8 15.5 8 14 8.67 14 9.5s.67 1.5 1.5 1.5zm-7 0c.83 0 1.5-.67 1.5-1.5S9.33 8 8.5 8 7 8.67 7 9.5 7.67 11 8.5 11zm3.5 6.5c2.33 0 4.31-1.46 5.11-3.5H6.89c.8 2.04 2.78 3.5 5.11 3.5z' fill='"+iconColor+"'/></svg>");
    else if(name == "check")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm-2 15l-5-5 1.41-1.41L10 14.17l7.59-7.59L19 8l-9 9z' fill='"+iconColor+"'/></svg>");
    else if(name == "close")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M12 2C6.47 2 2 6.47 2 12s4.47 10 10 10 10-4.47 10-10S17.53 2 12 2zm5 13.59L15.59 17 12 13.41 8.41 17 7 15.59 10.59 12 7 8.41 8.41 7 12 10.59 15.59 7 17 8.41 13.41 12 17 15.59z' fill='"+iconColor+"'/><path d='M0 0h24v24H0z' fill='none'/></svg>");
    else if(name == "warn")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M1 21h22L12 2 1 21zm12-3h-2v-2h2v2zm0-4h-2v-4h2v4z' fill='"+iconColor+"'/></svg>");
    else if(name == "photo")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M0 0h24v24H0z' fill='none'/><path d='M21 19V5c0-1.1-.9-2-2-2H5c-1.1 0-2 .9-2 2v14c0 1.1.9 2 2 2h14c1.1 0 2-.9 2-2zM8.5 13.5l2.5 3.01L14.5 12l4.5 6H5l3.5-4.5z' fill='"+iconColor+"'/></svg>");
    else if(name == "camera")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><circle cx='12' cy='12' r='3.2'/><path d='M9 2L7.17 4H4c-1.1 0-2 .9-2 2v12c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V6c0-1.1-.9-2-2-2h-3.17L15 2H9zm3 15c-2.76 0-5-2.24-5-5s2.24-5 5-5 5 2.24 5 5-2.24 5-5 5z' fill='"+iconColor+"'/><path d='M0 0h24v24H0z' fill='none'/></svg>");
    else if(name == "send")
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M2.01 21L23 12 2.01 3 2 10l15 2-15 2z' fill='"+iconColor+"'/><path d='M0 0h24v24H0z' fill='none'/></svg>");
    else
        return generateIcon("<svg xmlns='http://www.w3.org/2000/svg'><path d='M6 13c-.55 0-1 .45-1 1s.45 1 1 1 1-.45 1-1-.45-1-1-1zm0 4c-.55 0-1 .45-1 1s.45 1 1 1 1-.45 1-1-.45-1-1-1zm0-8c-.55 0-1 .45-1 1s.45 1 1 1 1-.45 1-1-.45-1-1-1zm-3 .5c-.28 0-.5.22-.5.5s.22.5.5.5.5-.22.5-.5-.22-.5-.5-.5zM6 5c-.55 0-1 .45-1 1s.45 1 1 1 1-.45 1-1-.45-1-1-1zm15 5.5c.28 0 .5-.22.5-.5s-.22-.5-.5-.5-.5.22-.5.5.22.5.5.5zM14 7c.55 0 1-.45 1-1s-.45-1-1-1-1 .45-1 1 .45 1 1 1zm0-3.5c.28 0 .5-.22.5-.5s-.22-.5-.5-.5-.5.22-.5.5.22.5.5.5zm-11 10c-.28 0-.5.22-.5.5s.22.5.5.5.5-.22.5-.5-.22-.5-.5-.5zm7 7c-.28 0-.5.22-.5.5s.22.5.5.5.5-.22.5-.5-.22-.5-.5-.5zm0-17c.28 0 .5-.22.5-.5s-.22-.5-.5-.5-.5.22-.5.5.22.5.5.5zM10 7c.55 0 1-.45 1-1s-.45-1-1-1-1 .45-1 1 .45 1 1 1zm0 5.5c-.83 0-1.5.67-1.5 1.5s.67 1.5 1.5 1.5 1.5-.67 1.5-1.5-.67-1.5-1.5-1.5zm8 .5c-.55 0-1 .45-1 1s.45 1 1 1 1-.45 1-1-.45-1-1-1zm0 4c-.55 0-1 .45-1 1s.45 1 1 1 1-.45 1-1-.45-1-1-1zm0-8c-.55 0-1 .45-1 1s.45 1 1 1 1-.45 1-1-.45-1-1-1zm0-4c-.55 0-1 .45-1 1s.45 1 1 1 1-.45 1-1-.45-1-1-1zm3 8.5c-.28 0-.5.22-.5.5s.22.5.5.5.5-.22.5-.5-.22-.5-.5-.5zM14 17c-.55 0-1 .45-1 1s.45 1 1 1 1-.45 1-1-.45-1-1-1zm0 3.5c-.28 0-.5.22-.5.5s.22.5.5.5.5-.22.5-.5-.22-.5-.5-.5zm-4-12c-.83 0-1.5.67-1.5 1.5s.67 1.5 1.5 1.5 1.5-.67 1.5-1.5-.67-1.5-1.5-1.5zm0 8.5c-.55 0-1 .45-1 1s.45 1 1 1 1-.45 1-1-.45-1-1-1zm4-4.5c-.83 0-1.5.67-1.5 1.5s.67 1.5 1.5 1.5 1.5-.67 1.5-1.5-.67-1.5-1.5-1.5zm0-4c-.83 0-1.5.67-1.5 1.5s.67 1.5 1.5 1.5 1.5-.67 1.5-1.5-.67-1.5-1.5-1.5z' fill='"+iconColor+"'/><path d='M0 0h24v24H0z' fill='none'/></svg>");
}

QIcon UnifiedIcon::generateIcon(QString xml)
{
    QSvgRenderer svg(xml.toUtf8());
    QImage img(128, 128, QImage::Format_ARGB32);
    img.fill(0x00ffffff);
    QPainter painter(&img);
    svg.render(&painter);
    return QIcon(QPixmap::fromImage(img));
}
