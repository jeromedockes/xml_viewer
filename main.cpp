#include <QApplication>

#include "main_window.h"


int main(int argc, char* argv[])
{
    QApplication app{argc, argv};

    xml_viewer::Main_window main_window{};
    main_window.show();

    return app.exec();
}
