#include <iostream>

#include "html_display.h"

namespace xml_viewer
{

    HTML_display::HTML_display()
    {
        file_watcher_ = make_unique<File_watcher>();
        QObject::connect(
                file_watcher_->get_fs_watcher(),
                &QFileSystemWatcher::fileChanged,
                this, &HTML_display::load_file);

        QFont font(this->font().family(), 25);
        setFont(font);
        // setLineWrapMode(QTextEdit::NoWrap);
        setStyleSheet(
                "border-style: 'solid'; border-color: 'black'; border: 2;");
    }

    bool HTML_display::load_file(const QString& file_name)
    {
        std::cout << "load file: " << file_name.toStdString() << std::endl;
        clear();
        file_watcher_->set_file(file_name);
        setSource(file_name);
        // std::cout << toHtml().toStdString() << std::endl;
        return true;
    }

}
