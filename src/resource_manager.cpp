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
#include "resource_manager.hpp"
#include <QDir>
#include <QIcon>
#include <QCoreApplication>

Settings Resource_Manager::settings;
Resource_Manager Resource_Manager::singleton;

QString Resource_Manager::program_name()
{
    return tr("Knotter");
}

QString Resource_Manager::program_version()
{
    return QString(VERSION);
}

bool Resource_Manager::has_least_version(int maj, int min)
{
    return check_least_version ( program_version(), maj, min );
}

bool Resource_Manager::check_least_version(QString version, int maj, int min)
{
    QStringList v = version.split(".");
    //int i = v[0].toInt(), j = v[1].toInt();
    if ( v.size() < 2)
        return false;
    else
        return v[0].toInt() >= maj&& v[1].toInt() >= min;
}

QString Resource_Manager::data(QString name)
{
    QDir path ( DATA_DIR ); // install dir

    if ( !path.exists(name) )
        path.setPath( QCoreApplication::applicationDirPath() ); // executable dir

    if ( !path.exists(name) )
        path.setPath( QDir::currentPath() ); // current dir


    if ( !path.exists(name) )
        return QString(); // not found

    return QDir::cleanPath(path.absoluteFilePath(name));
}



void Resource_Manager::initialize(QString default_lang_code)
{
    // Initialize Icon theme

    QIcon::setThemeSearchPaths(
#ifdef DEBUG
                QStringList()
#else
                QIcon::themeSearchPaths()
#endif
                 << data("img/icons") );

    QIcon::setThemeName("knotter-icons");

    // Load Settings
    settings.load_config();

    // Translation

    if ( !default_lang_code.isEmpty() )
    {
        QString name = language_name(default_lang_code);
        if ( !name.isEmpty() )
            register_translation(name,default_lang_code,QString());
    }

    QDir translations = data("translations");
    QStringList translation_files = translations.entryList(QStringList()<<"*.qm");

    QRegExp re("[^_]+_([^.]+)\\.qm");
    foreach ( QString file, translation_files )
    {
        if ( re.exactMatch(file) )
        {
            QString code = re.cap(1);
            QString name = language_name(code);
            if ( !name.isEmpty() )
                register_translation(name,code,file);
        }
        else
            qWarning() << tr("Warning:")
                       << tr("Unrecognised translation file name pattern: %1")
                          .arg(file);
    }
}


Resource_Manager::~Resource_Manager()
{
    settings.save_config();


    foreach ( QTranslator* tr, translators.values() )
        delete tr;
}



QString Resource_Manager::language_name(QString lang_code)
{
    QLocale lang_loc = QLocale(lang_code);
    QString name;

    #if HAS_QT_4_8
        name = lang_loc.nativeLanguageName(); // native name
    #else
        name = QLocale::languageToString(lang_loc.language()); // English name
    #endif

    if ( !name.isEmpty() )
        name[0] = name[0].toUpper();

    return name;
}

void Resource_Manager::register_translation(QString name, QString code, QString file)
{
    singleton.lang_names[name]=code;
    if ( !file.isEmpty() )
    {
        QTranslator* ntrans = new QTranslator;
        ntrans->load(file);
        singleton.translators[code]=ntrans;
    }
    else
        singleton.translators[code] = NULL;
}

QString Resource_Manager::current_lang_name()
{
    return singleton.lang_names.key(current_lang_code());
}

QString Resource_Manager::current_lang_code()
{
    return singleton.translators.key(singleton.current_translator);
}

QTranslator *Resource_Manager::translator()
{
    return singleton.current_translator;
}

QStringList Resource_Manager::available_languages()
{
    return singleton.lang_names.keys();
}

void Resource_Manager::change_lang_code(QString code)
{

    if ( !translators.contains(code) )
    {
        QString base_code = code.left(code.lastIndexOf('_')); // en_US -> en
        bool found = false;
        foreach ( QString installed_code, translators.keys() )
        {
            if ( installed_code.left(installed_code.lastIndexOf('_')) == base_code )
            {
                code = installed_code;
                found = true;
                break;
            }
        }
        if ( not found )
        {
            qWarning() << tr("Warning:") <<
                          tr("There is no translation for language %1 (%2)")
                          .arg(language_name(code))
                          .arg(code);
            return;
        }
    }

    QCoreApplication* app = QCoreApplication::instance();
    app->removeTranslator(current_translator);
    current_translator = translators[code];
    app->installTranslator(current_translator);
    emit language_changed();
}

void Resource_Manager::change_lang_name(QString name)
{
    change_lang_code(lang_names[name]);
}

