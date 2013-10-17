/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2013  Mattia Basaglia

Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "settings.hpp"
#include "resource_manager.hpp"
#include <QSettings>
#include "xml_exporter.hpp"
#include "xml_loader.hpp"
#include "knot_view.hpp"
#include <QStyleFactory>
#include <QApplication>
#include <QStyle>

Settings::Settings()
    : save_nothing(false),
      m_save_toolbars(true),
      m_save_ui(true), m_icon_size(22), tool_button_style(Qt::ToolButtonIconOnly),
      m_max_recent_files(5),
      m_graph_cache(false), m_fluid_refresh(true), m_antialiasing(true), m_script_timeout(0),
      m_save_grid(true), m_grid_enabled(true), m_grid_size(32), m_grid_shape(Snapping_Grid::SQUARE),
      m_check_unsaved_files(true),
      m_save_knot_style(false),
      m_clipboard(KNOT|SVG)
{
}

void Settings::load_config()
{
    QSettings settings("knotter","knotter");

    config_version = settings.value("version",Resource_Manager::program_version()).toString();

    Resource_Manager::change_lang_code( settings.value("language",
                            Resource_Manager::current_lang_code()).toString() );


    settings.beginGroup("recent_files");
    m_recent_files = settings.value("list",m_recent_files).toStringList();
    set_max_recent_files ( settings.value("max",m_max_recent_files).toInt() );
    settings.endGroup();// recent_files

    m_graph_cache = settings.value("performance/cache",m_graph_cache).toBool();
    m_fluid_refresh = settings.value("performance/fluid_refresh",m_fluid_refresh).toBool();
    m_antialiasing = settings.value("performance/antialiasing",m_antialiasing).toBool();
    m_script_timeout = settings.value("performance/script_timeout",m_script_timeout).toInt();

    m_save_knot_style = settings.value("style/save",m_save_knot_style).toBool();
    saved_knot_style_xml = settings.value("style/xml",saved_knot_style_xml).toString();

    m_clipboard = Clipboard(settings.value("clipboard",int(m_clipboard)).toInt());

    settings.beginGroup("grid");
    m_save_grid = settings.value("save",m_save_grid).toBool();
    m_grid_enabled = settings.value("enabled",m_grid_enabled).toBool();
    m_grid_size = settings.value("size",m_grid_size).toInt();
    m_grid_shape = Snapping_Grid::Grid_Shape(settings.value("shape",int(m_grid_shape)).toInt());
    settings.endGroup();

    m_check_unsaved_files = settings.value("check_unsaved_files",m_check_unsaved_files).toBool();

    QStringList disabled_plugins = settings.value("disabled_plugins").toStringList();
    foreach(Plugin* p, Resource_Manager::plugins())
    {
        QString name = p->string_data("plugin_file");
        if ( !name.isEmpty() && disabled_plugins.contains(name) )
            p->enable(false);
    }

    settings.beginGroup("gui");
    Node::radius = settings.value("node/radius",Node::radius).toInt();

    QString style = settings.value("style").toString();
    if ( QStyleFactory::keys().contains(style,Qt::CaseInsensitive) )
    {
        QApplication::setStyle(style);
    }


    m_save_toolbars = settings.value("save_toolbars",m_save_toolbars).toBool();
    int ntoolbars = settings.beginReadArray("toolbar");
    for ( int i = 0; i < ntoolbars; i++ )
    {
        settings.setArrayIndex(i);

        String_Toolbar tb;

        tb.name = settings.value("name").toString();
        if ( tb.name.isEmpty() )
        {
             qWarning() << tr("Warning:")
                        << tr("Not loading toolbar without name");
            continue;
        }
        tb.title = settings.value("title").toString();

        tb.actions = settings.value("items").toStringList();

        toolbars.push_back(tb);
    }
    settings.endArray();//toolbars


    m_save_ui = settings.value("save",m_save_ui).toBool();
    m_window_geometry = settings.value("geometry").toByteArray();
    m_window_state = settings.value("state").toByteArray();

    m_icon_size = settings.value("icon_size",m_icon_size).toInt();

    tool_button_style = Qt::ToolButtonStyle(
                settings.value("buttons",tool_button_style).toInt() );

    settings.endGroup();//gui


    settings.beginGroup("colors");
    Node::color_resting = settings.value("node/resting",Node::color_resting.name()).toString();
    Node::color_highlighted = settings.value("node/highlighted",Node::color_highlighted.name()).toString();
    settings.endGroup();//colors



}

