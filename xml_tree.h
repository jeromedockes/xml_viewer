#ifndef XML_VIEWER_XML_TREE_H
#define XML_VIEWER_XML_TREE_H

#include <QDomElement>
#include <QTreeWidget>
#include <QLabel>
#include <QFileSystemWatcher>
#include <memory>

namespace xml_viewer
{

    class XML_tree: public QTreeWidget{

        Q_OBJECT

        public:
            XML_tree();

        public slots:

            bool load_file(const QString& file_name);
            bool reload_file(const QString&);
            void watch_again();

        signals:

            void reloaded() const;

        private:

            void build_widget_tree(const QDomElement& dom_root);
            void set_doc_file(const QString& new_doc_file);
            QString doc_file_{};
            std::unique_ptr<QFileSystemWatcher> file_watcher_{};
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
