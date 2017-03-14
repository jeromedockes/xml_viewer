#ifndef XML_VIEWER_MAIN_WINDOW_H
#define XML_VIEWER_MAIN_WINDOW_H


#include <QMainWindow>

namespace xml_viewer
{

    class Main_window: public QMainWindow{

        Q_OBJECT

        public:

            Main_window(const QString& file_name, bool render_html);

            QSize sizeHint() const override
            {
                return QSize(5000, 5000);
            }
    };

}

#endif
