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

function goUp(flm)
{
    flm.folder = flm.parentFolder.toString()
}
function goInto(to,flm,ic,iv,imt)
{
    var cpath = flm.folder.toString();
    var npath = cpath + "/" + to;
    var type = getType(to);
    flm.folder = npath;
    if(type === "png" || type === "PNG" || type === "jpg" || type === "jpeg" || type === "JPG" || type === "JPEG"){
        setImageViewer(ic,iv,imt,npath,type,to);
    }
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
        return "images/folder.png";
    else if(type === "pdf")
        return "images/pdf.png";
    else if(type === "txt" || type === "doc" || type === "docx" || type === "sh")
        return "images/doc.png";
    else if (type === "png" || type === "PNG")
        return "images/png.png";
    else if (type=== "jpeg" || type == "jpg" || type === "JPG" || type === "JPEG")
        return "images/jpeg.png"
    else if (type === "zip"||type ==="rar"||type ==="gz")
        return "images/zip.png";
    else if (type === "mpeg" || type === "MPEG" || type === "MPG" || type === "mpg" || type === "AVI" || type === "avi" || type === "mov" || type === "MOV")
        return "images/mov.png";
    else if (type === "WMA" || type === "wma")
        return "images/wma.png";
    else if (type === "mp3" || type === "MP3" || type === "m3u")
        return "images/music.png";
    else if (type === "cpp")
        return "images/cpp.png";
    else if (type === "h")
        return "images/h.png";
    else if (type === "html")
        return "images/html.png";
    else if (type === "o" || type === "out")
        return "images/o.png";
    else if (type === "iso" || type === "bin" || type === "nrg" || type === "daa")
        return "images/iso.png";
    else if (type=== "exe")
        return "images/exe.png";
    else if (type === "java")
	return "images/java.png";
    else
        return "images/default.png";
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

function setOpacityBug(backHold,back,ic,rec1,ct,bb)
{
    backHold.state = "";
    ic.opacity = 1;
    rec1.opacity = 1;
    ct.opacity = 1;
    bb.opacity = 1;
    setZIndexes(back,ic,ct,bb);
}

function setZIndexes(back,ic,ct,bb)
{
    back.z = -100;
    ic.z = 0
    ct.z = 50
    bb.z = 50;
}

