#ifndef XML_VIEWER_XML_TREE_H
#define XML_VIEWER_XML_TREE_H

#include <QDomElement>
#include <QTreeWidget>

namespace xml_viewer
{

    class XML_tree: public QTreeWidget{

        public:

            XML_tree()
                :QTreeWidget{}{}

            bool load_file(const QString& file_name);

        private:

            void build_widget_tree(const QDomElement& dom_root);
    };

    void add_dom_node(const QDomNode& dom_node,
            QTreeWidgetItem* widget_node);

    void add_dom_attribute(const QDomAttr& dom_attribute,
            QTreeWidgetItem* widget_node);

    void add_dom_text_node(const QDomText& dom_text_node,
            QTreeWidgetItem* widget_node);
}

#endif
