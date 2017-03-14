#include <QFileInfo>
#include <QDir>

#include "utils.h"
#include "html_display.h"

namespace xml_viewer
{

    HTML_display::HTML_display()
    {
        QFont font(this->font().family(), 25);
        setFont(font);

    }

    bool HTML_display::load_file(const QString& file_name)
    {
        clear();
        set_doc_file(file_name);
        setSource(file_name);
        return true;
    }

    void HTML_display::watch_again()
    {
        if(file_watcher_ == nullptr){
            return;
        }
        file_watcher_->addPath(doc_file_);
    }

    void HTML_display::set_doc_file(const QString& new_doc_file)
    {
        QFileInfo file_info{new_doc_file};
        doc_file_ = new_doc_file;
        auto directory = file_info.absoluteDir().absolutePath();
        if(file_watcher_ == nullptr){
            file_watcher_ = make_unique<QFileSystemWatcher>(
                    QStringList{new_doc_file, directory});
            QObject::connect(
                    file_watcher_.get(), &QFileSystemWatcher::fileChanged,
                    this, &HTML_display::reload);
            QObject::connect(
                    file_watcher_.get(), &QFileSystemWatcher::directoryChanged,
                    this, &HTML_display::watch_again);
            return;
        }
        file_watcher_->removePaths(file_watcher_->files());
        file_watcher_->removePaths(file_watcher_->directories());
        file_watcher_->addPaths(QStringList{new_doc_file, directory});
    }
}
