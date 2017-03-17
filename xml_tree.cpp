#include <iostream>
#include <memory>

#include <QStringList>
#include <QBoxLayout>
#include <QDomDocument>
#include <QFile>
#include <QFont>
#include <QRegularExpression>

#include "xml_tree.h"


namespace xml_viewer
{
    XML_tree::XML_tree()
    {
        file_watcher_ = make_unique<File_watcher>();
        QObject::connect(
                file_watcher_->get_fs_watcher(),
                &QFileSystemWatcher::fileChanged,
                this, &XML_tree::load_file);

        QFont font(this->font().family(), 25);
        setFont(font);
        mappings_filter_ = make_unique<Editor_mappings>();
        installEventFilter(mappings_filter_.get());
        // setIndentation(indentation() * 2);
        // setWordWrap(true);
    }

    const QString& element_name_template()
    {
        static const QString template_string = QString(
                "%1");

        return template_string;
    }

    const QString& attribute_template()
    {
        static const QString template_string = QString(
                "%1 %2=%3");

        return template_string;
    }

    bool XML_tree::load_file(const QString& file_name)
    {
        std::cout << "load file: " << file_name.toStdString() << std::endl;

        file_watcher_->set_file(file_name);
        setHeaderLabels(QStringList{file_name});

        QFile file{file_name};
        if(!file.open(QIODevice::ReadOnly)){
            clear();
            return false;
        }

        QDomDocument document{};
        if(!document.setContent(&file)){
            clear();
            return false;
        }
        auto dom_root{document.documentElement()};
        build_widget_tree(dom_root);
        emit reloaded();
        return true;
    }


    void XML_tree::build_widget_tree(const QDomElement& dom_root)
    {
        clear();
        add_dom_node(dom_root, invisibleRootItem());
    }

    void add_dom_attribute(const QDomAttr& dom_attribute,
            QTreeWidgetItem* widget_node)
    {
        widget_node->setText(0, attribute_template().arg(
                    QString(widget_node->text(0)),
                    QString(dom_attribute.name()),
                    QString(dom_attribute.value())).simplified());
    }

    void add_dom_text_node(const QDomText& dom_text_node,
            QTreeWidgetItem* widget_node)
    {
        auto new_child = make_unique<QTreeWidgetItem>(
                QStringList{dom_text_node.data().simplified()});
        new_child->setData(0, User_roles::is_element, false);
        widget_node->addChild(new_child.release());
    }

    void add_dom_node(const QDomNode& dom_node,
            QTreeWidgetItem* widget_node)
    {
        if(dom_node.isAttr()){
            return add_dom_attribute(dom_node.toAttr(), widget_node);
        }
        if(dom_node.isElement()){
            auto dom_element{dom_node.toElement()};
            auto text = element_name_template().arg(dom_element.tagName());
            auto new_child = make_unique<QTreeWidgetItem>(
                    QStringList{text});
            new_child->setData(0, User_roles::is_element, true);
            auto new_widget_node = new_child.get();
            widget_node->addChild(new_child.release());
            auto attributes = dom_element.attributes();
            auto n_attributes = attributes.size();
            for(int i = 0; i != n_attributes; ++i){
                add_dom_node(attributes.item(i), new_widget_node);
            }
            auto children = dom_element.childNodes();
            auto n_children = children.size();
            for(int i = 0; i != n_children; ++i){
                add_dom_node(children.item(i), new_widget_node);
            }
            return;
        }
        if(dom_node.isText()){
            return add_dom_text_node(
                    dom_node.toText(), widget_node);
        }
        // processing instructions, comments, namespace: ignored
        // document: not added, only root node is added
    }
}
