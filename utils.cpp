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
        return Command_line_options{parser.positionalArguments().at(0),
                parser.isSet(render_html)};
    }
}
