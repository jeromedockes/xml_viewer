#ifndef XML_VIEWER_XML_NODE_DISPLAY_H
#define XML_VIEWER_XML_NODE_DISPLAY_H

#include <QTextBrowser>
#include <QTreeWidgetItem>

namespace xml_viewer
{

    class XML_node_display: public QTextBrowser{

        public:

            XML_node_display();

        public slots:

            void display_node_text(const QTreeWidgetItem* new_node);

        protected:

            bool focusNextPrevChild(bool next)
            {
                (void) next;
                return false;
            }

        private:

            std::unique_ptr<Editor_mappings> mappings_filter_{};
    };

    QString highlighted_text(const QTreeWidgetItem& item);
}

#endif
