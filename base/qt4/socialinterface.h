/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/

#ifndef PLEXY_SOCIAL_INTERFACE
#define PLEXY_SOCIAL_INTERFACE

#include <abstractplugininterface.h>
#include <socialplugin.h>

namespace PlexyDesk
{
class PLEXYDESK_EXPORT SocialInterface : public QObject, public AbstractPluginInterface
{
    Q_OBJECT
public:
    void init() {
    }
    virtual ~SocialInterface() {
    }
    virtual SocialPlugin *instance() = 0;
};
} // namespace PlexyDesk

Q_DECLARE_INTERFACE(PlexyDesk::SocialInterface, "org.plexydesk.SocialInterface")
#endif
