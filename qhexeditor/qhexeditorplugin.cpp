#include "qhexeditor.h"
#include "qhexeditorplugin.h"

#include <QtPlugin>

QHexEditorPlugin::QHexEditorPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QHexEditorPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool QHexEditorPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QHexEditorPlugin::createWidget(QWidget *parent)
{
    return new QHexEditor(parent);
}

QString QHexEditorPlugin::name() const
{
    return QLatin1String("QHexEditor");
}

QString QHexEditorPlugin::group() const
{
    return QLatin1String("");
}

QIcon QHexEditorPlugin::icon() const
{
    return QIcon();
}

QString QHexEditorPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QHexEditorPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QHexEditorPlugin::isContainer() const
{
    return false;
}

QString QHexEditorPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QHexEditor\" name=\"qHexEditor\">\n</widget>\n");
}

QString QHexEditorPlugin::includeFile() const
{
    return QLatin1String("qhexeditor.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qhexeditorplugin, QHexEditorPlugin)
#endif // QT_VERSION < 0x050000
