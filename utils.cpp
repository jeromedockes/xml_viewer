#include <QDir>
#include <QFileInfo>
#include <QShortcut>
#include <QTextEdit>
#include <QScrollBar>
#include <QKeyEvent>

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

    std::map<Key_event_summary, int> Editor_mappings::mappings_{
        {{Qt::Key_H, Qt::NoModifier}, Qt::Key_Left},
            {{Qt::Key_J, Qt::NoModifier}, Qt::Key_Down},
            {{Qt::Key_K, Qt::NoModifier}, Qt::Key_Up},
            {{Qt::Key_L, Qt::NoModifier}, Qt::Key_Right},
            {{Qt::Key_D, Qt::ControlModifier}, Qt::Key_PageDown},
            {{Qt::Key_U, Qt::ControlModifier}, Qt::Key_PageUp},
            {{Qt::Key_G, Qt::ShiftModifier}, Qt::Key_End},
            {{Qt::Key_G, Qt::NoModifier}, Qt::Key_Home},
            {{Qt::Key_N, Qt::ControlModifier}, Qt::Key_Down},
            {{Qt::Key_P, Qt::ControlModifier}, Qt::Key_Up},
            {{Qt::Key_F, Qt::ControlModifier}, Qt::Key_Right},
            {{Qt::Key_B, Qt::ControlModifier}, Qt::Key_Left}};

    bool Editor_mappings::eventFilter(QObject* object, QEvent* event)
    {
        if(event->type() != QEvent::KeyPress){
            return QObject::eventFilter(object, event);
        }
        auto key_press = static_cast<QKeyEvent*>(event);
        auto kb_mapping = mappings_.find(std::make_pair(
                    key_press->key(), key_press->modifiers()));
        if(kb_mapping == mappings_.end()){
            return QObject::eventFilter(object, event);
        }
        auto new_event = make_unique<QKeyEvent>(
                QEvent::KeyPress, kb_mapping->second, Qt::NoModifier);
        QApplication::postEvent(object, new_event.release());
        return true;
    }

}
