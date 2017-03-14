#include "utils.h"
#include "xml_tree.h"
#include "main_window.h"

namespace xml_viewer
{

    Main_window::Main_window(const char* file_name)
        :QMainWindow{} {

            auto xml_tree = make_unique<XML_tree>();
            xml_tree->load_file(file_name);
            setCentralWidget(xml_tree.release());
        }
}
