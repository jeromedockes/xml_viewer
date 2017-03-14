#include "utils.h"
#include "xml_node_display.h"

namespace xml_viewer
{
    XML_node_display::XML_node_display()
    {
        setReadOnly(true);
        setOpenExternalLinks(true);
        QFont font(this->font().family(), 25);
        setFont(font);
        setTextInteractionFlags(
                Qt::TextBrowserInteraction | Qt::TextSelectableByKeyboard);
        // setOpenExternalLinks(true);
    }

    void XML_node_display::display_node_text(const QTreeWidgetItem* new_node)
    {
        setText(highlighted_text(new_node));
    }

    QString highlighted_text(const QTreeWidgetItem* item)
    {
        auto text = QString{item->text(0)};

        if(item->data(0, User_roles::is_element).toBool()){
            text.replace(QRegularExpression("(\\S*)=(\\S*)"),
                    QString("<font color=DarkOrange>\\1</font>=\\2"));

            text.replace(QRegularExpression("^\\s*(\\S+)"),
                    QString("<b><font color=blue>\\1</font></b>"));
        }

        return text.replace(
                QRegularExpression("(https?://\\S+)"),
                QString("<a href='\\1'>\\1</a>"));
    }


}
