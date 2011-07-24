/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Varuna Lekamwasam <vrlekamwasam@gmail.com>
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
    flm.folder = flm.parentFolder.toString();
    setOpacityBug(backHold,back,rec1,ct,bb);
}
function goInto(to,flm,ic,flv)
{
    var cpath = flm.folder.toString();
    var npath = cpath + "/" + to;
    var type = getType(to);
    flm.folder = npath;
    if(type !== "")
        flv.openFile(npath);
}
function getIconName(flm)
{
    return flm.folder.toString();
}

function getFolderName(flm)
{
    var vpath = flm.folder.toString().substring(7,flm.folder.toString().length);
    if(vpath.length < 2)
        return vpath;

    if (vpath.lastIndexOf('/') != vpath.length-1)
    {
        var name1 = vpath.substring(0,vpath.length);
        var tp1 = name1
        return name1.substring(tp1.lastIndexOf('/')+1,tp1.length);
    }
    else
    {
        var name = vpath.substring(0,vpath.length-1);
        var tp = name
        return name.substring(tp.lastIndexOf('/')+1,tp.length);
    }
}

function getTypeIcon(type)
{
    if(type === "")
        return "image://plexydesk/folder";
    else if(type === "pdf")
        return "image://plexydesk/pdf";
    else if(type === "txt" || type === "doc" || type === "docx" || type === "sh")
        return "image://plexydesk/doc";
    else if (type === "png" || type === "PNG")
        return "image://plexydesk/png";
    else if (type=== "jpeg" || type == "jpg" || type === "JPG" || type === "JPEG")
        return "image://plexydesk/jpeg"
    else if (type === "zip"||type ==="rar"||type ==="gz")
        return "image://plexydesk/zip";
    else if (type === "mpeg" || type === "MPEG" || type === "MPG" || type === "mpg" || type === "AVI" || type === "avi" || type === "mov" || type === "MOV")
        return "image://plexydesk/mov";
    else if (type === "WMA" || type === "wma")
        return "image://plexydesk/wma";
    else if (type === "mp3" || type === "MP3" || type === "m3u")
        return "image://plexydesk/music";
    else if (type === "cpp")
        return "image://plexydesk/cpp";
    else if (type === "h")
        return "image://plexydesk/h";
    else if (type === "html")
        return "image://plexydesk/html";
    else if (type === "o" || type === "out")
        return "image://plexydesk/o";
    else if (type === "iso" || type === "bin" || type === "nrg" || type === "daa")
        return "image://plexydesk/iso";
    else if (type=== "exe")
        return "image://plexydesk/exe.png";
    else if (type === "java")
	return "image://plexydesk/java";
    else
        return "image://plexydesk/default";
}

function getType(filename)
{
    var isDotThere = filename.lastIndexOf('.');
    if (isDotThere > 0){
    var type = filename.substring(isDotThere+1,filename.length);
    return type;
    }
    else
    {
        return "";
    }
}

function getFileName(filename)
{
    var type = getType(filename);
    if (filename.length > 10)
        return filename.substring(0,7) + "...";
    else
        return filename;
}

function setImageViewer(ic,iv,imt,path,type,name)
{
        ic.state = "viewImage"
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
    ct.z = 50
    bb.z = 50;
}

