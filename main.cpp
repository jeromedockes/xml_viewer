#include <iostream>

#include <QApplication>
#include <QPixmap>
#include <QIcon>

#include "utils.h"
#include "main_window.h"


int main(int argc, char* argv[])
{
    QApplication app{argc, argv};
    QPixmap pixmap{":/images/x_icon.png"};
    QIcon icon{pixmap};
    app.setWindowIcon(icon);

    auto cli_options = xml_viewer::get_command_line_options(app);
    if(!cli_options.file_name.size()){
        std::cout << "Please give a file name" << std::endl;
        return 1;
    }
    xml_viewer::Main_window main_window(cli_options.file_name,
            cli_options.render_html);
    main_window.show();

    return app.exec();
}
