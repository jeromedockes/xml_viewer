#ifndef XML_VIEWER_UTILS_H
#define XML_VIEWER_UTILS_H

#include <memory>

#include <QApplication>
#include <Qt>
#include <QCommandLineParser>
#include <QFileSystemWatcher>


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

    class File_watcher: public QObject {

        Q_OBJECT

        public:

            File_watcher();

            void set_file(const QString& file_name);
            const QFileSystemWatcher* get_fs_watcher() const
            {
                return fs_watcher_.get();
            }

        private:

            QString file_name_{};
            std::unique_ptr<QFileSystemWatcher> fs_watcher_{};
            void watch_again();
    };
}

#endif
