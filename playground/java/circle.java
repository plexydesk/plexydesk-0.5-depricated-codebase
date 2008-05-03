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
import java.io.*;
import java.util.Scanner;
 
 public class circle {
    
    Point center = new Point (0,0);
    Point movement = new Point (0,0);
    double radius;
    
    public circle(double x, double y, double radiu){ 
        center.setX(x);
        center.setY(y);
        radius=radiu;
    }
    
    public void setMovement(double x, double y){
        movement.setX(x);
        movement.setY(y);
    }
    
    public void setXYRa(double x, double y, double radiu){
        center.setX(x);
        center.setY(y);
        radius=radiu;
    
    }
    
    public double getRadius(){
        return radius;
    }
 }
        
