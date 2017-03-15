#ifndef XML_VIEWER_XML_TREE_H
#define XML_VIEWER_XML_TREE_H

#include <QDomElement>
#include <QTreeWidget>
#include <QLabel>
#include <memory>

#include "utils.h"

namespace xml_viewer
{

    class XML_tree: public QTreeWidget{

        Q_OBJECT

        public:
            XML_tree();

        public slots:

            bool load_file(const QString& file_name);

        signals:

            void reloaded() const;

        private:

            void build_widget_tree(const QDomElement& dom_root);
            std::unique_ptr<File_watcher> file_watcher_{};
            std::unique_ptr<Editor_mappings> mappings_filter_{};
    };

    const QString& element_name_template();
    const QString& attribute_template();

    void add_dom_node(const QDomNode& dom_node,
            QTreeWidgetItem* widget_node);

    void add_dom_attribute(const QDomAttr& dom_attribute,
            QTreeWidgetItem* widget_node);

    void add_dom_text_node(const QDomText& dom_text_node,
            QTreeWidgetItem* widget_node);

    std::unique_ptr<QLabel> make_wrapped_label(const QString& text);

}

#endif
