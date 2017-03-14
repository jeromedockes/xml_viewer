#ifndef XML_VIEWER_UTILS_H
#define XML_VIEWER_UTILS_H

#include <memory>

#include <QApplication>
#include <Qt>
#include <QCommandLineParser>


namespace xml_viewer
{

    template<typename T, typename ...Args>
        std::unique_ptr<T> make_unique(Args&& ...args)
        {
           return std::unique_ptr<T>(new T(std::forward<Args>(args)... ));
        }

    enum User_roles {first_user_role = Qt::UserRole, is_element};


    struct Command_line_options{

        QString file_name;
        bool render_html;

    };

    Command_line_options get_command_line_options(const QApplication& app);

}

#endif
