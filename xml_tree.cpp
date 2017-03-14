#include <iostream>
#include <memory>

#include <QStringList>
#include <QBoxLayout>
#include <QDomDocument>
#include <QFile>
#include <QDir>
#include <QFont>
#include <QFileInfo>
#include <QRegularExpression>

#include "utils.h"
#include "xml_tree.h"


namespace xml_viewer
{

    const QString& element_name_template()
    {
        static const QString highlighted = QString(
                "<b><font color=blue>%1</font></b>");

        return highlighted;
    }

    const QString& attribute_template()
    {
        static const QString highlighted = QString(
                "%1 <font color=DarkOrange>%2</font>=%3");

        return highlighted;
    }

    bool XML_tree::load_file(const QString& file_name)
    {
        set_doc_file(file_name);

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

    bool XML_tree::reload_file(const QString& path)
    {
        std::cout << "reloading " << doc_file_.toStdString() << std::endl
            << "because " << path.toStdString() << " changed" << std::endl;
        return load_file(doc_file_);
    }

    void XML_tree::watch_again()
    {
        if(file_watcher_ == nullptr){
            return;
        }
        file_watcher_->addPath(doc_file_);
    }

    void XML_tree::set_doc_file(const QString& new_doc_file)
    {
        QFileInfo file_info{new_doc_file};
        doc_file_ = new_doc_file;
        auto directory = file_info.absoluteDir().absolutePath();
        if(file_watcher_ == nullptr){
            file_watcher_ = make_unique<QFileSystemWatcher>(
                    QStringList{new_doc_file, directory});
            QObject::connect(
                    file_watcher_.get(), &QFileSystemWatcher::fileChanged,
                    this, &XML_tree::reload_file);
            QObject::connect(
                    file_watcher_.get(), &QFileSystemWatcher::directoryChanged,
                    this, &XML_tree::watch_again);
            return;
        }
        file_watcher_->removePaths(file_watcher_->files());
        file_watcher_->removePaths(file_watcher_->directories());
        file_watcher_->addPaths(QStringList{new_doc_file, directory});
    }

    void XML_tree::build_widget_tree(const QDomElement& dom_root)
    {
        clear();
        add_dom_node(dom_root, invisibleRootItem());
    }

    QString highlight_links(const QString& plain)
    {
        return QString(plain).replace(
                QRegularExpression("(https?://\\S+)"),
                QString("<a href='\\1'>\\1</a>"));
    }

    void add_dom_attribute(const QDomAttr& dom_attribute,
            QTreeWidgetItem* widget_node)
    {
        auto label = static_cast<QLabel*>(
                widget_node->treeWidget()->itemWidget(widget_node, 0));
        label->setText(attribute_template().arg(
                    QString(label->text()),
                    QString(dom_attribute.name()),
                    QString(dom_attribute.value())).simplified());
    }

    QTreeWidgetItem* add_widget_child(std::unique_ptr<QWidget>&& new_child,
            QTreeWidgetItem* parent_node)
    {
        auto new_widget_node_handle = make_unique<QTreeWidgetItem>();
        auto new_widget_node = new_widget_node_handle.get();
        parent_node->addChild(new_widget_node_handle.release());
        auto tree_widget = parent_node->treeWidget();
        tree_widget->setItemWidget(new_widget_node, 0, new_child.release());
        return new_widget_node;
    }

    std::unique_ptr<QLabel> make_wrapped_label(const QString& text)
    {
        auto label = make_unique<QLabel>(text);
        label->setTextFormat(Qt::RichText);
        label->setWordWrap(true);
        QFont font(label->font().family(), 25);
        label->setFont(font);
        label->setTextInteractionFlags(
                Qt::TextBrowserInteraction | Qt::TextSelectableByKeyboard);
        // label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        label->setOpenExternalLinks(true);

        return std::move(label);
    }

    void add_dom_text_node(const QDomText& dom_text_node,
            QTreeWidgetItem* widget_node)
    {
        auto text = highlight_links(dom_text_node.data().simplified());
        auto label = make_wrapped_label(text);
        add_widget_child(std::move(label), widget_node);
    }

    void add_dom_node(const QDomNode& dom_node,
            QTreeWidgetItem* widget_node)
    {
        if(dom_node.isAttr()){
            return add_dom_attribute(dom_node.toAttr(), widget_node);
        }
        if(dom_node.isElement()){
            auto dom_element{dom_node.toElement()};
            auto label = make_wrapped_label(
                    element_name_template().arg(dom_element.tagName()));
            auto new_widget_node = add_widget_child(
                    std::move(label), widget_node);
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
