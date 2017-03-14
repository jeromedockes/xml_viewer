#ifndef XML_VIEWER_XML_TREE_H
#define XML_VIEWER_XML_TREE_H

#include <QDomElement>
#include <QTreeWidget>
#include <QLabel>
#include <QFileSystemWatcher>

namespace xml_viewer
{

    class XML_tree: public QTreeWidget{

        public:

            XML_tree()
                :highlight_text_{true}{}

            XML_tree(bool highlight_text)
                :highlight_text_{highlight_text}{}

        public slots:

            bool load_file(const QString& file_name);
            bool reload_file(const QString&);
            void watch_again();

        private:

            void build_widget_tree(const QDomElement& dom_root);
            void set_doc_file(const QString& new_doc_file);
            bool highlight_text_;
            QString doc_file_{};
            std::unique_ptr<QFileSystemWatcher> file_watcher_{};
    };

    const QString& element_name_template(bool highlight);
    const QString& attribute_template(bool highlight);

    QTreeWidgetItem*add_widget_child(std::unique_ptr<QWidget>&& new_child,
            QTreeWidgetItem* parent_node);

    void add_dom_node(const QDomNode& dom_node,
            QTreeWidgetItem* widget_node, bool highlight);

    void add_dom_attribute(const QDomAttr& dom_attribute,
            QTreeWidgetItem* widget_node, bool highlight);

    void add_dom_text_node(const QDomText& dom_text_node,
            QTreeWidgetItem* widget_node, bool highlight);

    std::unique_ptr<QLabel> make_wrapped_label(
            const QString& text, bool highlight);
}

#endif
