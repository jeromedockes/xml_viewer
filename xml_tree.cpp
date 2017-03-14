#include <QBoxLayout>
#include <QLabel>
#include <QDomDocument>
#include <QFile>
#include <QFont>

#include "utils.h"
#include "xml_tree.h"


namespace xml_viewer
{

    bool XML_tree::load_file(const QString& file_name)
    {
        QFile file{file_name};

        if(!file.open(QIODevice::ReadOnly)){
            return false;
        }

        QDomDocument document{};

        if(!document.setContent(&file)){
            return false;
        }

        auto dom_root{document.documentElement()};
        build_widget_tree(dom_root);
        return true;
    }

    void XML_tree::build_widget_tree(const QDomElement& dom_root)
    {
        clear();
        add_dom_node(dom_root, invisibleRootItem());
    }


    void add_dom_node(const QDomNode& dom_node,
            QTreeWidgetItem* widget_node)
    {
        if(dom_node.isAttr()){
            auto dom_attribute = dom_node.toAttr();
            widget_node->setText(0, QString("%1; %2=%3").arg(
                        QString(widget_node->text(0)),
                        QString(dom_attribute.name()),
                        QString(dom_attribute.value())).simplified());
            return;
        }
        auto new_widget_node_handle = make_unique<QTreeWidgetItem>();
        auto new_widget_node = new_widget_node_handle.get();
        new_widget_node->setFont(0,
                QFont(new_widget_node->font(0).family(), 25));
        widget_node->addChild(new_widget_node_handle.release());
        if(dom_node.isElement()){
            auto dom_element{dom_node.toElement()};
            new_widget_node->setText(0, dom_element.tagName());
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
            auto dom_text_node = dom_node.toText();
            auto label = make_unique<QLabel>(
                    dom_text_node.data().simplified());
            label->setWordWrap(true);
            QFont font(label->font().family(), 25);
            label->setFont(font);
            label->adjustSize();
            auto tree_widget = widget_node->treeWidget();
            tree_widget->setItemWidget(new_widget_node, 0, label.release());
            return;
        }
        // processing instructions, comments, namespace: ignored
        // document: not added, only root node is added
    }
}
