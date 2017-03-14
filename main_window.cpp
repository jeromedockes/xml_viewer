#include "utils.h"
#include "xml_tree.h"
#include "main_window.h"

namespace xml_viewer
{

    Main_window::Main_window()
        :QMainWindow{} {

            auto xml_tree = make_unique<XML_tree>();
            xml_tree->load_file("/home/jerome/workspace/cognitive_structure/topic_models/xml/jats_full.xml");
            setCentralWidget(xml_tree.release());
        }
}