void Settings::save_config()
{
    if ( save_nothing )
        return;

    QSettings settings("knotter","knotter");

    settings.setValue("version",Resource_Manager::program_version());


    settings.setValue("language", Resource_Manager::current_lang_code());


    settings.beginGroup("recent_files");
    settings.setValue("list",m_recent_files);
    settings.setValue("max",m_max_recent_files );
    settings.endGroup();// recent_files

    settings.setValue("performance/cache",m_graph_cache);
    settings.setValue("performance/fluid_refresh",m_fluid_refresh);
    settings.setValue("performance/antialiasing",m_antialiasing);
    settings.setValue("performance/script_timeout",m_script_timeout);

    settings.setValue("style/save",m_save_knot_style);
    settings.setValue("style/xml",saved_knot_style_xml);


    settings.setValue("clipboard",int(m_clipboard));


    settings.beginGroup("grid");
    settings.setValue("enabled",m_grid_enabled);
    settings.setValue("save",m_save_grid);
    settings.setValue("size",m_grid_size);
    settings.setValue("shape",int(m_grid_shape));
    settings.endGroup();

    settings.setValue("check_unsaved_files",m_check_unsaved_files);

    QStringList disabled_plugins;
    foreach ( Plugin* p, Resource_Manager::plugins() )
    {
        if ( !p->is_enabled() )
            disabled_plugins << p->string_data("plugin_file");
    }
    settings.setValue("disabled_plugins",disabled_plugins);

    settings.beginGroup("gui");

    settings.setValue("style",QApplication::style()->objectName());

    settings.setValue("save_toolbars",m_save_toolbars);
    settings.beginWriteArray("toolbar",toolbars.size());
    for(int i = 0; i < toolbars.size(); i++ )
    {
        settings.setArrayIndex(i);
        settings.setValue("name",toolbars[i].name);
        settings.setValue("title",toolbars[i].title);
        settings.setValue("items",toolbars[i].actions);
    }
    settings.endArray();


    settings.setValue("save",m_save_ui);
    settings.setValue("geometry",m_window_geometry);
    settings.setValue("state",m_window_state);

    settings.setValue("icon_size",m_icon_size);

    settings.setValue("buttons",int(tool_button_style));

    settings.endGroup();//gui


    settings.beginGroup("colors");
    settings.setValue("node/resting",Node::color_resting.name());
    settings.setValue("node/highlighted",Node::color_highlighted.name());
    settings.endGroup();//colors
}

void Settings::clear_config()
{
    save_nothing = true;

    QSettings settings("knotter","knotter");
    settings.clear();
}


void Settings::initialize_window(QMainWindow *w)
{
    if ( !toolbars.empty() && m_save_toolbars )
    {
        foreach(QToolBar* tb, w->findChildren<QToolBar*>() )
            delete tb;
        foreach(const String_Toolbar& tb, toolbars)
            tb.create(w);
    }

    if ( m_save_ui )
    {
        w->restoreGeometry(m_window_geometry);
        w->restoreState(m_window_state);
        w->setIconSize(QSize(m_icon_size,m_icon_size));
        w->setToolButtonStyle(tool_button_style);
    }

}

void Settings::save_window(QMainWindow *w)
{
    toolbars.clear();
    foreach(QToolBar* tb, w->findChildren<QToolBar*>())
        toolbars.append(tb);
    m_icon_size = w->iconSize().width();
    tool_button_style = w->toolButtonStyle();
    m_window_geometry = w->saveGeometry();
    m_window_state = w->saveState();
}

QString Settings::version() const
{
    return config_version;
}

bool Settings::least_version(int maj, int min) const
{
    return Resource_Manager::check_least_version(config_version,maj,min);
}

bool Settings::current_version() const
{
    return Resource_Manager::program_version() == config_version;
}

void Settings::icon_size(int sz)
{
    m_icon_size = sz;
    emit icon_size_changed(sz);
}

void Settings::button_style(Qt::ToolButtonStyle bs)
{
    tool_button_style = bs;
    emit tool_button_style_changed(bs);
}


void Settings::add_recent_file(QString file_name)
{

    m_recent_files.push_front(file_name);
    m_recent_files.removeDuplicates();
    while ( m_recent_files.size() > m_max_recent_files )
        m_recent_files.pop_back();

}

void Settings::set_max_recent_files(int max)
{
    m_max_recent_files = max;
    if ( m_recent_files.size() > max )
    {
        m_recent_files.erase(m_recent_files.begin()+max,m_recent_files.end());
    }
}

void Settings::clear_recent_files()
{
    m_recent_files.clear();
}

void Settings::set_knot_style(const Graph &graph)
{
    saved_knot_style_xml = export_xml_style(graph);
}

void Settings::get_knot_style(Graph &graph) const
{
    if ( m_save_knot_style )
        import_xml_style(saved_knot_style_xml,graph);
}

void Settings::set_clipboard_feature(Settings::Clipboard_Enum feature, bool enable)
{
    if ( enable )
       m_clipboard |= feature;
    else
        m_clipboard &= ~feature;
}
