#include <iostream>

#include <QSplitter>
#include <QKeySequence>
#include <QShortcut>

#include "utils.h"
#include "xml_tree.h"
#include "main_window.h"
#include "xml_node_display.h"
#include "html_display.h"

namespace xml_viewer
{

    void Main_window::create_html_viewer(const QString& file_name)
    {
        auto html_display = make_unique<HTML_display>();
        html_display->load_file(file_name);
        setCentralWidget(html_display.release());
    }

    void Main_window::create_xml_viewer(const QString& file_name)
    {
        auto central_splitter = make_unique<QSplitter>(Qt::Vertical);

        auto xml_tree = make_unique<XML_tree>();
        xml_tree->load_file(file_name);

        auto text_display = make_unique<XML_node_display>();

        QObject::connect(xml_tree.get(), &XML_tree::currentItemChanged,
                text_display.get(), &XML_node_display::display_node_text);

        central_splitter->addWidget(xml_tree.release());
        central_splitter->addWidget(text_display.release());
        central_splitter->setStretchFactor(0, 1.5);

        setCentralWidget(central_splitter.release());
    }

    void Main_window::add_shortcuts()
    {
        // *this takes possesion of shortcut (passed as parent)
        auto q_shortcut = new QShortcut(QKeySequence{"Ctrl+Q"}, this);
        QObject::connect(q_shortcut, &QShortcut::activated,
                this, &Main_window::close);

        // *this takes possesion of shortcut (passed as parent)
        auto w_shortcut = new QShortcut(QKeySequence{"Ctrl+W"}, this);
        QObject::connect(w_shortcut, &QShortcut::activated,
                this, &Main_window::close);
    }

    Main_window::Main_window(const QString& file_name, bool render_html)
        :QMainWindow{}
    {
        add_shortcuts();

        if(render_html){
            create_html_viewer(file_name);
            return;
        }
        create_xml_viewer(file_name);
    }
}
