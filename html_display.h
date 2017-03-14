#ifndef XML_VIEWER_HTML_DISPLAY_H
#define XML_VIEWER_HTML_DISPLAY_H

#include <QTextBrowser>
#include <QString>
#include <QFileSystemWatcher>

namespace xml_viewer
{

    class HTML_display: public QTextBrowser{

        public:

           HTML_display();
           bool load_file(const QString& file_name);

       public slots:

           void watch_again();

        private:

            void set_doc_file(const QString& new_doc_file);

            QString doc_file_{};
            std::unique_ptr<QFileSystemWatcher> file_watcher_{};
    };
}

#endif
