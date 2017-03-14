#include <QSplitter>

#include "utils.h"
#include "xml_tree.h"
#include "main_window.h"
#include "xml_node_display.h"

namespace xml_viewer
{

    Main_window::Main_window(const char* file_name)
        :QMainWindow{} {

            auto central_splitter = make_unique<QSplitter>(Qt::Vertical);

            auto xml_tree = make_unique<XML_tree>();
            xml_tree->load_file(file_name);

            auto text_display = make_unique<XML_node_display>();

            QObject::connect(xml_tree.get(), &XML_tree::itemClicked,
                    text_display.get(), &XML_node_display::display_node_text);

            QObject::connect(xml_tree.get(), &XML_tree::reloaded,
                    text_display.get(), &XML_node_display::clear);

            central_splitter->addWidget(xml_tree.release());
            central_splitter->addWidget(text_display.release());
            central_splitter->setStretchFactor(0, 1.5);

            setCentralWidget(central_splitter.release());
        }
}
