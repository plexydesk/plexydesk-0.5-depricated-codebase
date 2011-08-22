/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Varuna Lekamwasam <vrlekamwasam@gmail.com>
*                 PhobosK <phobosk@kbfx.net>
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

.pragma library


function goUp(flm,backHold,back,rec1,ct,bb)
{
    var _folder = flm.parentFolder.toString().replace(/^.*\/\.\.$/, "file:");
    if(_folder == "") return;
    flm.folder = _folder;
    setOpacityBug(backHold,back,rec1,ct,bb);
}

function goInto(to,flm,ic)
{
    var cpath = flm.folder.toString();
    var npath = cpath + "/" + to;
    flm.folder = npath;
    // Folders are not opened. Handled internally by FolderListModel
    flm.openFile(npath);
}

function getFolderName(flm)
{
    var vpath = flm.folder.toString().substring(7,flm.folder.toString().length);
    if(vpath.length < 2)
        return vpath;

    if (vpath.lastIndexOf('/') != vpath.length-1) {
        var name1 = vpath.substring(0,vpath.length);
        var tp1 = name1
        vpath = name1.substring(tp1.lastIndexOf('/')+1,tp1.length);
        }
    else {
        var name = vpath.substring(0,vpath.length-1);
        var tp = name
        vpath = name.substring(tp.lastIndexOf('/')+1,tp.length);
        }

    if (vpath.length > 30)
        return vpath.substring(0,27) + "...";
    else
        return vpath;
}

function getFileName(filename)
{
    if (filename.length > 10)
        return filename.substring(0,7) + "...";
    else
        return filename;
}

function setImageViewer(ic,iv,imt,path,type,name)
{
    ic.state = "viewImage";
    iv.source = path;
    imt.text = name;
}

function setOpacityBug(backHold,back,rec1,ct,bb)
{
    backHold.state = "";
    rec1.opacity = 1;
    ct.opacity = 1;
    bb.opacity = 1;
    setZIndexes(back,ct,bb);
}

function setZIndexes(back,ct,bb)
{
    back.z = -100;
    ct.z = 50;
    bb.z = 50;
}
