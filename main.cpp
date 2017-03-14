#include <QApplication>

#include "utils.h"
#include "main_window.h"


int main(int argc, char* argv[])
{
    QApplication app{argc, argv};
    auto cli_options = xml_viewer::get_command_line_options(app);
    if(!cli_options.file_name.size()){
        return 1;
    }
    xml_viewer::Main_window main_window(cli_options.file_name,
            cli_options.render_html);
    main_window.show();

    return app.exec();
}
