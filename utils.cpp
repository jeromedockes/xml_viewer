#include <QDir>
#include <QFileInfo>

#include "utils.h"

namespace xml_viewer
{

    Command_line_options get_command_line_options(const QApplication& app)
    {
        QCommandLineParser parser{};
        parser.addPositionalArgument("file", "file to display");
        QCommandLineOption render_html("html", "render HTML");
        parser.addOption(render_html);
        parser.process(app);
        if(!parser.positionalArguments().size()){
            return Command_line_options{QString{}, 0};
        }
        return Command_line_options{parser.positionalArguments().at(0),
                parser.isSet(render_html)};
    }

    File_watcher::File_watcher()
        : file_name_{} {
            fs_watcher_ = make_unique<QFileSystemWatcher>();
            QObject::connect(
                    fs_watcher_.get(), &QFileSystemWatcher::directoryChanged,
                    this, &File_watcher::watch_again);
        }

    void File_watcher::watch_again()
    {
        fs_watcher_->addPath(file_name_);
    }

    void File_watcher::set_file(const QString& file_name)
    {
        QFileInfo file_info{file_name};
        file_name_ = file_name;
        auto directory = file_info.absoluteDir().absolutePath();
        fs_watcher_->removePaths(fs_watcher_->files());
        fs_watcher_->removePaths(fs_watcher_->directories());
        fs_watcher_->addPaths(QStringList{file_name, directory});
    }

}
