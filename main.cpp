#include <QApplication>

#include "main_window.h"


int main(int argc, char* argv[])
{
    if(!(argc > 1)){
        return 1;
    }
    QApplication app{argc, argv};

    xml_viewer::Main_window main_window{argv[1]};
    main_window.show();

    return app.exec();
}
