#ifndef XML_VIEWER_HTML_DISPLAY_H
#define XML_VIEWER_HTML_DISPLAY_H

#include <QTextBrowser>
#include <QString>

#include "utils.h"

namespace xml_viewer
{

    class HTML_display: public QTextBrowser{

        public:

           HTML_display();
           bool load_file(const QString& file_name);

        private:

           std::unique_ptr<File_watcher> file_watcher_{};
    };
}

#endif
