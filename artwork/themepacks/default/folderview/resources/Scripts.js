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
    var _folder = flm.parentFolder.toString();
    if(_folder == "") return;
    flm.folder = _folder;
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

    if (vpath.lastIndexOf('/') != vpath.length-1) {
        var name1 = vpath.substring(0,vpath.length);
        var tp1 = name1
        return name1.substring(tp1.lastIndexOf('/')+1,tp1.length);
        }
    else {
        var name = vpath.substring(0,vpath.length-1);
        var tp = name
        return name.substring(tp.lastIndexOf('/')+1,tp.length);
        }
}

function getTypeIcon(type)
{
    type = type.toLowerCase();

    if(type === "")
        return "image://plexydesk/folder";

    /* Executables */
    else if (type === "exe")
        return "image://plexydesk/exe";
    else if (type === "lnk")
        return "image://plexydesk/lnk";
    else if(type === "sh" || type === "bat")
        return "image://plexydesk/script";
    else if(type === "desktop")
        return "image://plexydesk/exec";

    /* Sources */
    else if (type === "cpp")
        return "image://plexydesk/cpp";
    else if (type === "c")
        return "image://plexydesk/c";
    else if (type === "h")
        return "image://plexydesk/h";
    else if (type === "o" || type === "out")
        return "image://plexydesk/obj";

    /* Int. Lang. */
    else if (type === "php" || type === "phtml" || type === "php3" || type === "php4" || type === "php5")
        return "image://plexydesk/php";
    else if (type === "pl")
        return "image://plexydesk/pl";
    else if (type === "py" || type === "pyc")
        return "image://plexydesk/py";
    else if (type === "java" || type === "jnlp" || type === "jar")
        return "image://plexydesk/java";
    else if (type === "js")
        return "image://plexydesk/js";
    else if (type === "rb")
        return "image://plexydesk/rb";
    else if (type === "xml")
        return "image://plexydesk/xml";

    /* Documents */
    else if(type === "pdf")
        return "image://plexydesk/pdf";
    else if(type === "txt")
        return "image://plexydesk/txt";
    else if(type === "doc" || type === "docx" || type === "odt" || type === "rtf")
        return "image://plexydesk/doc";
    else if(type === "xls" || type === "xlsx" || type === "xlt" || type === "xltx" || type === "xlb" || type === "ods")
        return "image://plexydesk/xls";
    else if(type === "ppt" || type === "pptx" || type === "pps" || type === "ppsx" || type === "odp")
        return "image://plexydesk/ppt";
    else if(type === "db" || type === "odb")
        return "image://plexydesk/odb";
    else if (type === "html" || type === "htm" || type === "shtml")
        return "image://plexydesk/html";
    else if (type === "url")
        return "image://plexydesk/url";

    /* Images */
    else if (type === "png" || type === "jpeg" || type == "jpg" || type === "bmp" || type === "tiff")
        return "image://plexydesk/img";
    else if (type === "svg")
        return "image://plexydesk/svg"

    /* Archives */
    else if (type === "zip" || type ==="rar" || type ==="7z" || type ==="gz" || type === "bz2" || type === "cab" || type === "sit")
        return "image://plexydesk/archive";

    /* Video */
    else if (type === "mpeg" || type === "mpg" || type === "avi" || type === "mp4" || type === "m4v" || type === "mkv" || type === "wmv" || type === "mov" || type === "aiff")
        return "image://plexydesk/video";

    /* Audio */
    else if (type === "mp3" || type === "wma" || type === "flac" || type === "m3u")
        return "image://plexydesk/audio";

    /* Disc Images */
    else if (type === "iso" || type === "bin" || type === "nrg" || type === "daa")
        return "image://plexydesk/iso";

    /* Packages */
    else if (type === "rpm" || type === "deb" || type === "dmg" || type === "tgz" || type === "ebuild")
        return "image://plexydesk/package";

    /* Misc */
    else if (type === "torrent")
        return "image://plexydesk/torrent";

    /* Unknown */
    else
        return "image://plexydesk/unknown";
}

function getType(filename)
{
    var isDotThere = filename.lastIndexOf('.');
    if (isDotThere > 0){
        var type = filename.substring(isDotThere+1,filename.length);
        return type;
        }
    else
        return "";
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
